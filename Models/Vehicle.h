//
// Created by Kolio on 10/16/2026.
//

#ifndef GARAGESERVICE_VEHICLE_H
#define GARAGESERVICE_VEHICLE_H
#include <string>

using namespace std;

class Vehicle {
private:
    pmr::string carBrand;
    string carModel;
    string regNumber;
    short yearOfMaking;

    static short getCurrentYear();

public:

    Vehicle();
    Vehicle(const string& carBrand, const string& carModel, const string& regNumber, short yearOfMaking);

    [[nodiscard]] pmr::string car_brand() const {
        return carBrand;
    }

    void set_car_brand(const pmr::string &car_brand) {
        carBrand = car_brand;
    }

    [[nodiscard]] string car_model() const {
        return carModel;
    }

    void set_car_model(const string &car_model) {
        carModel = car_model;
    }

    [[nodiscard]] string reg_number() const {
        return regNumber;
    }

    void set_reg_number(const string &reg_number) {
        regNumber = reg_number;
    }

    [[nodiscard]] short year_of_making() const {
        return yearOfMaking;
    }

    void set_year_of_making(short year_of_making) {
        if (year_of_making >= 1900 && year_of_making <= getCurrentYear()) {
            yearOfMaking = year_of_making;
        }
    }


};


#endif //GARAGESERVICE_VEHICLE_H
