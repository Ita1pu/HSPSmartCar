using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Rest
{
    public class GpsCoordinate
    {
        private double _latitude;
        private double _longitude;

        public double Latitude
        {
            get { return _latitude; }
            set
            {
                if (value <-90 || value > 90)
                {
                    _latitude = value / 1000000;
                }
                else
                {
                    _latitude = value;
                }
            }

        }

        public double Longitude
        {
            get { return _longitude; }
            set
            {
                if (value <-180 || value > 180)
                {
                    _longitude = value / 1000000;
                }
                else
                {
                    _longitude = value;
                }
            }
        }

        public GpsCoordinate()
        {
            _latitude = 0;
            _longitude = 0;
        }

        public GpsCoordinate(double Latitude, double Longitude)
        {
            this.Latitude = Latitude;
            this.Longitude = Longitude;
        }

        public double GetDistance(GpsCoordinate other) //returens distance between given and the current gpsCoordinate in m after Haversine Formula
        {
            double distance = 0;

            double equatorialEarthRadius = 6378.1370D;
            double degree2radian = (Math.PI / 180D);

            double diffLongitude = (this.Longitude - other.Longitude) * degree2radian;
            double diffLatitude = (this.Latitude - other.Latitude) * degree2radian;

            double a = Math.Pow(Math.Sin(diffLatitude / 2), 2) + Math.Cos(this.Latitude * degree2radian)* Math.Cos(other.Latitude * degree2radian) * 
                Math.Pow(Math.Sin(diffLongitude / 2), 2);

            double c = 2 * Math.Atan2(Math.Sqrt(a), Math.Sqrt(1 - a));

            distance = equatorialEarthRadius * c;

            return distance*1000; //because it is calculated in km
        }


        public override string ToString()
        {
            return string.Format("{0},{1}", Latitude, Longitude);
        }
    }

}
