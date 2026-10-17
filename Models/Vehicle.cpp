//
// Created by Kolio on 10/16/2026.
//

#include "Vehicle.h"

Vehicle::Vehicle() {
    this->vehicleId = 0;
    this->customerId = 0;
    this->vehicleBrand = "Unknown";
    this->vehicleModel = "Unknown";
    this->vehicleRegNumber = "Unknown";
    this->yearOfManufacture = 0;
}

Vehicle::Vehicle(int vehicleId, int customerId, const std::string &vehicleBrand, const std::string &vehicleModel,
    const std::string &vehicleRegNumber, short yearOfManufacture) {
    setVehicleId(vehicleId);
    setCustomerId(customerId);
    setVehicleBrand(vehicleBrand);
    setVehicleModel(vehicleModel);
    setVehicleRegNumber(vehicleRegNumber);
    setYearOfManufacture(yearOfManufacture);

}

std::string Vehicle::normalizeRegNumber(const std::string &regNumber) {
    std::string normalized;

    for (unsigned char ch : regNumber) {
        if (std::isspace(ch) || ch == '-') {
            continue;
        }

        normalized += static_cast<char>(std::toupper(ch));
    }

    return normalized;
}

