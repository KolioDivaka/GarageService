//
// Created by Kolio on 10/17/2026.
//

#include "AutoWorkshopManager.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include "ClientHandler.h"
#include "DataLoader.h"
#include "../Repositories/CustomerRepository.h"
#include "../Repositories/VehicleRepository.h"
#include "../Repositories/RepairOrderRepository.h"
#include "../Utility/StringUtils.h"


AutoWorkshopManager::AutoWorkshopManager(FileManager& fileManager,
                                         CustomerRepository& customerRepository,
                                         VehicleRepository& vehicleRepository,
                                         RepairOrderRepository& repairOrderRepository)
    : fileManager(fileManager),
      customerRepo(customerRepository),
      vehicleRepo(vehicleRepository),
      orderRepo(repairOrderRepository) {

    DataLoader dataLoader(fileManager);
    dataLoader.loadIntoRepositories(customerRepo, vehicleRepo, orderRepo);
}

AutoWorkshopManager::AutoWorkshopManager(FileManager& fileManager)
    : fileManager(fileManager),
      customerRepo(*(new CustomerRepository())),
      vehicleRepo(*(new VehicleRepository())),
      orderRepo(*(new RepairOrderRepository())) {}

void AutoWorkshopManager::addCustomer(const Customer &customer) {
    customerRepo.addCustomer(customer);
    fileManager.saveCustomers(customerRepo.getCustomers());
}

int AutoWorkshopManager:: acceptVehicles(int customerId, int vehicleId, const std::string &customerStatement) {

    if (!customerRepo.customerExists(customerId)) {
        throw std::invalid_argument("Invalid customer ID: " + std::to_string(customerId));
    }
    if (!vehicleRepo.vehicleExists(vehicleId)) {
        throw std::invalid_argument("Invalid vehicle ID: " + std::to_string(vehicleId));
    }

    const Vehicle& vehicle = vehicleRepo.findVehicleById(vehicleId);
    if (vehicle.getCustomerId() != customerId) {
        throw std::invalid_argument("Vehicle does not belong to the specified customer!");
    }

    if (orderRepo.findActiveRepairByVehicleId(vehicleId) != nullptr) {
        throw std::invalid_argument("Cannot admit a vehicle which already is in the Workshop!");
    }

    int newOrderId = orderRepo.getNextRepairOrderId();
    RepairOrder newOrder(newOrderId, customerId, vehicleId, customerStatement);

    orderRepo.addOrder(newOrder);
    fileManager.saveRepairs(orderRepo.getRepairOrders());

    return newOrderId;
}

void AutoWorkshopManager::editCustomer(int customerId, const std::string &name, const std::string &phoneNumber) {
    customerRepo.editCustomer(customerId, name, phoneNumber);
    fileManager.saveCustomers(customerRepo.getCustomers());
}

std::vector<Customer> AutoWorkshopManager::findCustomersByName(const std::string &namePart) const {
    const std::string needle = StringUtils::normalizeForSearch(namePart);

    if (needle.empty()) {
        return customerRepo.getCustomers();
    }

    std::vector<Customer> result;

    for (const Customer& customer : customerRepo.getCustomers()) {
        if (StringUtils::normalizeForSearch(customer.getName()).find(needle)
            != std::string::npos) {
            result.push_back(customer);
            }
    }

    return result;
}

const Customer& AutoWorkshopManager::findCustomerById(int clientId ) const {
    return customerRepo.findCustomerById(clientId);
}

void AutoWorkshopManager::addVehicle(const Vehicle &vehicle) {

    if (!customerRepo.customerExists(vehicle.getCustomerId())) {
        throw std::invalid_argument("Invalid customer ID: " + std::to_string(vehicle.getCustomerId()));
    }
    vehicleRepo.addVehicle(vehicle);
    fileManager.saveVehicles(vehicleRepo.getVehicles());
}

