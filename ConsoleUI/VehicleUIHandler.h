//
// Created by Kolio on 10/29/2026.
//

#ifndef GARAGESERVICE_VEHICLEUIHANDLER_H
#define GARAGESERVICE_VEHICLEUIHANDLER_H

#include "../Services/AutoWorkshopManager.h"
#include <string>
class VehicleUIHandler {
private:
    AutoWorkshopManager& manager;

public:
    explicit VehicleUIHandler(AutoWorkshopManager& manager) : manager(manager) {}

    int promptNewVehicle(int presetCustomerId = 0);
    void handleEditVehicle();
    int chooseVehicle(int ownerId);
    void handleFindByRegNum();
};


#endif //GARAGESERVICE_VEHICLEUIHANDLER_H
