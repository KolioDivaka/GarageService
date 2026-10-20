//
// Created by Kolio on 10/17/2026.
//

#include "AutoWorkshopManager.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "../Repositories/CustomerRepository.h"
#include "../Repositories/VehicleRepository.h"
#include "../Repositories/RepairOrderRepository.h"

using namespace std;

void AutoWorkshopManager::addCustomer(const Customer &customer) {
    customerRepo.addCustomer(customer);
    fileManager.saveCustomers(customerRepo.getCustomers());
}

int AutoWorkshopManager::acceptVehicles(int customerId, int vehicleId, const string &customerStatement) {

    if (!customerRepo.customerExists(customerId)) {
        throw invalid_argument("Invalid customer ID: " + to_string(customerId));
    }
    if (!vehicleRepo.vehicleExists(vehicleId)) {
        throw invalid_argument("Invalid vehicle ID: " + to_string(vehicleId));
    }

    const Vehicle& vehicle = vehicleRepo.findVehicleById(vehicleId);
    if (vehicle.getCustomerId() != customerId) {
        throw invalid_argument("Vehicle does not belong to the specified customer!");
    }

    if (orderRepo.findActiveRepairByVehicleId(vehicleId) != nullptr) {
        throw invalid_argument("Cannot admit a vehicle which already is in the Workshop!");
    }

    int newOrderId = orderRepo.getNextRepairOrderId();
    RepairOrder newOrder(newOrderId, customerId, vehicleId, customerStatement);

    orderRepo.addOrder(newOrder);
    fileManager.saveRepairs(orderRepo.getRepairOrders());

    return newOrderId;
}

void AutoWorkshopManager::editCustomer(int customerId, const string &name, const string &phoneNumber) {
    customerRepo.editCustomer(customerId, name, phoneNumber);
    fileManager.saveCustomers(customerRepo.getCustomers());
}

void AutoWorkshopManager::addVehicle(const Vehicle &vehicle) {

    if (!customerRepo.customerExists(vehicle.getCustomerId())) {
        throw invalid_argument("Invalid customer ID: " + to_string(vehicle.getCustomerId()));
    }
    vehicleRepo.addVehicle(vehicle);
    fileManager.saveVehicles(vehicleRepo.getVehicles());
}

void AutoWorkshopManager::editVehicle(int vehicleId, const std::string &brand, const std::string &model,
    const std::string &regNumber, std::optional<int> yearOfManufacture) {
    vehicleRepo.editVehicle(vehicleId, brand, model, regNumber, yearOfManufacture);
    fileManager.saveVehicles(vehicleRepo.getVehicles());
}

void AutoWorkshopManager::startRepair(int orderId) {
    RepairOrder& order = orderRepo.findOrderById(orderId);
    order.startRepair();
    fileManager.saveRepairs(orderRepo.getRepairOrders());
}

void AutoWorkshopManager::completeRepair(int orderId) {
    RepairOrder& order = orderRepo.findOrderById(orderId);
    order.completeRepair();
    fileManager.saveRepairs(orderRepo.getRepairOrders());
}

void AutoWorkshopManager::payRepair(int orderId) {
    RepairOrder& order = orderRepo.findOrderById(orderId);
    order.payRepair();
    fileManager.saveRepairs(orderRepo.getRepairOrders());
}
void AutoWorkshopManager::addRepairService(int orderId, const RepairService &service) {
    RepairOrder& order = orderRepo.findOrderById(orderId);
    order.addRepairService(service);
    fileManager.saveRepairs(orderRepo.getRepairOrders());
}
void AutoWorkshopManager::addParts(int orderId, const Part &part) {
    RepairOrder& order = orderRepo.findOrderById(orderId);
    order.addPart(part);
    fileManager.saveRepairs(orderRepo.getRepairOrders());
}

const RepairOrder * AutoWorkshopManager::findActiveRepairByRegNum(const string &regNum) const {
    int foundVehicleId = -1;
    for (const Vehicle& v : vehicleRepo.getVehicles()) {
        if (v.getVehicleRegNum() == regNum) {
            foundVehicleId = v.getVehicleId();
            break;
        }
    }

    if (foundVehicleId == -1) {
        throw invalid_argument("Cannot find vehicle with registration number: " + regNum);
    }

    // След това търсим по ID на колата в поръчките
    return orderRepo.findActiveRepairByVehicleId(foundVehicleId);
}

double AutoWorkshopManager::calculateTurnover(const TimePoint &from, const TimePoint &to) const {
    if (from > to) {
        throw invalid_argument("Start date cannot be greater than end date!");
    }
    double turnover = 0.0;
    for (const RepairOrder& order : orderRepo.getRepairOrders()) {
        if (order.getStatus() == RepairStatus::Paid) {
            const auto paidDate = order.getPaidDate();
            if (!paidDate.has_value()) {
                throw logic_error("Paid repair has no payment date " + to_string(order.getOrderId()));
            }
            if (*paidDate >= from && *paidDate <= to) {
                turnover += order.calculateTotalCost();
            }
        }
    }

    return turnover;

}