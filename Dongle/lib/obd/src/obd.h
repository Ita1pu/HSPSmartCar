#ifndef OBD_H
#define OBD_H

//#include <vector>
//#include <utility.h>

#include <avr/pgmspace.h>

#include <FreematicsONE.h>

#include <types.h>
#include <signals.h>

namespace obd {

class ObdDevice
{
    public:
        ObdDevice(COBDSPI* baseLayer);
        ~ObdDevice();
        //functions from freematics only encapsulated here
        bool initialize();
        void uninit();
    	void reset();
    	void end();

        int getValueOfPid(char pid, bool& successful);

        char updateVeryFastPids();
        char updateNormalPids();
        char updateSlowPids();
        //char updateFastPids(); //nicht in benutzung, haben hier keine ausgewählt

        //dont delete the array i will update it when i am called
        ourTypes::pidData* getPidArray();
        //this array have to be delete after infos are used
        ourTypes::dtcData* getDiagnositcTroubleCodes(unsigned char& amount);

        //std::vector<ourTypes::dtcData>* getDiagnositcTroubleCodes();
        void clearDiagnosticTroubleCodes();

        char* getVehicleIdentificationNumber();

        bool isPidValid(unsigned char pid);
        bool getClamp15State();


    private:
        bool Clamp15 = false;
        bool wasAlreadyInitialiesed = false;
        COBDSPI* baseLayer;
        OBD_PROTOCOLS lastUsedProtocol;
        //std::vector<ourTypes::dtcData>* dtcVector = nullptr;//diagnostic trouble codes vector



        void identifyMaxPidArrayLength();

        unsigned char maxLengthPidArray = 0;
        ourTypes::pidData* pidArray;
};

};//end namespace obd

#endif
