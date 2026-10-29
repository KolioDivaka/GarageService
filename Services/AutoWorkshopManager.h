#ifndef GARAGESERVICE_AUTOWORKSHOPMANAGER_H
#define GARAGESERVICE_AUTOWORKSHOPMANAGER_H

#include <chrono>
#include <vector>
#include <string>
#include <optional>
#include "FileManager.h"
#include "../Repositories/CustomerRepository.h"
#include "../Repositories/VehicleRepository.h"
#include "../Repositories/RepairOrderRepository.h"
// Forward declarations за избягване на кръгови зависимости
class FileManager;
class CustomerRepository;
class VehicleRepository;
class RepairOrderRepository;

using TimePoint = std::chrono::system_clock::time_point;

class AutoWorkshopManager {
private:
    FileManager& fileManager;
    CustomerRepository& customerRepo;
    VehicleRepository& vehicleRepo;
    RepairOrderRepository& orderRepo;

public:

    AutoWorkshopManager(FileManager& fileManager,
                        CustomerRepository& customerRepository,
                        VehicleRepository& vehicleRepository,
                        RepairOrderRepository& repairOrderRepository):
    fileManager(fileManager),
    customerRepo(customerRepository),
    vehicleRepo(vehicleRepository),
    orderRepo(repairOrderRepository) {
        customerRepo.setCustomers(fileManager.loadCustomers());
        vehicleRepo.setVehicles(fileManager.loadVehicles());
        orderRepo.setRepairOrders(fileManager.loadRepairOrders());
    }

    // Customer Methods
    void addCustomer(const Customer& customer);
    void editCustomer(int customerId, const std::string& name, const std::string& phoneNumber);
    std::vector<Customer> findCustomersByName(const std::string& namePart) const;
    const Customer& findCustomerById(int clientId) const;

    // Vehicle Methods
    void addVehicle(const Vehicle& vehicle);
    void editVehicle(int vehicleId, const std::string& brand, const std::string& model, const std::string& regNumber, std::optional<int> yearOfManufacture = std::nullopt);
    std::vector<Vehicle> getVehiclesByCustomerId(int customerId) const;
    const Vehicle& findVehicleByRegNum(const std::string& regNum) const;
    const Vehicle& findVehicleById(int vehicleId) const;

    // Business Logic
    int acceptVehicles(int customerId, int vehicleId, const std::string& customerStatement);
    void startRepair(int orderId);
    void completeRepair(int orderId);
    void payRepair(int orderId);
    void addRepairService(int orderId, const RepairService& service);
    void addParts(int orderId, const Part& part);

    // Spravki i tursene
    const RepairOrder* findActiveRepairByRegNum(const std::string& regNum) const;
    std::vector<RepairOrder> getRepairHistoryByRegNum(const std::string& regNum) const;
    double calculateTurnover(const TimePoint& from, const TimePoint& to) const;
    RepairOrder& getOrderById(int orderId);
    const RepairOrder& getOrderById(int orderId) const;
    [[nodiscard]] std::vector<RepairOrder> listUnfinishedRepairs() const;

    int getCustomersNextId();
    int getVehiclesNextId();
    int getRepairsNextId() const;
};

#endif // GARAGESERVICE_AUTOWORKSHOPMANAGER_H