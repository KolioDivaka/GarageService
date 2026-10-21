//
// Created by Kolio on 10/21/2026.
//

#include "VehicleRepository.h"
#include "../Models/Vehicle.h"
#include <limits>
#include <stdexcept>

namespace{
    bool isBlank(const string& text) {
        return text.find_first_not_of(" \t\r\n\f\v") == string::npos;
    }
};

Vehicle & VehicleRepository::findVehicleById(int vehicleId) {
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

bool VehicleRepository::vehicleExists(int vehicleId) const {
    for (const Vehicle& vehicle : vehicles) {
        if (vehicle.getVehicleId() == vehicleId) {
            return true;
        }
    }
    return false;
}

void VehicleRepository::addVehicle(const Vehicle &vehicle) {

    if (vehicleExists(vehicle.getVehicleId())) {
        throw std::invalid_argument("Vehicle ID already exists: " + std::to_string(vehicle.getVehicleId()));
    }

    for (const Vehicle& v : vehicles) {
        if (v.getVehicleRegNum() == vehicle.getVehicleRegNum()) {
            throw std::invalid_argument("Vehicle registration already exists: " + v.getVehicleRegNum());
        }
    }

    if (vehicle.getVehicleId() == std::numeric_limits<int>::max()) {
        throw std::overflow_error("Cannot add vehicle: ID limit reached!");
    }



    vehicles.push_back(vehicle);
    updateNextVehicleId();


}

void VehicleRepository::editVehicle(int vehicleId, const string &brand, const string &model, const string &regNumber,
    optional<int> yearOfManufacture) {

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
}

const Vehicle & VehicleRepository::findByRegNum(const std::string &regNum) const {
    for (const Vehicle& vehicle : vehicles) {
        if (vehicle.getVehicleRegNum() == regNum) {
            return vehicle;
        }
    }

    throw std::invalid_argument(
        "Cannot find vehicle with registration number: " + regNum
    );
}

void VehicleRepository::updateNextVehicleId() {
    int maxVehicleId = 0;

    for (const Vehicle& v : vehicles) {
        maxVehicleId = max(maxVehicleId,v.getVehicleId());
    }

    constexpr int maxInt = std::numeric_limits<int>::max();

    if (maxVehicleId == maxInt) {
        throw overflow_error(
            "Cannot generate repair order IDs: int limit reached!");
    }

    nextVehicleId = maxVehicleId+1;

}
