//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_REPAIRBILL_H
#define GARAGESERVICE_REPAIRBILL_H
#include <chrono>
#include <iomanip>
#include <iosfwd>

#include "../Services/AutoWorkshopManager.h"

class Customer;
class Vehicle;
class RepairOrder;
using namespace std;

class RepairBill {
private:
    const RepairOrder& order;
    const Customer& customer;
    const Vehicle& vehicle;

public:
    RepairBill(
        const RepairOrder& order,
        const Customer& customer,
        const Vehicle& vehicle
    ) noexcept : order(order), customer(customer), vehicle(vehicle) {};

    const RepairOrder& getOrder() const noexcept;
    const Customer& getCustomer() const noexcept;
    const Vehicle& getVehicle() const noexcept;
};

std::ostream& operator<<(std::ostream& os, const RepairBill& bill);

#endif //GARAGESERVICE_REPAIRBILL_H
