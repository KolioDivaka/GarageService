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
using TimePoint = std::chrono::system_clock::time_point;
using namespace std;

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

    void addOrder(const RepairOrder &repair_order);
    RepairOrder& findOrderById(int orderId);
    Vehicle& findVehicleById(int vehicleId);
    Customer& findCustomerById(int customerId);
    [[nodiscard]] bool customerExists(int customerId) const;
    [[nodiscard]] bool vehicleExists(int vehicleId) const;
public:
    AutoWorkshopManager( FileManager& fileManager);
    AutoWorkshopManager(const AutoWorkshopManager& other);

    [[nodiscard]] std::vector<Customer> getCustomers() const {
        return customers;
    }

    [[nodiscard]] std::vector<Vehicle> getVehicles() const {
        return vehicles;
    }

    [[nodiscard]] std::vector<RepairOrder> getRepairOrders() const {
        return repairOrders;
    }

    [[nodiscard]] int getNextCustomerId() const {
        return nextCustomerId;
    }

    [[nodiscard]] int getNextVehicleId() const {
        return nextVehicleId;
    }

    [[nodiscard]] int getNextRepairOrderId() const {
        return nextRepairOrderId;
    }

    void setCustomers(const std::vector<Customer> &customers1) {
        this->customers = customers1;
    }

    void setVehicles(const std::vector<Vehicle> &vehicles1) {
        this->vehicles = vehicles1;
    }

    void setRepairOrders(const std::vector<RepairOrder> &repair_orders) {
        repairOrders = repair_orders;
    }

    void setNextCustomerId(const int next_customer_id) {
        nextCustomerId = next_customer_id;
    }

    void setNextVehicleId(const int next_vehicle_id) {
        nextVehicleId = next_vehicle_id;
    }

    void setNextRepairOrderId(const int next_repair_order_id) {
        nextRepairOrderId = next_repair_order_id;
    }

    int acceptVehicles(int customerId, int vehicleId, const string &customerStatement);

    void addCustomer( const Customer &customer );
    void addVehicle( const Vehicle &vehicle );

    void editCustomer( int customerId, const string& name, const string& phoneNumber);
    void editVehicle( int vehicleId, const std::string& brand, const std::string& model, const std::string& regNumber, std::optional<int> yearOfManufacture = std::nullopt );


    void startRepair(int orderId);
    void completeRepair(int orderId);
    void payRepair(int orderId);

    void addRepairService(int orderId, const RepairService& service);
    void addParts(int orderId, const Part& part);

    const RepairOrder* findActiveRepairByRegNum(const string& regNum) const;

    vector<RepairOrder> getUnfinishedRepairs() const;

    double calculateTurnover(const TimePoint& from, const TimePoint& to) const;
    const RepairOrder& findOrderById(int orderId) const;
};


#endif //GARAGESERVICE_AUTOWORKSHOPMANAGER_H
