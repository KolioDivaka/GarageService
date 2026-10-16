//
// Created by Kolio on 10/16/2026.
//

#include "Vehicle.h"

#include <chrono>
#include <cmath>
#include <bits/chrono.h>


short Vehicle::getCurrentYear() {
    const auto now = chrono::system_clock::now();
    const chrono::year_month_day ymd{floor<chrono::days>(now)};

    return static_cast<short>(static_cast<int>(ymd.year()));
}

Vehicle::Vehicle() {
    this->carBrand = "Unknown";
    this->carModel = "Unknown";
    this->regNumber = "XXXXXXX";
    this->yearOfMaking = 0000;

}

Vehicle::Vehicle(const string &carBrand, const string &carModel, const string &regNumber, short yearOfMaking) {
    this->carBrand = carBrand;
    this->carModel = carModel;
    this->regNumber = regNumber;
    this->yearOfMaking = yearOfMaking;
}

