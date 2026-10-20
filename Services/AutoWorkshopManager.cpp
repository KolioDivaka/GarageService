//
// Created by Kolio on 10/17/2026.
//

#include "AutoWorkshopManager.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <limits>

using namespace std;

namespace{
    bool isBlank(const string& text) {
        return text.find_first_not_of(" \t\r\n\f\v") == string::npos;
    }
};

void AutoWorkshopManager::loadFiles() {

    customers = fileManager.loadCustomers();
    vehicles = fileManager.loadVehicles();
    repairOrders = fileManager.loadRepairOrders();

    validateLoadedData();
    updateNextIds();
}

void AutoWorkshopManager::saveFiles() const {
    fileManager.saveCustomers(customers);
    fileManager.saveVehicles(vehicles);
    fileManager.saveRepairs(repairOrders);
}

void AutoWorkshopManager::validateLoadedData() const {

    unordered_set<int> customerIds;
    unordered_set<int> vehicleIds;
    unordered_set<int> orderIds;
    unordered_set<int> vehiclesWithActiveOrders;
    unordered_set<string> registrationNumbers;

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

    if (maxCustomerId == maxInt ||
        maxVehicleId == maxInt ||
        maxRepairOrderId ==     maxInt) {
        throw overflow_error(
            "Cannot generate repair order IDs: int limit reached!");
    }
    nextCustomerId = maxCustomerId+1;
    nextVehicleId = maxVehicleId+1;
    nextRepairOrderId = maxRepairOrderId+1;
}

void AutoWorkshopManager::addOrder(const RepairOrder &repair_order) {
    repairOrders.push_back(repair_order);
}

RepairOrder& AutoWorkshopManager::findOrderById(int orderId) {
    RepairOrder* order = nullptr;

    for ( RepairOrder& repair_order : repairOrders) {
        if (repair_order.getOrderId()== orderId) {
            order = &repair_order;
            break;
        }
    }
    if (!order) {
        throw invalid_argument("Cannot find an order with ID:  " + std::to_string(orderId));
    }

    return *order;

}

Vehicle & AutoWorkshopManager::findVehicleById(int vehicleId) {
    Vehicle* vehicle = nullptr;

    for (Vehicle& vehicle1 : vehicles) {
        if (vehicle1.getVehicleId() == vehicleId) {
            vehicle = &vehicle1;
            break;
        }
    }
    if (!vehicle) {
        throw invalid_argument("Cannot find vehicle with ID:"+to_string(vehicleId));
    }

    return *vehicle;
}

Customer & AutoWorkshopManager::findCustomerById(int customerId) {
    Customer* customer = nullptr;

    for (Customer& c : customers) {
        if (c.getCustomerId()==customerId) {
            customer = &c;
            break;
        }
    }
    if (!customer) {
        throw invalid_argument("Cannot find customer with ID:"+to_string(customerId));
    }
    return *customer;
}

bool AutoWorkshopManager::customerExists(int customerId) const {
    for (const Customer& customer : getCustomers()) {
        if (customer.getCustomerId() == customerId) {
            return true;
        }
    }
    return false;
}

