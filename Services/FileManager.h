//
// Created by Kolio on 10/18/2026.
//

#ifndef GARAGESERVICE_FILEMANAGER_H
#define GARAGESERVICE_FILEMANAGER_H
#include <vector>

#include "../Models/Customer.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"

class RepairOrder;
class Customer;

class FileManager {
private:
    static constexpr const char* CUSTOMERS_FILE = "customers.txt";
    static constexpr const char* VEHICLES_FILE = "vehicles.txt";
    static constexpr const char* REPAIR_ORDERS_FILE = "repair_orders.txt";


public:
    [[nodiscard]] std::vector<Customer> loadCustomers() const;
    [[nodiscard]] std::vector<Vehicle> loadVehicles() const;
    [[nodiscard]] std::vector<RepairOrder> loadRepairOrders() const;

    void saveCustomers(const std::vector<Customer>& customers) const;
    void saveVehicles(const std::vector<Vehicle>& vehicles) const;
    void saveRepairs(const std::vector<RepairOrder>& orders) const;




};


#endif //GARAGESERVICE_FILEMANAGER_H
