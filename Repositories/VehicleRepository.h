//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_VEHICLEREPOSITORY_H
#define GARAGESERVICE_VEHICLEREPOSITORY_H
#include "../Models/Vehicle.h"
#include <optional>
#include <string>
#include <vector>




class VehicleRepository {

private:
   std::vector<Vehicle> vehicles;
   int nextVehicleId = 1;

public:
   Vehicle& findVehicleById(int vehicleId);
   [[nodiscard]] bool vehicleExists(int vehicleId) const;
   void addVehicle( const Vehicle &vehicle );
   void editVehicle( int vehicleId, const std::string& brand, const std::string& model, const std::string& regNumber, std::optional<int> yearOfManufacture = std::nullopt );
   const Vehicle& findByRegNum(const std::string& regNum) const;
    [[nodiscard]] std::vector<Vehicle> getVehicles() const {
         return vehicles;
      }

      [[nodiscard]] int getVehicleNextId() const {
         return nextVehicleId;
      }
   void setVehicles( const std::vector<Vehicle>& loadedVehicles ) {
       this->vehicles = loadedVehicles;
       updateNextVehicleId();
    }

private:
   void updateNextVehicleId();

};


#endif //GARAGESERVICE_VEHICLEREPOSITORY_H
