#ifndef OBD_H
#define OBD_H

#include <vector>
#include <utility.h>

#include <types.h>

namespace obd {

class ObdDevice
{
    public:
        ObdDevice();
        bool initialize();

        char* getValueOfPid(char pid, int& size); //todo: or i return a single struct element of pidData

        bool updateVeryFastPids();
        bool updateFastPids();
        bool updateNormalPids();
        bool updateSlowPids();

        std::vector<ourTypes::pidData> getVeryFastPids();
        std::vector<ourTypes::pidData> getFastPids();
        std::vector<ourTypes::pidData> getNormalPids();
        std::vector<ourTypes::pidData> getSlowPids();


    private:
        std::vector<ourTypes::pidData> veryFastPids;
        std::vector<ourTypes::pidData> fastPids;
        std::vector<ourTypes::pidData> normalPids;
        std::vector<ourTypes::pidData> slowPids;



};

};//end namespace obd

#endif
