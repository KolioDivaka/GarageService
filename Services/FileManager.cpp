//
// Created by Kolio on 10/18/2026.
//

#include "FileManager.h"

#include <filesystem>
#include <fstream>
#include <unordered_set>
using namespace std;

namespace {
    using TimePoint = std::chrono::system_clock::time_point;

    std::optional<TimePoint> readSavedDate(const std::string& text);
    string writeSavedDate(const optional<TimePoint>& date);
}

std::vector<Customer> FileManager::loadCustomers() const { // NOLINT(*-convert-member-functions-to-static)
    if (!std::filesystem::exists(CUSTOMERS_FILE)) {
        return {};
    }

    std::ifstream file(CUSTOMERS_FILE);

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Cannot open file: ") + CUSTOMERS_FILE);
    }

    vector<Customer> loadedCustomers;
    pmr::unordered_set<int> usedIds;

    string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;

        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        istringstream row(line);

        int customerId;
        string name;
        string phoneNumber;


        if (!(row>>customerId>> std::quoted(name)>> std::quoted(phoneNumber))) {
            throw std::runtime_error("Invalid customer record at line"+to_string(lineNum));
        }

        row>> std::ws;

        if (!row.eof()) {
            throw std::runtime_error("Unexpected data at line "+ std::to_string(lineNum));
        }

        if (customerId<=0) {
            throw std::runtime_error("Invalid customer record at line"+ std::to_string(lineNum));
        }

        if (usedIds.insert(customerId).second) {
            throw std::runtime_error("Duplicate customer record at line"+ std::to_string(lineNum));
        }

        try {
            loadedCustomers.emplace_back(customerId, name, phoneNumber);
        }catch (const std::logic_error& e) {
            throw std::runtime_error("Invalid customer at line "+ std::to_string(lineNum)+ ": "+e.what());
        }
    }
    if (file.bad() || (file.fail() && file.eof())) {
        throw std::runtime_error("Error loading customer records from file " + std::to_string(*CUSTOMERS_FILE));
    }

    return loadedCustomers;
}

