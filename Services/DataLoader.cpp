//
// Created by Kolio on 10/21/2026.
//

#include "DataLoader.h"

#include <unordered_set>

void DataLoader::validateData(const std::vector<Customer> &customers, const std::vector<Vehicle> &vehicles,
                              const std::vector<RepairOrder> &repairOrders) const {
    pmr::unordered_set<int> customerIds;
    unordered_set<int> vehicleIds;
    unordered_set<int> orderIds;
    unordered_set<int> vehiclesWithActiveOrders;
    unordered_set<string> registrationNumbers;

    for (const Customer& customer : customers) {
        const int id = customer.getCustomerId();

        if (id <= 0) {
            throw runtime_error("Invalid customer ID: " + to_string(id));
        }
        if (!customerIds.insert(id).second) {
            throw runtime_error("Duplicate customer ID: " + to_string(id));
        }
    }

    for (const Vehicle& vehicle : vehicles) {
        const int id = vehicle.getVehicleId();
        const int ownerId = vehicle.getCustomerId();

        if (id <= 0) {
            throw runtime_error("Invalid vehicle ID: " + to_string(id));
        }

        if (!vehicleIds.insert(id).second) {
            throw runtime_error("Duplicate vehicle ID: " + to_string(id));
        }
        if (!customerIds.contains(ownerId)) {
            throw runtime_error("Vehicle "
                + to_string(id)
                + " refers to missing customer "
                + to_string(ownerId));
        }
        if (!registrationNumbers.insert(vehicle.getVehicleRegNum()).second) {
            throw runtime_error("Duplicate vehicle registration number: "
                + vehicle.getVehicleRegNum()
            );
        }
    }

    for (const RepairOrder& order : repairOrders) {
        const int id = order.getOrderId();
        const int customerId = order.getCustomerId();
        const int vehicleId = order.getVehicleId();

        if (id <= 0) {
            throw runtime_error("Invalid order ID: " + to_string(id));
        }

        if (!orderIds.insert(id).second) {
            throw runtime_error("Duplicate order ID: " + to_string(id));
        }

        if (!customerIds.contains(customerId)) {
            throw runtime_error("Repair order " + to_string(id)
                + " refers to missing customer " +
                to_string(customerId));
        }
        if (!vehicleIds.contains(vehicleId)) {
            throw runtime_error("Repair order " + to_string(id)
                + " refers to missing vehicle " +
                to_string(vehicleId));
        }
        if (order.getStatus() == RepairStatus::Accepted
                || order.getStatus() == RepairStatus::InProgress) {
             if (!vehiclesWithActiveOrders.insert(vehicleId).second) {
                    throw runtime_error(
                        "Vehicle " + to_string(vehicleId)
                        + " has multiple active repair orders!"
                    );
                }
        }
    }
}

void DataLoader::loadIntoRepositories(CustomerRepository &customerRepo, VehicleRepository &vehicleRepo,
                                      RepairOrderRepository &orderRepo) {
    vector<Customer> customers = fileManager.loadCustomers();
    vector<Vehicle> vehicles = fileManager.loadVehicles();
    vector<RepairOrder> repairOrders = fileManager.loadRepairOrders();
}
