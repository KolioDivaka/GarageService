//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_VEHICLEREPOSITORY_H
#define GARAGESERVICE_VEHICLEREPOSITORY_H
#include "../Models/Vehicle.h"
#include <optional>
#include <string>
#include <vector>


using namespace std;

class VehicleRepository {

private:
   vector<Vehicle> vehicles;
   int nextVehicleId = 1;

public:
   Vehicle& findVehicleById(int vehicleId);
   [[nodiscard]] bool vehicleExists(int vehicleId) const;
   void addVehicle( const Vehicle &vehicle );
   void editVehicle( int vehicleId, const string& brand, const string& model, const string& regNumber, optional<int> yearOfManufacture = std::nullopt );
   const Vehicle& findByRegNum(const std::string& regNum) const;
    [[nodiscard]] vector<Vehicle> getVehicles() const {
         return vehicles;
      }

      [[nodiscard]] int getVehicleNextId() const {
         return nextVehicleId;
      }
   void setVehicles( const vector<Vehicle>& loadedVehicles ) {
       this->vehicles = loadedVehicles;
       updateNextVehicleId();
    }

private:
   void updateNextVehicleId();

};


#endif //GARAGESERVICE_VEHICLEREPOSITORY_H