void AutoWorkshopManager::editVehicle(int vehicleId, const std::string &brand, const std::string &model,
    const std::string &regNumber, std::optional<int> yearOfManufacture) {
    vehicleRepo.editVehicle(vehicleId, brand, model, regNumber, yearOfManufacture);
    fileManager.saveVehicles(vehicleRepo.getVehicles());
}

std::vector<Vehicle> AutoWorkshopManager::getVehiclesByCustomerId(int customerId) const {
    std::vector<Vehicle> result;

    for (const Vehicle& v : vehicleRepo.getVehicles()) {
        if (v.getCustomerId() == customerId) {
            result.push_back(v);
        }
    }

    return result;
}

const Vehicle & AutoWorkshopManager::findVehicleByRegNum(const std::string &regNum) const {
    return vehicleRepo.findByRegNum(regNum);
}

const Vehicle& AutoWorkshopManager::findVehicleById(int vehicleId) const {
    return vehicleRepo.findVehicleById(vehicleId);
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

const RepairOrder * AutoWorkshopManager::findActiveRepairByRegNum(const std::string &regNum) const {
    int foundVehicleId = -1;
    for (const Vehicle& v : vehicleRepo.getVehicles()) {
        if (v.getVehicleRegNum() == regNum) {
            foundVehicleId = v.getVehicleId();
            break;
        }
    }

    if (foundVehicleId == -1) {
        throw std::invalid_argument("Cannot find vehicle with registration number: " + regNum);
    }

    // След това търсим по ID на колата в поръчките
    return orderRepo.findActiveRepairByVehicleId(foundVehicleId);
}

std::vector<RepairOrder> AutoWorkshopManager::getRepairHistoryByRegNum(const std::string &regNum) const {
    const Vehicle* vehicle = nullptr;

    for (const Vehicle& v : vehicleRepo.getVehicles()) {
        if (v.getVehicleRegNum() == regNum) {
            vehicle = &v;
            break;
        }
    }

    if (!vehicle) {
        throw std::invalid_argument(
            "Cannot find vehicle with registration number: " + regNum
        );
    }

    std::vector<RepairOrder> history;

    for (const RepairOrder& order : orderRepo.getRepairOrders()) {
        if (order.getVehicleId() == vehicle->getVehicleId()) {
            history.push_back(order);
        }
    }

    return history;
}

double AutoWorkshopManager::calculateTurnover(const TimePoint &from, const TimePoint &to) const {

    if (from >= to) {
        throw std::invalid_argument(
            "Start date must be before end date!"
        );
    }

    double turnover = 0.0;

    for (const RepairOrder& order : orderRepo.getRepairOrders()) {
        if (order.getStatus() != RepairStatus::Paid) {
            continue;
        }

        const auto paidDate = order.getPaidDate();

        if (!paidDate.has_value()) {
            throw std::logic_error(
                "Paid repair has no payment date: "
                + std::to_string(order.getOrderId())
            );
        }

        if (*paidDate >= from && *paidDate < to) {
            turnover += order.calculateTotalCost();
        }
    }

    return turnover;
}

const RepairOrder& AutoWorkshopManager::getOrderById(
    int orderId
) const {
    return orderRepo.findOrderById(orderId);
}

std::vector<RepairOrder> AutoWorkshopManager::listUnfinishedRepairs() const {
    return orderRepo.getUnfinishedRepairs();
}

int AutoWorkshopManager::getCustomersNextId() {
    return customerRepo.getNextCustomerId();
}

int AutoWorkshopManager::getVehiclesNextId() {
    return vehicleRepo.getVehicleNextId();
}

int AutoWorkshopManager::getRepairsNextId() const {
    return orderRepo.getNextRepairOrderId();
}

RepairOrder& AutoWorkshopManager::getOrderById(int orderId) {
    return const_cast<RepairOrder&>(
        static_cast<const AutoWorkshopManager*>(this)
            ->getOrderById(orderId)
    );
}
