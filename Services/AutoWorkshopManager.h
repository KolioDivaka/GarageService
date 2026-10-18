//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#define GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#include <vector>

#include "FileManager.h"
#include "../Models/Customer.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"


class Vehicle;
class FileManager;

class AutoWorkshopManager {
private:
    FileManager& fileManager;

    std::vector<Customer> customers;
    std::vector<Vehicle> vehicles;
    std::vector<RepairOrder> repairOrders;

    int nextCustomerId = 1;
    int nextVehicleId = 1;
    int nextRepairOrderId = 1;

private:
    void loadFiles();
    void saveFiles() const;
    void validateLoadedData() const;
    void updateNextIds();


public:
    AutoWorkshopManager( FileManager& fileManager);
    AutoWorkshopManager(const AutoWorkshopManager& other);
};


#endif //GARAGESERVICE_AUTOWORKSHOPMANAGER_H
