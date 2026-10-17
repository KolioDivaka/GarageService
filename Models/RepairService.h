//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_REPAIRSERVICE_H
#define GARAGESERVICE_REPAIRSERVICE_H
#include <stdexcept>
#include <string>


class RepairService {
private:
    std::string description;
    double hoursLabor{};
    double hourlyRate{};

public:
    RepairService();
    RepairService(const std::string& description ,double hoursLabor, double hourlyRate);

    [[nodiscard]] std::string description1() const {
        return description;
    }

    [[nodiscard]] double hours_labor() const {
        return hoursLabor;
    }

    [[nodiscard]] double hourly_rate() const {
        return hourlyRate;
    }


    void setDescription(const std::string& newDescription) {
        if (newDescription.find_first_not_of(" \t\n\r") == std::string::npos) {
            throw std::invalid_argument("Repair Description cannot be empty!");
        }
        this->description = newDescription;
    }

    void setHoursLabor(double hours_labor) {
        if (hours_labor < 0.0) {
           throw std::invalid_argument("Hours labor must be greater than zero!");
        }
        this->hoursLabor = hours_labor;
    }

    void setHourlyRate(double hourly_rate) {
        if (hourly_rate < 0.0) {
            throw std::invalid_argument("Hourly rate must be greater than zero!");
        }
        this->hourlyRate = hourly_rate;
    }

    [[nodiscard]]double calculateCost() const {
        return hoursLabor * hourlyRate;
    }


};


#endif //GARAGESERVICE_REPAIRSERVICE_H