std::vector<Vehicle> FileManager::loadVehicles() const { // NOLINT(*-convert-member-functions-to-static)
     if (!std::filesystem::exists(VEHICLES_FILE)) {
        return {};
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

    return loadedVehicles;
}

std::vector<RepairOrder> FileManager::loadRepairOrders() const { // NOLINT(*-convert-member-functions-to-static)

    if (!filesystem::exists(REPAIR_ORDERS_FILE)) {
        return {};
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
                requireNextLine();
                istringstream serviceRow(line);

                string description;
                double hours;
                double hourlyRate;

                if (!(serviceRow >> quoted(description) >> hours >> hourlyRate)) {
                    throw std::runtime_error("Invalid repair service!");
                }

                ensureEndOfLine(serviceRow);

                services.emplace_back(description, hours, hourlyRate);
            }

            vector<Part> parts;
            const int partCount = readCount();

            for (int i = 0; i < partCount; ++i) {
                requireNextLine();
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
    return loadedOrders;
}

void FileManager::saveCustomers(const std::vector<Customer> &customers) const {
    const filesystem::path targetPath = CUSTOMERS_FILE;

    filesystem::path tempPath = targetPath;
    tempPath+= ".tmp";

    ofstream file(tempPath,ios::out | ios::trunc);

    if (!file.is_open()) {
        throw runtime_error("Cannot open temporary customers file" + tempPath.string());
    }

    for (const Customer& customer : customers) {
        file<< customer.getCustomerId()<< ' '
        << quoted(customer.getName()) << ' '
        << quoted(customer.getPhoneNumber()) << '\n';
    }
    file.close();

    if (!file) {
        throw runtime_error("Failed to write customers file: " + tempPath.string());
    }

    error_code error;
    filesystem::rename(tempPath, targetPath, error);
    if (error) {
        throw std::runtime_error("Cannot replace customers file: " + error.message());
    }
}

void FileManager::saveVehicles(const std::vector<Vehicle> &vehicles) const {
    const std::filesystem::path targetPath = VEHICLES_FILE;

    std::filesystem::path tempPath = targetPath;
    tempPath += ".tmp";

    std::ofstream file(tempPath, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot open temporary vehicles file: "
            + tempPath.string()
        );
    }

    for (const Vehicle& vehicle : vehicles) {
        file << vehicle.getVehicleId() << ' '
             << vehicle.getCustomerId() << ' '
             << std::quoted(vehicle.getVehicleBrand()) << ' '
             << std::quoted(vehicle.getVehicleModel()) << ' '
             << std::quoted(vehicle.getVehicleRegNum()) << ' '
             << vehicle.getYearOfManufacture() << '\n';
    }

    file.close();

    if (!file) {
        throw std::runtime_error(
            "Failed to write vehicles file: "
            + tempPath.string()
        );
    }

    std::error_code error;
    std::filesystem::rename(tempPath, targetPath, error);

    if (error) {
        throw std::runtime_error(
            "Cannot replace vehicles file: " + error.message()
        );
    }
}

void FileManager::saveRepairs(const std::vector<RepairOrder> &orders) const {

    constexpr size_t MAX_ITEM_PER_ORDER = 10000;

    const filesystem::path targetPath = REPAIR_ORDERS_FILE;

    filesystem::path tempPath = targetPath;
    tempPath += ".tmp";

    ofstream file(tempPath,ios::out | ios::trunc);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open temporary repairs file: " + tempPath.string());
    }

    file << setprecision(
        numeric_limits<double>::max_digits10);

    for (const RepairOrder& order : orders) {
        const auto& services = order.getServices();
        const auto& parts = order.getParts();

        if (services.size() > MAX_ITEM_PER_ORDER||
            parts.size() > MAX_ITEM_PER_ORDER) {
            throw std::runtime_error("Too many items in repair order: "
                + to_string(order.getOrderId()));
        }

        int statusCode;

        switch (order.getStatus()) {
            case RepairStatus::Accepted:
                statusCode = 0;
                break;
            case RepairStatus::InProgress:
                statusCode = 1;
                break;
            case RepairStatus::Completed:
                statusCode = 2;
                break;
            case RepairStatus::Paid:
                statusCode = 3;
                break;
            default:
                throw runtime_error("Invalid repair status!");
        }

        file<< order.getOrderId() << ' '
        << order.getCustomerId() << ' '
        << order.getVehicleId()<< ' '
        << statusCode << ' '
        << writeSavedDate( order.getAcceptedDate() ) << ' '
        << writeSavedDate( order.getCompleteDate() ) << ' '
        << writeSavedDate( order.getPaidDate() ) << ' '
        << quoted(order.getCustomerStatement())<< '\n';

        file<<services.size()<<'\n';

        for (const RepairService& service : services) {
            file<< quoted(service.getDerscription())<<' '
            << service.getHoursLabor() << ' '
            << service.getHourlyRate() << '\n';
        }
        file << parts.size()<<'\n';

        for (const Part& part : parts) {
            file<< quoted(part.getPartName()) << ' '
            << quoted(part.getPartNumber()) << ' '
            << part.getUnitPrice() << ' '
            << part.getQuantity() << '\n';
        }
    }

    file.close();
    if (!file) {
        throw std::runtime_error(
            "Failed to write repairs file: " + tempPath.string());
    }

    error_code error;
    filesystem::rename(tempPath, targetPath, error);
    if (error) {
        throw std::runtime_error("Cannot replace repairs file: " + error.message());
    }
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

    string writeSavedDate(const optional<TimePoint>& date) {
        if (!date.has_value()) {
            return "-";
        }

        const std::time_t rawTime =
            std::chrono::system_clock::to_time_t(*date);

        const std::tm* localTime = std::localtime(&rawTime);

        if (localTime == nullptr) {
            throw std::runtime_error(
                "Cannot convert saved date!"
            );
        }

        const std::tm calendarDate = *localTime;

        std::ostringstream result;
        result << std::put_time(&calendarDate, "%Y-%m-%d");

        if (!result) {
            throw std::runtime_error(
                "Cannot format saved date!"
            );
        }

        return result.str();
    }

}
