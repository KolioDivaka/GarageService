//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#define GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#include <vector>

#include "../Models/Customer.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"


class Vehicle;

class AutoWorkshopManager {
private:
    static constexpr const char* CUSTOMERS_FILE = "customers.txt";
    static constexpr const char* VEHICLES_FILE = "vehicles.txt";
    static constexpr const char* REPAIR_ORDERS_FILE = "repair_orders.txt";

    std::vector<Customer> customers;
    std::vector<Vehicle> vehicles;
    std::vector<RepairOrder> repairOrders;

    int nextCustomerId = 1;
    int nextVehicleId = 1;
    int nextRepairOrderId = 1;

private:
    void loadFiles();
    void loadCustomers();
    void loadVehicles();
    void loadRepairOrders();

    void validateLoadedData() const;
    void updateNextIds();


public:
    AutoWorkshopManager();

    AutoWorkshopManager(const AutoWorkshopManager& other);
};


#endif //GARAGESERVICE_AUTOWORKSHOPMANAGER_H
