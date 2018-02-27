using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace SmartCarApi.DataParser
{
    public class ValueParser
    {
        public static int ParseDataId(byte[] rawId)
        {
            if (rawId.Length < 2) return 0;
            
            return rawId[1] | (rawId[0] << 8);
        }

        public static int ParseInt32(byte[] rawInt)
        {
            if (rawInt.Length < 4) return 0;

            if (BitConverter.IsLittleEndian)
            {
                return BitConverter.ToInt32(rawInt.Reverse().ToArray(), 0);
            }

            return BitConverter.ToInt32(rawInt, 0);
        }

        public static long ParseInt64(byte[] rawInt)
        {
            if (rawInt.Length < 8) return 0;

            if (BitConverter.IsLittleEndian)
            {
                return BitConverter.ToInt64(rawInt.Reverse().ToArray(), 0);
            }

            return BitConverter.ToInt64(rawInt, 0);
        }
    }
 }
