#include <iostream>
#include "Logger.hpp"
#include "IDBWrapper.hpp"
#include "Client.hpp"
#include "MotorDeviceManager.hpp"
using namespace std;

int main()
{
    Logger::clearLogs();
    Logger::saveToFile("MotorDeviceManager: App started");
    Client* client = new Client();
    if(client->clientInitialize(12345))
    {
        client->connectToIDB();
        client->start();
    }
    client->sendRequest("0x0-MDM-Welcome");
    IDBWrapper* wrapper = new IDBWrapper(client, "MDM");
    MotorDeviceManager* mdm = new MotorDeviceManager(wrapper);
    mdm->connectToMotor();
    mdm->requestForMotorConfiguring();

    getchar();
    return 0;
}
