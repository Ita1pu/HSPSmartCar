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
        ~ObdDevice();
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

        bool isPidValid(unsigned char pid);
        bool getClamp15State();


    private:
        bool updatePidVector(std::vector<ourTypes::pidData>* pidVector);
        bool fillPidVectors();

        bool Clamp15 = false;
        bool wasAlreadyInitialiesed = false;
        COBDSPI* baseLayer;
        OBD_PROTOCOLS lastUsedProtocol;
        std::vector<ourTypes::pidData>* veryFastPids = nullptr;
        std::vector<ourTypes::pidData>* fastPids = nullptr;
        std::vector<ourTypes::pidData>* normalPids = nullptr;
        std::vector<ourTypes::pidData>* slowPids = nullptr;
        std::vector<ourTypes::dtcData>* dtcVector = nullptr;//diagnostic trouble codes vector
};

};//end namespace obd

#endif
