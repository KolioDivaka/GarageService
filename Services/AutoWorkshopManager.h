//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#define GARAGESERVICE_AUTOWORKSHOPMANAGER_H

#include "FileManager.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"
#include "../Repositories/CustomerRepository.h"
#include "../Repositories/VehicleRepository.h"
#include "../Repositories/RepairOrderRepository.h"



class RepairOrderRepository;
class VehicleRepository;
using TimePoint = std::chrono::system_clock::time_point;
using namespace std;

class Vehicle;
class FileManager;

class AutoWorkshopManager {
private:
    FileManager& fileManager;

    CustomerRepository& customerRepo;
    VehicleRepository& vehicleRepo;
    RepairOrderRepository& orderRepo;

public:
    AutoWorkshopManager(FileManager& fileManger,
        CustomerRepository& customerRepository,
        VehicleRepository& vehicleRepository,
        RepairOrderRepository& repairOrderRepository)
    :fileManager(fileManger),customerRepo(customerRepository),vehicleRepo(vehicleRepository),orderRepo(repairOrderRepository){}

    //Customer Methods
    void addCustomer(const Customer& customer);
    void editCustomer(int customerId, const string& name, const string& phoneNumber);
    std::vector<Customer> findCustomersByName(const std::string& namePart) const;
    const Customer& findCustomerById(int clientId) const;
    //Vehicle Methods
    void addVehicle(const Vehicle& vehicle);
    void editVehicle( int vehicleId, const std::string& brand, const std::string& model, const std::string& regNumber, std::optional<int> yearOfManufacture = std::nullopt );
    vector<Vehicle> getVehiclesByCustomerId(int customerId) const;
    const Vehicle& findVehicleByRegNum(const std::string& regNum) const;
    const Vehicle& findVehicleById(int vehicleId) const;
    //Buisness Logic
    int acceptVehicles(int customerId, int vehicleId, const string &customerStatement);
    void startRepair(int orderId);
    void completeRepair(int orderId);
    void payRepair(int orderId);
    void addRepairService(int orderId, const RepairService& service);
    void addParts(int orderId, const Part& part);

    //Spravki i tursene
    const RepairOrder* findActiveRepairByRegNum(const string& regNum) const;
    vector<RepairOrder> getRepairHistoryByRegNum(const string& regNum) const;
    double calculateTurnover(const TimePoint& from, const TimePoint& to) const;
    RepairOrder& getOrderById(int orderId);
    const RepairOrder& getOrderById(int orderId) const;


    int getCustomersNextId() {
        return customerRepo.getNextCustomerId();
    }
    int getVehiclesNextId() {
        return vehicleRepo.getVehicleNextId();
    }
    int getRepairsNextId() {
        return orderRepo.getNextRepairOrderId();
    }

};


#endif //GARAGESERVICE_AUTOWORKSHOPMANAGER_H
