//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_DATALOADER_H
#define GARAGESERVICE_DATALOADER_H
#include <vector>
#include "FileManager.h"
#include "../Models/Customer.h"
#include "../Models/Vehicle.h"
#include "../Models/RepairOrder.h"

class CustomerRepository;
class VehicleRepository;
class RepairOrderRepository;
using namespace std;
class DataLoader {
private:
    FileManager& fileManager;

    void validateData(const std::vector<Customer>& customers,
                      const std::vector<Vehicle>& vehicles,
                      const std::vector<RepairOrder>& repairOrders) const;

public:
    explicit DataLoader(FileManager& fm): fileManager(fm) {};

    // Този метод чете, валидира и пълни репозиторитата
    void loadIntoRepositories(CustomerRepository &customerRepo,
                              VehicleRepository &vehicleRepo,
                              RepairOrderRepository &orderRepo);
};


#endif //GARAGESERVICE_DATALOADER_H
