//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_CLIENTHANDLER_H
#define GARAGESERVICE_CLIENTHANDLER_H
#include "AutoWorkshopManager.h"

using namespace std;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class ClientHandler {
private:
    AutoWorkshopManager& manager;

public:
    ClientHandler(AutoWorkshopManager& manager):manager(manager) {};

    void run();

};


#endif //GARAGESERVICE_CLIENTHANDLER_H
