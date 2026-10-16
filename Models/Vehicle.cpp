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

