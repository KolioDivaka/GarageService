//
// Created by Kolio on 10/17/2026.
//

#include "AutoWorkshopManager.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

using namespace std;

namespace {
    using TimePoint = std::chrono::system_clock::time_point;

    // 1. Forward declaration here
    std::optional<TimePoint> readSavedDate(const std::string& text);
}

void AutoWorkshopManager::loadFiles() {

    loadCustomers();
    loadVehicles();
    loadRepairOrders();

    validateLoadedData();
    updateNextIds();
}

void AutoWorkshopManager::loadCustomers() {
    if (!std::filesystem::exists(CUSTOMERS_FILE)) {
        return;
    }

    std::ifstream file(CUSTOMERS_FILE);

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Cannot open file: ") + CUSTOMERS_FILE);
    }

    vector<Customer> loadedCustomers;
    unordered_set<int> usedIds;

    string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;

        if (line.find_first_not_of(" /t/r/n") == std::string::npos) {
            continue;
        }

        istringstream row(line);

        int customerId;
        string name;
        string phoneNumber;


        if (!(row>>customerId>>quoted(name)>>quoted(phoneNumber))) {
            throw runtime_error("Invalid customer record at line"+to_string(lineNum));
        }

        row>> ws;

        if (!row.eof()) {
            throw runtime_error("Unexpected data at line "+to_string(lineNum));
        }

        if (customerId<=0) {
            throw runtime_error("Invalid customer record at line"+to_string(lineNum));
        }

        if (usedIds.insert(customerId).second) {
            throw runtime_error("Duplicate customer record at line"+to_string(lineNum));
        }

        try {
            loadedCustomers.emplace_back(customerId, name, phoneNumber);
        }catch (const logic_error& e) {
            throw runtime_error("Invalid customer at line "+to_string(lineNum)+ ": "+e.what());
        }
    }
    if (file.bad() || (file.fail() && file.eof())) {
        throw runtime_error("Error loading customer records from file " + to_string(*CUSTOMERS_FILE));
    }

    customers.swap(loadedCustomers);

}

void AutoWorkshopManager::loadVehicles() {
    if (!std::filesystem::exists(VEHICLES_FILE)) {
        return;
    }

    std::ifstream file(VEHICLES_FILE);

    if (!file.is_open()) {
        throw std::runtime_error(
            std::string("Cannot open file: ") + VEHICLES_FILE
        );
    }

    std::vector<Vehicle> loadedVehicles;
    std::unordered_set<int> usedIds;
    std::unordered_set<std::string> usedRegNumbers;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::istringstream row(line);

        int vehicleId;
        int customerId;
        std::string brand;
        std::string model;
        std::string regNumber;
        short year;

        if (!(row >> vehicleId
                  >> customerId
                  >> std::quoted(brand)
                  >> std::quoted(model)
                  >> std::quoted(regNumber)
                  >> year)) {
            throw std::runtime_error(
                "Invalid vehicle record at line "
                + std::to_string(lineNumber)
            );
                  }

        row >> std::ws;

        if (!row.eof()) {
            throw std::runtime_error(
                "Unexpected vehicle data at line "
                + std::to_string(lineNumber)
            );
        }

        if (vehicleId <= 0 || customerId <= 0) {
            throw std::runtime_error(
                "Vehicle and customer IDs must be positive at line "
                + std::to_string(lineNumber)
            );
        }

        if (!usedIds.insert(vehicleId).second) {
            throw std::runtime_error(
                "Duplicate vehicle ID at line "
                + std::to_string(lineNumber)
            );
        }

        try {
            Vehicle vehicle(
                vehicleId,
                customerId,
                brand,
                model,
                regNumber,
                year
            );

            const std::string normalizedRegNumber =
                Vehicle::normalizeRegNumber(regNumber);

            if (!usedRegNumbers.insert(normalizedRegNumber).second) {
                throw std::invalid_argument(
                    "Duplicate registration number: "
                    + normalizedRegNumber
                );
            }

            loadedVehicles.push_back(vehicle);
        } catch (const std::logic_error& error) {
            throw std::runtime_error(
                "Invalid vehicle at line "
                + std::to_string(lineNumber)
                + ": " + error.what()
            );
        }
    }
    if (file.bad() || (file.fail() && !file.eof())) {
        throw std::runtime_error(
            std::string("Error while reading file: ") + VEHICLES_FILE
        );
    }

    vehicles.swap(loadedVehicles);
}

