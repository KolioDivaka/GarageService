//
// Created by Kolio on 10/16/2026.
//

#ifndef GARAGESERVICE_VEHICLE_H
#define GARAGESERVICE_VEHICLE_H
#include <chrono>
#include <regex>
#include <string>


class Vehicle {
private:
        int vehicleId{};
        int customerId{};
        std::string vehicleBrand;
        std::string vehicleModel;
        std::string vehicleRegNumber;
        short yearOfManufacture{};

        static short getCurrentYear() {
                using namespace std::chrono;

                const auto now = system_clock::now();
                const year_month_day ymd{floor<days>(now)};

                return static_cast<short>(static_cast<int>(ymd.year()));
        }

public:
        Vehicle();
        Vehicle(int vehicleId, int customerId, const std::string &vehicleBrand, const std::string &vehicleModel,
                const std::string &vehicleRegNumber, short yearOfManufacture);

        [[nodiscard]] int getVehicleId() const {
                return vehicleId;
        }

        [[nodiscard]] int getCustomerId() const {
                return customerId;
        }

        [[nodiscard]] std::string getVehicleBrand() const {
                return vehicleBrand;
        }

        [[nodiscard]] std::string getVehicleModel() const {
                return vehicleModel;
        }

        [[nodiscard]] std::string getVehicleRegNum() const {
                return vehicleRegNumber;
        }

        [[nodiscard]] short getYearOfManufacture() const {
                return yearOfManufacture;
        }

        void setVehicleId(int vehicle_id) {
                if (vehicle_id <= 0) {
                        throw std::invalid_argument("Id cannot be negative!!! ");
                }
                this->vehicleId = vehicle_id;

        }
        void setCustomerId(int customer_id) {
                if (customer_id <= 0) {
                        throw std::invalid_argument("Customer Id cannot be negative!!!");
                }
                this->customerId = customer_id;
        }

        void setVehicleModel(const std::string& vehicle_model) {
                if (vehicle_model.find_first_not_of(" \t\n\r") == std::string::npos) {
                        throw std::invalid_argument("Vehicle model cannot be empty!");
                }
                this->vehicleModel = vehicle_model;

        }
        void setVehicleBrand(const std::string& vehicle_brand) {
                if (vehicle_brand.find_first_not_of(" \t\n\r") == std::string::npos) {
                        throw std::invalid_argument("Vehicle brand cannot be empty!");
                }
                this->vehicleBrand = vehicle_brand;

        }

        void setYearOfManufacture(short year_Of_Manufacture) {
                if (year_Of_Manufacture >= 1900 && year_Of_Manufacture <= getCurrentYear()) {
                        this->yearOfManufacture = year_Of_Manufacture;
                }
                else {
                        throw std::invalid_argument("Invalid year of Manufacture! It should be between 1900 and " + std::to_string(getCurrentYear()));
                }
        }

        void setVehicleRegNumber(const std::string& new_reg_number) {
                const std::string normalized = normalizeRegNumber(new_reg_number);

                static const std::regex pattern(R"([A-Z0-9]{2,15})");

                if (!std::regex_match(normalized, pattern)) {
                        throw std::invalid_argument(
                            "Invalid registration number! Use 2 to 15 Latin letters "
                            "or digits. Spaces and hyphens are ignored."
                        );
                }

                this->vehicleRegNumber = normalized;
        }

        static std::string normalizeRegNumber(const std::string& regNumber);

};


#endif //GARAGESERVICE_VEHICLE_H
