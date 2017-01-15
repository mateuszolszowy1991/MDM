#ifndef MOTORDEVICEMANAGER_H
#define MOTORDEVICEMANAGER_H
#include <vector>
#include "Logger.hpp"
#include "IDBWrapper.hpp"
#include "Client.hpp"
using namespace std;

class MotorDeviceManager
{
    public:
        MotorDeviceManager(IDBWrapper* wrapper);
        virtual ~MotorDeviceManager();
        void connectToMotor();
        void requestForMotorConfiguring();
        void waitingForStartSignal();

    protected:

    private:
        IDBWrapper* wrapper_;
        Mo mdmMo_;
        Mo motorMo_;
        Client* motorClient_;
        vector<string> mdmMoPStates = {"Configuring", "Configured"};
        vector<string> splitString(string strPtr, char sign);
        void getMotorConfiguration();
        void createMotorObject(string data);
        void setMotorProceduralState(string state);
        void setMotorOperationalState(string state);

};

#endif // MOTORDEVICEMANAGER_H