void AutoWorkshopManager::loadRepairOrders() {

    if (filesystem::exists(REPAIR_ORDERS_FILE)) {
        return;
    }

    ifstream file(REPAIR_ORDERS_FILE);

    if (!file.is_open()) {
        throw std::runtime_error(
            string("Cannot open file: ") + REPAIR_ORDERS_FILE);
    }

    vector<RepairOrder> loadedOrders;
    unordered_set<int> usedIds;


    string line;
    int lineNumber = 0;

    auto readNextLine = [&]() -> bool {
        while (getline(file, line)) {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r\n") != std::string::npos) {
                return true;
            }
        }
        if (!file.eof() || file.bad()) {
            throw std::runtime_error("Error reading repair orders!");
        }
        return false;
    };

    auto requireNextLine = [&]() {
        if (!readNextLine()) {
            throw std::runtime_error("Unexpected extra data!");
        }
    };

    auto ensureEndOfLine = [](std::istringstream& row) {
        row >> std::ws;

        if (!row.eof()) {
            throw std::runtime_error("Unexpected extra data!");
        }
    };
    constexpr int MAX_ITEMS_PER_ORDER = 10000;

    auto readCount = [&]() -> int {
        requireNextLine();

        istringstream row(line);
        int count;
        if (!(row >> count) || count <0 || count > MAX_ITEMS_PER_ORDER) {
            throw std::runtime_error("Invalid item count!");
        }
        ensureEndOfLine(row);
        return count;
    };

    while (readNextLine()) {
        try {
            istringstream row(line);

            int orderId;
            int customerId;
            int vehicleId;
            int statusCode;

            string acceptedText;
            string completedText;
            string paidText;
            string customerStatement;

            if (!(row>> orderId
                >>customerId
                >>vehicleId
                >>statusCode
                >>acceptedText
                >>completedText
                >>paidText
                >>quoted(customerStatement)
                )) {
                throw std::runtime_error("Invalid order header!");
            }

            ensureEndOfLine(row);

            if (orderId<=0 || customerId<=0 || vehicleId<=0) {
                throw std::runtime_error("IDs must be positive!");
            }
            if (!usedIds.insert(orderId).second) {
                throw std::runtime_error("Duplicate order ID!");
            }

            RepairStatus status;
            switch (statusCode) {
                case 0:
                    status = RepairStatus::Accepted;
                    break;
                case 1:
                    status = RepairStatus::InProgress;
                    break;
                case 2:
                    status = RepairStatus::Completed;
                    break;
                case 3:
                    status = RepairStatus::Paid;
                    break;
                default:
                    throw std::runtime_error("Invalid status code!");
            }

            const auto accepted = readSavedDate(acceptedText);
            const auto completed = readSavedDate(completedText);
            const auto paid = readSavedDate(paidText);

            if (!accepted.has_value()) {
                throw std::runtime_error("Acceptance date is required!");
            }

            vector<RepairService> services;
            const int serviceCount = readCount();

            for (int i = 0; i < serviceCount; ++i) {
                readNextLine();
                istringstream serviceRow(line);

                string description;
                double hours;
                double hourlyRate;

                if (!(serviceRow >> description >> hours >> hourlyRate)) {
                    throw std::runtime_error("Invalid repair service!");
                }

                ensureEndOfLine(serviceRow);

                services.emplace_back(description, hours, hourlyRate);
            }

            vector<Part> parts;
            const int partCount = readCount();

            for (int i = 0; i < partCount; ++i) {
                readNextLine();
                istringstream partRow(line);

                string name;
                string partNumber;
                double unitPrice;
                int quantity;

                if (!(partRow>>quoted(name) >> quoted(partNumber)>>unitPrice>>quantity)) {
                    throw std::runtime_error("Invalid part!");
                }
                ensureEndOfLine(partRow);

                parts.emplace_back(name,partNumber,unitPrice,quantity);
            }

            loadedOrders.emplace_back(
                orderId,
                customerId,
                vehicleId,
                customerStatement,
                services,
                parts,
                status,
                *accepted,
                completed,
                paid
                );

        }catch (const std::logic_error& error) {
            throw std::runtime_error(
               "Invalid repair data near line "
               + std::to_string(lineNumber)
               + ": " + error.what()
           );
        }catch (const std::runtime_error& error) {
            throw std::runtime_error(
                "Cannot load repair near line "
                + std::to_string(lineNumber)
                + ": " + error.what()
            );
        }
    }
    repairOrders.swap(loadedOrders);
}

