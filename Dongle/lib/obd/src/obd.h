#ifndef OBD_H
#define OBD_H

#include <vector>
#include <utility.h>

#include <FreematicsONE.h>

#include <types.h>
#include <signals.h>

namespace obd {

class ObdDevice
{
    public:
        ObdDevice(COBDSPI* baseLayer);
        //functions from freematics only encapsulated here
        bool initialize();
        void uninit();
    	void reset();
    	void end();

        int getValueOfPid(char pid, bool& successful);

        bool updateVeryFastPids();
        bool updateFastPids();
        bool updateNormalPids();
        bool updateSlowPids();

        //dont delete the vectors i will update them when i am called
        std::vector<ourTypes::pidData>* getVeryFastPids();
        std::vector<ourTypes::pidData>* getFastPids();
        std::vector<ourTypes::pidData>* getNormalPids();
        std::vector<ourTypes::pidData>* getSlowPids();

        std::vector<ourTypes::dtcData>* getDiagnositcTroubleCodes();
        void clearDiagnosticTroubleCodes();

        char* getVehicleIdentificationNumber();


    private:
        bool updatePidVector(std::vector<ourTypes::pidData>* pidVector);

        COBDSPI* baseLayer;
        std::vector<ourTypes::pidData>* veryFastPids;
        std::vector<ourTypes::pidData>* fastPids;
        std::vector<ourTypes::pidData>* normalPids;
        std::vector<ourTypes::pidData>* slowPids;
};

};//end namespace obd

#endif
