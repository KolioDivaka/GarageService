//
// Created by Kolio on 10/21/2026.
//

#include "DataLoader.h"
#include "../Repositories/CustomerRepository.h"
#include "../Repositories/VehicleRepository.h"
#include "../Repositories/RepairOrderRepository.h"
#include <unordered_set>

void DataLoader::validateData(const std::vector<Customer> &customers, const std::vector<Vehicle> &vehicles,
                              const std::vector<RepairOrder> &repairOrders) const {
    std::unordered_set<int> customerIds;
    std::unordered_set<int> vehicleIds;
    std::unordered_set<int> orderIds;
    std::unordered_set<int> vehiclesWithActiveOrders;
    std::unordered_set<std::string> registrationNumbers;

    for (const Customer& customer : customers) {
        const int id = customer.getCustomerId();

        if (id <= 0) {
            throw std::runtime_error("Invalid customer ID: " + std::to_string(id));
        }
        if (!customerIds.insert(id).second) {
            throw std::runtime_error("Duplicate customer ID: " + std::to_string(id));
        }
    }

    for (const Vehicle& vehicle : vehicles) {
        const int id = vehicle.getVehicleId();
        const int ownerId = vehicle.getCustomerId();

        if (id <= 0) {
            throw std::runtime_error("Invalid vehicle ID: " + std::to_string(id));
        }

        if (!vehicleIds.insert(id).second) {
            throw std::runtime_error("Duplicate vehicle ID: " + std::to_string(id));
        }
        if (!customerIds.contains(ownerId)) {
            throw std::runtime_error("Vehicle "
                                     + std::to_string(id)
                                     + " refers to missing customer "
                                     + std::to_string(ownerId));
        }
        if (!registrationNumbers.insert(vehicle.getVehicleRegNum()).second) {
            throw std::runtime_error("Duplicate vehicle registration number: "
                                     + vehicle.getVehicleRegNum()
            );
        }
    }

    for (const RepairOrder& order : repairOrders) {
        const int id = order.getOrderId();
        const int customerId = order.getCustomerId();
        const int vehicleId = order.getVehicleId();

        if (id <= 0) {
            throw std::runtime_error("Invalid order ID: " + std::to_string(id));
        }

        if (!orderIds.insert(id).second) {
            throw std::runtime_error("Duplicate order ID: " + std::to_string(id));
        }

        if (!customerIds.contains(customerId)) {
            throw std::runtime_error("Repair order " + std::to_string(id)
                                     + " refers to missing customer " +
                                     std::to_string(customerId));
        }
        if (!vehicleIds.contains(vehicleId)) {
            throw std::runtime_error(std::to_string(vehicleId) +
                                     ("Repair order " + std::to_string(id)
                                      + " refers to missing vehicle "));
        }
        if (order.getStatus() == RepairStatus::Accepted
                || order.getStatus() == RepairStatus::InProgress) {
             if (!vehiclesWithActiveOrders.insert(vehicleId).second) {
                    throw std::runtime_error(
                        "Vehicle " + std::to_string(vehicleId)
                        + " has multiple active repair orders!"
                    );
                }
        }
    }
}

void DataLoader::loadIntoRepositories(CustomerRepository &customerRepo, VehicleRepository &vehicleRepo,
                                    RepairOrderRepository &orderRepo) {
    std::vector<Customer> customers = fileManager->loadCustomers();
    std::vector<Vehicle> vehicles = fileManager->loadVehicles();
    std::vector<RepairOrder> repairOrders = fileManager->loadRepairOrders();


    validateData(customers, vehicles, repairOrders);


    customerRepo.setCustomers(std::move(customers));
    vehicleRepo.setVehicles(std::move(vehicles));
    orderRepo.setRepairOrders(std::move(repairOrders));
}