void AutoWorkshopManager::validateLoadedData() const {

    unordered_set<int> customerIds;
    unordered_set<int> vehicleIds;
    unordered_set<int> orderIds;

    for (const Customer& customer : customers) {
        const int id = customer.getCustomerId();

        if (id<=0) {
            throw runtime_error("Invalid customer ID: " + to_string(id));
        }
        if (!customerIds.insert(id).second) {
            throw std::runtime_error("Duplicate customer ID: " + to_string(id));
        }
    }

    for (const Vehicle& vehicle : vehicles) {
        const int id = vehicle.getVehicleId();
        const int ownerId= vehicle.getCustomerId();

        if (id<= 0) {
            throw runtime_error("Invalid vehicle ID: " + to_string(id));
        }

        if (!vehicleIds.insert(id).second) {
            throw std::runtime_error("Duplicate vehicle ID: " + to_string(id));
        }
        if (!customerIds.contains(ownerId)) {
            throw std::runtime_error("Vehicle "
                + to_string(id)
                + " refers to missing customer "
                + to_string(ownerId));
        }

    }

    for (const RepairOrder& order : repairOrders) {
        const int id = order.getOrderId();
        const int customerId = order.getCustomerId();
        const int vehicleId = order.getVehicleId();

        if (id<= 0) {
            throw runtime_error("Invalid order ID: " + to_string(id));
        }

        if (!orderIds.insert(id).second) {
            throw std::runtime_error("Duplicate order ID: " + to_string(id));
        }

        if (!customerIds.contains(customerId)) {
            throw std::runtime_error("Repair order "+to_string(id)
                +" refers to missing customer "+
                to_string(customerId));
        }
        if (!vehicleIds.contains(vehicleId)) {
            throw std::runtime_error("Repair order "+to_string(id)
                +" refers to missing vehicle "+
                to_string(vehicleId));
        }
    }

}

void AutoWorkshopManager::updateNextIds() {
    int maxCustomerId = 0;
    int maxVehicleId = 0;
    int maxRepairOrderId = 0;

    for (const Customer& customer : customers) {
        maxCustomerId = max(maxCustomerId, customer.getCustomerId());
    }
    for (const Vehicle& vehicle : vehicles) {
        maxVehicleId = max(maxVehicleId, vehicle.getVehicleId());
    }
    for (const RepairOrder& order : repairOrders) {
        maxRepairOrderId = max(maxRepairOrderId, order.getOrderId());
    }

    constexpr int maxInt = std::numeric_limits<int>::max();

    if (maxCustomerId>maxInt ||
        maxVehicleId>maxInt ||
        maxRepairOrderId>maxInt) {
        throw overflow_error(
            "Cannot generate repair order IDs: int limit reached!");
    }
    nextCustomerId = maxCustomerId+1;
    nextVehicleId = maxVehicleId+1;
    nextRepairOrderId = maxRepairOrderId+1;
}

AutoWorkshopManager::AutoWorkshopManager() {
    loadFiles();
}

AutoWorkshopManager::AutoWorkshopManager(const AutoWorkshopManager &other) {
    customers = other.customers;
    vehicles = other.vehicles;
    repairOrders = other.repairOrders;

    nextCustomerId = other.nextCustomerId;
    nextVehicleId = other.nextVehicleId;
    nextRepairOrderId = other.nextRepairOrderId;
}


namespace {
    using TimePoint = std::chrono::system_clock::time_point;

    std::optional<TimePoint> readSavedDate(const std::string& text) {
        if (text == "-") {
            return std::nullopt;
        }

        std::istringstream input(text);
        long long seconds;

        if (!(input >> seconds)) {
            throw std::runtime_error("Invalid saved date: " + text);
        }

        input >> std::ws;

        if (!input.eof()) {
            throw std::runtime_error("Invalid saved date: " + text);
        }

        constexpr  auto minSeconds =
            std::chrono::duration_cast<std::chrono::seconds>(
                TimePoint::min().time_since_epoch()
            ).count();

        constexpr  auto maxSeconds =
            std::chrono::duration_cast<std::chrono::seconds>(
                TimePoint::max().time_since_epoch()
            ).count();

        if (seconds < minSeconds || seconds > maxSeconds) {
            throw std::runtime_error("Saved date is out of range!");
        }

        return TimePoint{
            std::chrono::duration_cast<TimePoint::duration>(
                std::chrono::seconds{seconds}
            )
        };
    }
}