bool AutoWorkshopManager::vehicleExists(int vehicleId) const {
    for (const Vehicle& vehicle : getVehicles()) {
        if (vehicle.getVehicleId() == vehicleId) {
            return true;
        }
    }
    return false;
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

int AutoWorkshopManager::acceptVehicles(int customerId, int vehicleId, const string &customerStatement) {


     if (!customerExists(customerId)) {
            throw invalid_argument("Invalid customer ID: " + to_string(customerId));
     }

    if (!vehicleExists(vehicleId)){
        throw invalid_argument("Invalid vehicle ID: " + to_string(vehicleId));
    }

    const Vehicle& vehicle = findVehicleById(vehicleId);

    if (vehicle.getCustomerId() != customerId) {
        throw invalid_argument(
            "Vehicle does not belong to the specified customer!"
        );
    }


    for (const RepairOrder& order : repairOrders) {
        if (order.getVehicleId()==vehicleId
            && (order.getStatus()== RepairStatus::Accepted
                || order.getStatus()== RepairStatus::InProgress)) {
                    throw invalid_argument("Cannot admit a vehicle which already is in the Workshop!");

        }
    }

    if (getNextRepairOrderId() == std::numeric_limits<int>::max()) {
        throw overflow_error(
            "Cannot accept another vehicle: repair order ID limit reached!"
        );
    }

    RepairOrder newOrder(getNextRepairOrderId(), customerId, vehicleId, customerStatement);

    addOrder(newOrder);
    updateNextIds();

    fileManager.saveRepairs(repairOrders);

    return newOrder.getOrderId();
}

void AutoWorkshopManager::addCustomer(const Customer &customer) {
    if (customerExists(customer.getCustomerId())) {
        throw invalid_argument( "Customer ID already exists: " + to_string(customer.getCustomerId()) );
    }

    if (customer.getCustomerId() == std::numeric_limits<int>::max()) {
        throw overflow_error("Cannot add customer: ID limit reached!");
    }

    customers.push_back(customer);
    updateNextIds();

    fileManager.saveCustomers(customers);
}

void AutoWorkshopManager::addVehicle(const Vehicle &vehicle) {

    if (vehicleExists(vehicle.getVehicleId())) {
        throw invalid_argument("Vehicle ID already exists: " +to_string( vehicle.getVehicleId()));

    }

    for (const Vehicle& v : vehicles) {
        if (v.getVehicleRegNum()== vehicle.getVehicleRegNum()) {
            throw invalid_argument("Vehicle registration already exists: " + v.getVehicleRegNum());
        }
    }

    if (vehicle.getVehicleId() == numeric_limits<int>::max()) {
        throw overflow_error("Cannot add vehicle: ID limit reached!");
    }

    if (!customerExists(vehicle.getCustomerId())) {
        throw invalid_argument(
            "Invalid customer ID: "
            + to_string(vehicle.getCustomerId())
        );
    }

    vehicles.push_back(vehicle);
    updateNextIds();

    fileManager.saveVehicles(vehicles);
}

void AutoWorkshopManager::editCustomer(int customerId, const string &name, const string &phoneNumber) {

    Customer& customer = findCustomerById(customerId);

    if (isBlank(name) && isBlank(phoneNumber)) {
        return;
    }

    Customer updatedCustomer = customer;
    if (!isBlank(name)) {
        updatedCustomer.setName(name);
    }
    if (!isBlank(phoneNumber)) {
        updatedCustomer.setPhoneNumber(phoneNumber);
    }
    customer = updatedCustomer;

    fileManager.saveCustomers(customers);
}

void AutoWorkshopManager::editVehicle(int vehicleId, const std::string &brand, const std::string &model,
    const std::string &regNumber, std::optional<int> yearOfManufacture) {

    Vehicle& vehicle = findVehicleById(vehicleId);

    if (isBlank(brand)
        && isBlank(model)
        && isBlank(regNumber)
        && !yearOfManufacture.has_value()) {
        return;
        }

    Vehicle updatedVehicle = vehicle;

    if (!isBlank(brand)) {
        updatedVehicle.setVehicleBrand(brand);
    }

    if (!isBlank(model)) {
        updatedVehicle.setVehicleModel(model);
    }

    if (!isBlank(regNumber)) {
        updatedVehicle.setVehicleRegNumber(regNumber);

        for (const Vehicle& other : vehicles) {
            if (other.getVehicleId() != vehicleId
                && other.getVehicleRegNum()
                    == updatedVehicle.getVehicleRegNum()) {
                throw invalid_argument(
                    "Vehicle registration already exists: "
                    + updatedVehicle.getVehicleRegNum()
                );
                    }
        }
    }

    if (yearOfManufacture.has_value()) {
        updatedVehicle.setYearOfManufacture(
            yearOfManufacture.value()
        );
    }

    vehicle = updatedVehicle;

    fileManager.saveVehicles(vehicles);
}


void AutoWorkshopManager::startRepair(int orderId) {
   RepairOrder& order =(findOrderById(orderId));

    order.startRepair();
    fileManager.saveRepairs(repairOrders);
}

void AutoWorkshopManager::completeRepair(int orderId) {
    RepairOrder& order = findOrderById(orderId);
    order.completeRepair();

    fileManager.saveRepairs(repairOrders);
}

void AutoWorkshopManager::payRepair(int orderId) {
    RepairOrder& order = findOrderById(orderId);
    order.payRepair();

    fileManager.saveRepairs(repairOrders);
}

void AutoWorkshopManager::addRepairService(int orderId, const RepairService &service) {
    RepairOrder& order = findOrderById(orderId);

    order.addRepairService(service);

    fileManager.saveRepairs(repairOrders);
}

void AutoWorkshopManager::addParts(int orderId, const Part &part) {

    RepairOrder& order = findOrderById(orderId);

    order.addPart(part);

    fileManager.saveRepairs(repairOrders);
}

const RepairOrder * AutoWorkshopManager::findActiveRepairByRegNum(const string &regNum) const {

    const Vehicle* vehicle = nullptr;

    for (const Vehicle& v : vehicles) {
        if (v.getVehicleRegNum() == regNum) {
            vehicle = &v;
            break;
        }
    }
    if (!vehicle) {
        throw invalid_argument("Cannot find vehicle with registration number: "
            + regNum);
    }
    for (const RepairOrder& order : repairOrders) {
        if (order.getVehicleId() == vehicle->getVehicleId()
            && (order.getStatus()== RepairStatus::Accepted
                || order.getStatus()== RepairStatus::InProgress)){
                return &order;
            }
    }
    return nullptr;

}

vector<RepairOrder> AutoWorkshopManager::getUnfinishedRepairs() const {
    vector<RepairOrder> unfinishedRepairs;
    for (const RepairOrder& order : repairOrders) {
        if (order.getStatus()== RepairStatus::Accepted
            || order.getStatus()== RepairStatus::InProgress) {
            unfinishedRepairs.push_back(order);
        }
    }

    return unfinishedRepairs;
}

double AutoWorkshopManager::calculateTurnover(const TimePoint &from, const TimePoint &to) const {
    if (from > to) {
        throw invalid_argument("Start date cannot be greater than end date!");
    }

    double turnover = 0.0;

    for (const RepairOrder& order : repairOrders) {
        if (order.getStatus() != RepairStatus::Paid) {
            continue;
        }

        const auto paidDate = order.getPaidDate();

        if (!paidDate.has_value()) {
            throw logic_error("Paid repair has no payment date " + to_string(order.getOrderId()));
        }
        if (*paidDate >= from && *paidDate <= to) {
            turnover += order.calculateTotalCost();
        }

    }
    return turnover;
}

const RepairOrder & AutoWorkshopManager::findOrderById(int orderId) const {
    for (const RepairOrder& order : repairOrders) {
        if (order.getOrderId() == orderId) {
            return order;
        }
    }

    throw invalid_argument(
        "Cannot find an order with ID: "
        + to_string(orderId)
    );
}

