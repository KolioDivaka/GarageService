//
// Created by Kolio on 10/18/2026.
//

#ifndef GARAGESERVICE_FILEMANAGER_H
#define GARAGESERVICE_FILEMANAGER_H

#include <vector>
#include <string_view>

#include "../Models/Customer.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"

class FileManager {
private:
    std::string customersFilename = "customers.txt";
    std::string vehiclesFilename = "vehicles.txt";
    std::string repairOrdersFilename = "repair_orders.txt";

public:
    FileManager() = default;

    FileManager(std::string custFile, std::string vehFile, std::string repFile)
        : customersFilename(std::move(custFile)),
          vehiclesFilename(std::move(vehFile)),
          repairOrdersFilename(std::move(repFile)) {}



    [[nodiscard]] std::vector<Customer> loadCustomers() const;
    [[nodiscard]] std::vector<Vehicle> loadVehicles() const;
    [[nodiscard]] std::vector<RepairOrder> loadRepairOrders() const;

    void saveCustomers(const std::vector<Customer>& customers) const;
    void saveVehicles(const std::vector<Vehicle>& vehicles) const;
    void saveRepairs(const std::vector<RepairOrder>& orders) const;

    
    [[nodiscard]] std::string getCustomersFilename() const {
        return customersFilename;
    }

    void setCustomersFilename(const std::string& filename) {
        customersFilename = filename;
    }

    [[nodiscard]] std::string getVehiclesFilename() const {
        return vehiclesFilename;
    }

    void setVehiclesFilename(const std::string& filename) {
        vehiclesFilename = filename;
    }

    [[nodiscard]] std::string getRepairOrdersFilename() const {
        return repairOrdersFilename;
    }

    void setRepairOrdersFilename(const std::string& filename) {
        repairOrdersFilename = filename;
    }
};


#endif //GARAGESERVICE_FILEMANAGER_H
