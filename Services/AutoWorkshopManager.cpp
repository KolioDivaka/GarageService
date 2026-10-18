//
// Created by Kolio on 10/17/2026.
//

#include "AutoWorkshopManager.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>

using namespace std;


void AutoWorkshopManager::loadFiles() {

    customers = fileManager.loadCustomers();
    vehicles = fileManager.loadVehicles();
    repairOrders = fileManager.loadRepairOrders();

    validateLoadedData();
    updateNextIds();
}

void AutoWorkshopManager::validateLoadedData() const {

    unordered_set<int> customerIds;
    unordered_set<int> vehicleIds;
    unordered_set<int> orderIds;

    for (const Customer& customer : customers) {
        const int id = customer.getCustomerId();

        if (id<=0) {
            throw runtime_error("Invalid customer ID: " + to_string(id));
        }
        if (!customerIds.insert(id).second) {
            throw std::runtime_error("Duplicate customer ID: " + to_string(id));
        }
    }

    for (const Vehicle& vehicle : vehicles) {
        const int id = vehicle.getVehicleId();
        const int ownerId= vehicle.getCustomerId();

        if (id<= 0) {
            throw runtime_error("Invalid vehicle ID: " + to_string(id));
        }

        if (!vehicleIds.insert(id).second) {
            throw std::runtime_error("Duplicate vehicle ID: " + to_string(id));
        }
        if (!customerIds.contains(ownerId)) {
            throw std::runtime_error("Vehicle "
                + to_string(id)
                + " refers to missing customer "
                + to_string(ownerId));
        }

    }

    for (const RepairOrder& order : repairOrders) {
        const int id = order.getOrderId();
        const int customerId = order.getCustomerId();
        const int vehicleId = order.getVehicleId();

        if (id<= 0) {
            throw runtime_error("Invalid order ID: " + to_string(id));
        }

        if (!orderIds.insert(id).second) {
            throw std::runtime_error("Duplicate order ID: " + to_string(id));
        }

        if (!customerIds.contains(customerId)) {
            throw std::runtime_error("Repair order "+to_string(id)
                +" refers to missing customer "+
                to_string(customerId));
        }
        if (!vehicleIds.contains(vehicleId)) {
            throw std::runtime_error("Repair order "+to_string(id)
                +" refers to missing vehicle "+
                to_string(vehicleId));
        }
    }

}

void AutoWorkshopManager::updateNextIds() {
    int maxCustomerId = 0;
    int maxVehicleId = 0;
    int maxRepairOrderId = 0;

    for (const Customer& customer : customers) {
        maxCustomerId = max(maxCustomerId, customer.getCustomerId());
    }
    for (const Vehicle& vehicle : vehicles) {
        maxVehicleId = max(maxVehicleId, vehicle.getVehicleId());
    }
    for (const RepairOrder& order : repairOrders) {
        maxRepairOrderId = max(maxRepairOrderId, order.getOrderId());
    }

    constexpr int maxInt = std::numeric_limits<int>::max();

    if (maxCustomerId>maxInt ||
        maxVehicleId>maxInt ||
        maxRepairOrderId>maxInt) {
        throw overflow_error(
            "Cannot generate repair order IDs: int limit reached!");
    }
    nextCustomerId = maxCustomerId+1;
    nextVehicleId = maxVehicleId+1;
    nextRepairOrderId = maxRepairOrderId+1;
}

AutoWorkshopManager::AutoWorkshopManager(FileManager& fileManager): fileManager(fileManager) {
        loadFiles();
}

AutoWorkshopManager::AutoWorkshopManager(const AutoWorkshopManager &other):fileManager(other.fileManager) {
    customers = other.customers;
    vehicles = other.vehicles;
    repairOrders = other.repairOrders;

    nextCustomerId = other.nextCustomerId;
    nextVehicleId = other.nextVehicleId;
    nextRepairOrderId = other.nextRepairOrderId;
}
