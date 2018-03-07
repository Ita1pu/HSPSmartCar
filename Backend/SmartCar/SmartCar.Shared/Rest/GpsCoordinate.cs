using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Rest
{
    public class GpsCoordinate
    {
        public double _latitude { get; set; }

        public double _longitude { get; set; }

        public GpsCoordinate()
        {
            _latitude = 0;
            _longitude = 0;
        }

        public GpsCoordinate(double Latitude, double Longitude)
        {
            _latitude = Latitude;
            _longitude = Longitude;
        }

        public double GetDistance(GpsCoordinate other) //returens distance between given and the current gpsCoordinate in m after Haversine Formula
        {
            double distance = 0;

            double equatorialEarthRadius = 6378.1370D;
            double degree2radian = (Math.PI / 180D);

            double diffLongitude = (this._longitude - other._longitude) * degree2radian;
            double diffLatitude = (this._latitude - other._latitude) * degree2radian;

            double a = Math.Pow(Math.Sin(diffLatitude / 2), 2) + Math.Cos(this._latitude * degree2radian)* Math.Cos(other._latitude * degree2radian) * 
                Math.Pow(Math.Sin(diffLongitude / 2), 2);

            double c = 2 * Math.Atan2(Math.Sqrt(a), Math.Sqrt(1 - a));

            distance = equatorialEarthRadius * c;

            return distance*1000; //because it is calculated in km
        }


        public override string ToString()
        {
            return string.Format("{0},{1}", _latitude, _longitude);
        }
    }

}
