using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared;
using Microsoft.AspNetCore.Http;
using SmartCar.Shared.Database;
using SmartCar.Shared.Model;
using SmartCar.Shared.Model.Identity;
using SmartCar.Shared.Rest;
using SmartCarApi.Extensions;
using SmartCarApi.Statistics.Basic;

namespace SmartCarApi.DataParser
{
    public class TripParser
    {
        private const int EntrySize = 15;
        private const int NewTripSpan = 360;

        private readonly AppUser _user;
        private readonly ApplicationDbContext _db;

        private List<SignalMap> _signals;             

        public TripParser(ApplicationDbContext db, AppUser user)
        {
            _user = user;
            _db = db;
        }

        public ParseResult ParseLogfile(IFormFile file)
        {
            var tripData = ParseTripData(file);

            if (tripData.Any())
            {
                //Split the trips
                var splitTrips = SplitTrips(tripData);

                //Initially calculate trip data
                SetBasicData(splitTrips);

                _db.Trips.AddRange(splitTrips);
                _db.SaveChanges();

                var result = new ParseResult
                {
                    Success = true,
                    ParsedTrips = splitTrips.Count
                };

                return result;
            }

            return new ParseResult{Success = false};
        }

        private List<TripData> ParseTripData(IFormFile file)
        {
            var tripData = new List<TripData>();
            var reader = new BinaryReader(file.OpenReadStream());
            var tripEntry = new byte[EntrySize];
            int readChars = 0;

            _signals = _db.SignalMap.ToList();

            //Seek over null entry
            reader.BaseStream.Seek(EntrySize, SeekOrigin.Begin);

            //Initial read
            readChars = reader.Read(tripEntry, 0, EntrySize);

            while (readChars >= EntrySize)
            {
                var tripItem = new TripData
                {
                    Mvid = tripEntry[0],
                    Timestamp = GetEntryTimestamp(tripEntry),
                    SignalType = GetSignal(tripEntry),
                    Value = GetValue(tripEntry)
                };
                
                tripData.Add(tripItem);

                readChars = reader.Read(tripEntry, 0, EntrySize);
            }
            
            return tripData;
        }

        private List<Trip> SplitTrips(IEnumerable<TripData> rawTripDataSource)
        {
            var trips = new List<Trip>();
            var tripDateSource = rawTripDataSource.ToList();

            if (tripDateSource.Any())
            {
                var lastMvid = tripDateSource.FirstOrDefault()?.Mvid ?? 0;
                var lastTimestamp = tripDateSource.FirstOrDefault()?.Timestamp ?? new DateTime();
                var currentTrip = new Trip { User = _user, TripStart = lastTimestamp };

                foreach (var tripEntry in tripDateSource)
                {
                    //If a new mvid is found or the time difference between two consecutive entries is greater than
                    //the value defined by NewTripSpan, a new trip is created.
                    if (tripEntry.Mvid != lastMvid || (tripEntry.Timestamp - lastTimestamp).TotalSeconds > NewTripSpan)
                    {
                        trips.Add(currentTrip);
                        currentTrip = new Trip { User = _user, TripStart = tripEntry.Timestamp };
                    }

                    lastMvid = tripEntry.Mvid;
                    lastTimestamp = tripEntry.Timestamp;

                    currentTrip.TripData.Add(tripEntry);
                }

                //Add last active trip
                trips.Add(currentTrip);
            }

            return trips;
        }

        private void SetBasicData(List<Trip> trips)
        {
            var statistics = new TripStatistic();

            foreach (var trip in trips)
            {
                statistics.CalculateBasicData(trip);    
            }
        }

        private SignalMap GetSignal(byte[] tripEntry)
        {
            var parsedId = ValueParser.ParseDataId(tripEntry.SubArray(9, 2));
            return _signals.FirstOrDefault(sm => sm.SignalId == parsedId);
        }

        private DateTime GetEntryTimestamp(byte[] tripEntry)
        {
            var timestamp = DateTime.Parse("01.01.1970 00:00");
            var rawTime = tripEntry.SubArray(1, 8);

            var msTimestamp = ValueParser.ParseInt64(rawTime);
            timestamp = timestamp.AddMilliseconds(msTimestamp);

            return timestamp;
        }

        private int GetValue(byte[] tripEntry)
        {
            var rawValue = tripEntry.SubArray(11, 4);
            return ValueParser.ParseInt32(rawValue);
        }
    }
}
