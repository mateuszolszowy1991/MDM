#include "MotorDeviceManager.hpp"

MotorDeviceManager::MotorDeviceManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("MDM/INF: MotorDeviceManager: ctor");
    mdmMo_ = wrapper_->createObject("CAR/MDM", "Online", mdmMoPStates[0]);

}

MotorDeviceManager::~MotorDeviceManager()
{
    //dtor
}

void MotorDeviceManager::connectToMotor()
{
    motorClient_ = new Client();
    if(motorClient_->clientInitialize(15001))
    {
        while(!motorClient_->connectToIDB())
        {
            Logger::saveToFile("MDM/WRN: MotorDeviceManager: motor is not pingable");
        }
        motorClient_->start();
    }
    createMotorObject("Configuring;BENZ;1.6;16V");
    //string(motorClient_->getServerResponse()));
}

void MotorDeviceManager::createMotorObject(string data)
{
    vector<string> splittedData = splitString(data, ';');
    motorMo_ = wrapper_->createObject("CAR/MDM/MOTOR", "Online", splittedData[3]);
    wrapper_->expandObject(&motorMo_, {"Model", "Volume", "Type"}, {splittedData[0], splittedData[1], splittedData[2]});

}

void MotorDeviceManager::requestForMotorConfiguring()
{
    Logger::saveToFile("MDM/INF: requestForMotorConfiguring");
    string response = motorClient_->sendRequest("0x91");
    Logger::saveToFile(("MDM/INF: response: " + response).c_str());
    if(response == "0x91")
    {
        Logger::saveToFile("MDM/INF: motor is configuring");
        wrapper_->setProceduralState(&motorMo_, "Configuring");
    }
    response = motorClient_->receive();
    if(response == "0x92")
    {
        Logger::saveToFile("MDM/INF: motor is configured");
        wrapper_->setProceduralState(&motorMo_, "Configured");
    }
    //waitingForStartSignal();
}

void MotorDeviceManager::waitingForStartSignal()
{
    Mo startEngineReq = wrapper_->setObserverForObjectCreate("CAR/MDM/START_ENGINE_REQ");
    Logger::saveToFile("MDM/INF: waitingForStartSignal: START_ENGINE_REQ found");
    wrapper_->createObject("CAR/MDM/START_ENGINE_REQ/RESULT", "Online", "OK");
}



vector<string> MotorDeviceManager::splitString(string strPtr, char sign)
{
    for (int i=0; i < strPtr.length(); i++)
    {
        if (strPtr[i] == sign)
            strPtr[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(strPtr);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;
}

