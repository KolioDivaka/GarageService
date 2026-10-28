//
// Created by Kolio on 10/21/2026.
//

#include "ClientHandler.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../Models/RepairBill.h"
namespace {

    class InputClosed final : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Console input is closed.";
        }
    };

    std::string toString(RepairStatus status) {
        switch (status) {
            case RepairStatus::Accepted: return "Accepted";
            case RepairStatus::InProgress: return "In Progress";
            case RepairStatus::Completed: return "Completed";
            case RepairStatus::Paid: return "Paid";
            default: return "Unknown";
        }
    }

    std::string formatDate(const std::optional<TimePoint>& date) {
        if (!date.has_value()) {
            return "-";
        }

        const std::time_t rawTime =
            std::chrono::system_clock::to_time_t(*date);

        const std::tm* localTime = std::localtime(&rawTime);

        if (localTime == nullptr) {
            return "-";
        }

        const std::tm calendarDate = *localTime;

        std::ostringstream result;
        result << std::put_time(&calendarDate, "%Y-%m-%d");
        return result.str();
    }

     std::tm parseCalendarDate(const std::string& text) {
        // Изискваме точно DD.MM.YYYY.
        if (text.size() != 10
            || text[2] != '.'
            || text[5] != '.') {
            throw std::invalid_argument(
                "Invalid date format. Use DD.MM.YYYY."
            );
        }

        for (std::size_t i = 0; i < text.size(); ++i) {
            if (i == 2 || i == 5) {
                continue;
            }

            if (text[i] < '0' || text[i] > '9') {
                throw std::invalid_argument(
                    "The date must contain digits and dots only."
                );
            }
        }

        int day = 0;
        int month = 0;
        int year = 0;
        char firstDot = '\0';
        char secondDot = '\0';

        std::istringstream stream(text);

        if (!(stream >> day >> firstDot
                     >> month >> secondDot >> year)) {
            throw std::invalid_argument("Cannot read the date.");
        }

        if (year < 1900
            || month < 1 || month > 12
            || day < 1 || day > 31) {
            throw std::invalid_argument("Invalid calendar date.");
        }

        std::tm calendarDate{};
        calendarDate.tm_mday = day;
        calendarDate.tm_mon = month - 1;
        calendarDate.tm_year = year - 1900;
        calendarDate.tm_isdst = -1;

        const std::time_t timestamp = std::mktime(&calendarDate);

        if (timestamp == static_cast<std::time_t>(-1)) {
            throw std::invalid_argument(
                "Date is outside the supported range."
            );
        }

        // mktime може да премести невалидна дата
        // в следващ месец — отхвърляме такава промяна.
        if (calendarDate.tm_mday != day
            || calendarDate.tm_mon != month - 1
            || calendarDate.tm_year != year - 1900) {
            throw std::invalid_argument(
                "This calendar date does not exist."
            );
        }

        return calendarDate;
    }

    std::chrono::system_clock::time_point toTimePoint(
        std::tm date,
        bool nextDay = false
    ) {
        if (nextDay) {
            ++date.tm_mday;
        }

        date.tm_isdst = -1;

        const std::time_t timestamp = std::mktime(&date);

        if (timestamp == static_cast<std::time_t>(-1)) {
            throw std::invalid_argument(
                "Date is outside the supported range."
            );
        }

        return std::chrono::system_clock::from_time_t(timestamp);
    }

}

string ClientHandler::readLine(const string &prompt) {
    cout << prompt;

    string text;

    if (!getline(cin, text )) {
        throw InputClosed();
    }
    return text;
}

int ClientHandler::readInt(const string &prompt) {
    while (true) {
        const string text = readLine(prompt);

        istringstream input(text);
        int value;

        if (input >> value) {
            input >> ws;

            if (input.eof()) {
                return value;
            }
        }

        cout << "Enter a valid whole number.\n";
    }
}

int ClientHandler::readPositiveInt(const string &prompt) {

    while (true) {
        const int value = readInt(prompt);
        if (value > 0) {
            return value;
        }
        cout << "Enter a valid whole number.\n";
    }

}

optional<int> ClientHandler::readOptionalInt(const string &prompt) {
    while (true) {
        const std::string text = readLine(prompt);

        if (text.find_first_not_of(" \t\r\n")
            == std::string::npos) {
            return std::nullopt;   // празно = без промяна
            }

        std::istringstream input(text);
        int value;

        if (input >> value) {
            input >> std::ws;
            if (input.eof()) {
                return value;
            }
        }

        std::cout << "Enter a whole number or leave empty.\n";
    }
}

double ClientHandler::readPositiveDouble(const std::string &prompt) const {
    while (true) {
        const std::string raw = readLine(prompt);

        try {
            std::size_t consumed = 0;
            const double value = std::stod(raw, &consumed);

            if (consumed != raw.size()) {
                throw std::invalid_argument("");
            }
            if (value <= 0.0) {
                std::cout << "The number must be positive. Try again.\n";
                continue;
            }

            return value;
        } catch (const std::exception&) {
            std::cout << "Please enter a valid number "
                         "(e.g. 12.50).\n";
        }
    }
}

int ClientHandler::pickCustomerId(const vector<Customer> &matches) const {
    if (matches.empty()) {
    std::cout << "No customers found.\n";
    return 0;
}

    std::cout << "\nFound " << matches.size() << " customer(s):\n";

    for (std::size_t i = 0; i < matches.size(); ++i) {
        std::cout << (i + 1) << ") ID " << matches[i].getCustomerId()
                  << " : " << matches[i].getName()
                  << " (" << matches[i].getPhoneNumber() << ")\n";
    }

    const int choice = readPositiveInt("Choose [1-" +
        to_string(matches.size()) + "] or 0 to cancel: ");

    if (choice == 0) {
        return 0;
    }

    if (static_cast<std::size_t>(choice) > matches.size()) {
        std::cout << "Invalid choice.\n";
        return 0;
    }

    return matches[choice - 1].getCustomerId();

}

int ClientHandler::chooseCustomer() {
    while (true) {
        const std::string namePart =
            readLine("Search customer by name (or 'n' for new): ");

        if (namePart == "n" || namePart == "N") {
            const int newId = promptNewCustomer();
            if (newId != 0) {
                return newId;
            }
            continue;
        }

        std::vector<Customer> matches;

        try {
            matches = manager.findCustomersByName(namePart);
        } catch (const std::invalid_argument&) {
            std::cout << "Search text cannot be empty.\n";
            continue;
        }

        if (matches.empty()) {
            std::cout << "No match. "
                         "Press Enter to search again or 'n' to create new customer.\n";
            continue;
        }

        const int customerId = pickCustomerId(matches);
        if (customerId != 0) {
            return customerId;
        }

    }
}

int ClientHandler::chooseVehicle(int ownerId) {
    while (true) {
        const auto owned = manager.getVehiclesByCustomerId(ownerId);

        if (owned.empty()) {
            std::cout << "This customer has no vehicles. Register a new one.\n";
            return promptNewVehicle(ownerId);
        }

        std::cout << "\nVehicles of this customer:\n";

        for (std::size_t i = 0; i < owned.size(); ++i) {
            std::cout << (i + 1) << ") "
                      << owned[i].getVehicleBrand() << ' '
                      << owned[i].getVehicleModel() << " ["
                      << owned[i].getVehicleRegNum() << ", "
                      << owned[i].getYearOfManufacture() << "]\n";
        }

        std::cout << "n) Register new vehicle\n";
        std::cout << "0) Cancel\n";

        const std::string choice = readLine("Choose: ");

        if (choice == "n" || choice == "N") {
            const int newId = promptNewVehicle(ownerId);
            if (newId != 0) {
                return newId;
            }
            continue;
        }

        if (choice == "0") {
            return 0;
        }

        std::istringstream input(choice);
        int index;

        if (input >> index && input.eof()
            && index >= 1
            && static_cast<std::size_t>(index) <= owned.size()) {
            return owned[index - 1].getVehicleId();
       }

        std::cout << "Invalid choice.\n";

    }

}

void ClientHandler::showBill(int orderId) const {
    const RepairOrder& order = manager.getOrderById(orderId);
    const Customer& customer = manager.findCustomerById(order.getCustomerId());
    const Vehicle& vehicle = manager.findVehicleById(order.getVehicleId());

    std::cout << "\n" << RepairBill(order, customer, vehicle ) << "\n";
}

int ClientHandler::promptNewCustomer() {

    try {
        const std::string name = readLine("Customer name: ");
        const std::string phone = readLine("Phone: ");

        Customer customer(manager.getCustomersNextId(), name, phone);
        manager.addCustomer(customer);

        std::cout << "Customer added with ID: "
                  << customer.getCustomerId() << '\n';

        return customer.getCustomerId();
    }
    catch (const std::exception &e) {
        std::cout <<"Failed to add customer!" <<e.what() << '\n';
        return 0;
    }
}

void ClientHandler::handleEditCustomer() {

        string searchParam = readLine("Search customer's name: ");
        try {
            vector<Customer> foundCustomers = manager.findCustomersByName(searchParam);

            int customerId = pickCustomerId(foundCustomers);
            cout << "(Leave blank to keep current value)\n";
            string newCustomerName = readLine("New customer name: ");
            string newCustomerPhoneNumber = readLine("New customer phone number: ");

            manager.editCustomer(customerId, newCustomerName, newCustomerPhoneNumber);
            cout << "Customer edited successfully!\n";

        }catch (const exception &e) {
            cout <<"Failed to edit customer. "<< e.what() << endl;
        }
    }

void ClientHandler::promptNewService(int orderId) {

    const std::string description = readLine("Service description: ");
    const double hours = readPositiveDouble("Hours: ");
    const double rate = readPositiveDouble("Hourly rate: ");

    std::cout << "Add \"" << description << "\", "
              << hours << " h x " << rate
              << " = " << hours * rate << "? (y/n): ";

    if (readLine("") == "y") {
        manager.addRepairService(orderId, RepairService(description, hours, rate));
        std::cout << "Service added.\n";
    }
}

void ClientHandler::promptNewPart(int orderId) {
    const std::string name = readLine("Part name: ");
    const std::string number = readLine("Part number: ");
    const double price = readPositiveDouble("Unit price: ");
    const int qty = readPositiveInt("Quantity: ");

    std::cout << "Add " << name << " (" << number << "), "
              << qty << " x " << price
              << " = " << qty * price << "? (y/n): ";

    if (readLine("") == "y") {
        manager.addParts(orderId, Part(name, number, price, qty));
        std::cout << "Part added.\n";
    }
}

int ClientHandler::promptNewVehicle(int presetCustomerId ) {
    const int customerId = (presetCustomerId != 0)
            ? presetCustomerId
            : chooseCustomer();

    if (customerId == 0) {
        return 0;
    }

    const std::string brand = readLine("Brand: ");
    const std::string model = readLine("Model: ");
    const std::string regNum = readLine("Registration number: ");
    const int year = readPositiveInt("Year of manufacture: ");

    Vehicle vehicle(
        manager.getVehiclesNextId(), customerId, brand, model, regNum, year
    );

    manager.addVehicle(vehicle);

    std::cout << "Vehicle added with ID: "
              << vehicle.getVehicleId() << '\n';

    return vehicle.getVehicleId();


}



void ClientHandler::handleEditVehicle() {
    const std::string regNum = readLine("Vehicle registration number: ");

    const Vehicle& current = manager.findVehicleByRegNum(regNum);

    std::cout << "\nEditing vehicle: "
              << current.getVehicleBrand() << ' '
              << current.getVehicleModel() << '\n'
              << "Leave a field empty to keep its current value.\n";

    const std::string brand =
        readLine("Brand [" + current.getVehicleBrand() + "]: ");
    const std::string model =
        readLine("Model [" + current.getVehicleModel() + "]: ");
    const std::string newRegNum =
        readLine("Reg. number [" + current.getVehicleRegNum() + "]: ");
    const std::optional<int> year =
        readOptionalInt("Year [" +
            std::to_string(current.getYearOfManufacture()) + "]: ");

    manager.editVehicle(
        current.getVehicleId(),
        brand,
        model,
        newRegNum,
        year
    );

    std::cout << "Vehicle updated.\n";
}

void ClientHandler::handleAcceptVehicle() {
    try {
        const int customerId = chooseCustomer();
        if (customerId == 0) {
            return;
        }

        const int vehicleId = chooseVehicle(customerId);
        if (vehicleId == 0) {
            return;
        }

        const std::string statement =
            readLine("Customer complaint / description: ");

        const int orderId = manager.acceptVehicles(
            customerId, vehicleId, statement
        );

        std::cout << "Vehicle accepted. Repair order ID: "
                  << orderId << '\n';
    }catch (const std::exception &e) {
        std::cout <<"Failed to accept vehicle!" <<e.what() << endl;
    }
}

void ClientHandler::handleOrderActions(int orderId) {

     while (true) {
        const RepairOrder& order = manager.getOrderById(orderId);

        std::cout << "\n=== ORDER #" << orderId
                  << " | " << toString(order.getStatus()) << " ===\n"
                  << "1. Show service card\n";

        switch (order.getStatus()) {
            case RepairStatus::Accepted:
                std::cout << "2. Start repair\n";
                break;

            case RepairStatus::InProgress:
                std::cout << "2. Add service\n"
                          << "3. Add part\n"
                          << "4. Complete repair and issue bill\n";
                break;

            case RepairStatus::Completed:
                std::cout << "2. Show bill\n"
                          << "3. Register payment\n";
                break;

            case RepairStatus::Paid:
                std::cout << "2. Show bill\n";
                break;
        }

        std::cout << "0. Back\n";

        const int choice = readInt("Choose: ");
        if (choice == 0) {
            return;
        }

        try {
            if (choice == 1) {
                std::cout << order;

            } else if (order.getStatus() == RepairStatus::Accepted
                       && choice == 2) {
                manager.startRepair(orderId);
                std::cout << "Repair started.\n";

            } else if (order.getStatus() == RepairStatus::InProgress) {
                if (choice == 2) {
                    promptNewService(orderId);
                } else if (choice == 3) {
                    promptNewPart(orderId);
                } else if (choice == 4) {
                    manager.completeRepair(orderId);
                    showBill(orderId);
                } else {
                    std::cout << "Unknown option.\n";
                }

            } else if (order.getStatus() == RepairStatus::Completed) {
                if (choice == 2) {
                    showBill(orderId);
                } else if (choice == 3) {
                    manager.payRepair(orderId);
                    std::cout << "Payment registered.\n";
                } else {
                    std::cout << "Unknown option.\n";
                }

            } else if (order.getStatus() == RepairStatus::Paid) {
                if (choice == 2) {
                    showBill(orderId);
                } else {
                    std::cout << "Unknown option.\n";
                }
            }
        } catch (const std::exception& error) {
            std::cerr << "Operation failed: " << error.what() << '\n';
        }
    }
}

void ClientHandler::handleFindByRegNum() {
    try {
        const std::string regNum = readLine("Registration number: ");

        const Vehicle& vehicle = manager.findVehicleByRegNum(regNum);

        std::cout << "\nVehicle: "
                  << vehicle.getVehicleBrand() << ' '
                  << vehicle.getVehicleModel() << " ["
                  << vehicle.getVehicleRegNum() << ", "
                  << vehicle.getYearOfManufacture() << "]\n";

        const RepairOrder* active = manager.findActiveRepairByRegNum(regNum);

        if (active == nullptr) {
            std::cout << "This vehicle has no active repair order.\n";
            return;
        }


        handleOrderActions(active->getOrderId());
    }catch (const std::exception& error) {
        std::cerr << "Operation failed: " << error.what() << '\n';
    }
}

void ClientHandler::handleUnfinishedRepairs() {
    const vector<RepairOrder> unfinished = manager.listUnfinishedRepairs();

    if (unfinished.empty()) {
        cout<< "\nNo unfinished repairs found.\n";
        return;
    }

    cout<< "\n=========== UNFINISHED REPAIRS ===========\n";

    for (const RepairOrder order : unfinished) {
        const Vehicle& vehicle = manager.findVehicleById(order.getVehicleId());

        cout << "  [" << order.getOrderId() << "] "
                  << vehicle.getVehicleRegNum() << " | "
                  << vehicle.getVehicleBrand() << ' '
                  << vehicle.getVehicleModel()
                  << " | " << toString(order.getStatus())
                  << " | accepted: " << formatDate(order.getAcceptedDate())
                  << '\n';
    }

    cout<< "\nEnter order ID to work on it, or 0 to go back.\n";
    const int choiceId = readInt("Chose: ");

    if (choiceId == 0) {
        return;
    }

    bool found = false;
    for (const RepairOrder order : unfinished) {
        if (order.getOrderId() == choiceId) {
            found = true;
            break;
        }
    }
    if (!found) {
        cout<< "No unfinished repair with ID "<< choiceId<<endl;
        return;
    }

    handleOrderActions(choiceId);
}

void ClientHandler::handleTurnover() {
    cout << "\n=========== TURNOVER FOR PERIOD ===========\n";

    const string fromText = readLine("From date (DD.MM.YYYY): ");

    const string toText = readLine("To date (DD.MM.YYYY): ");

    try {
        const tm fromDate = parseCalendarDate(fromText);
        const tm toDate = parseCalendarDate(toText);

        const auto from = toTimePoint(fromDate);
        const auto lastDay = toTimePoint(toDate);

        if (from> lastDay) {
            cout<< "The start date cannot be after the end date."<< endl;
            return;
        }

        const auto toExclusive = toTimePoint(toDate,true);
        const double turnover = manager.calculateTurnover(from, toExclusive);

        const auto oldFlags = cout.flags();
        const auto oldPrecision = cout.precision();

        cout << "\nPeriod: " << fromText
                  << " - " << toText
                  << "\nTotal turnover: "
                  << std::fixed << std::setprecision(2)
                  << turnover << '\n';

        cout.flags(oldFlags);
        cout.precision(oldPrecision);
    }catch (const std::exception& error) {
        cerr << "Cannot generate report: "
        << error.what() << '\n';
    }
}

void ClientHandler::run() {

    while (true) {
        std::cout
            << "\n"
            << "========================================\n"
            << "           AUTO WORKSHOP MENU           \n"
            << "========================================\n"
            << "1. Accept vehicle for repair\n"
            << "2. Search by registration number\n"
            << "3. Edit customer\n"
            << "4. Edit vehicle\n"
            << "5. List unfinished repairs\n"
            << "6. Turnover for period\n"
            << "0. Exit\n"
            << "========================================\n";

        try {
            const int choice = readInt("Choose: ");

            switch (choice) {
                case 1:
                    handleAcceptVehicle();
                    break;

                case 2:
                    handleFindByRegNum();
                    break;

                case 3:
                    handleEditCustomer();
                    break;

                case 4:
                    handleEditVehicle();
                    break;

                case 5:
                    handleUnfinishedRepairs();
                    break;

                case 6:
                    handleTurnover();
                    break;

                case 0:
                    std::cout << "Goodbye!\n";
                    return;

                default:
                    std::cout
                        << "Unknown option. Choose a number from 0 to 6.\n";
                    break;
            }
        } catch (const std::exception& error) {
            if (std::cin.eof() || std::cin.bad()) {
                std::cerr << "\nInput closed. Exiting.\n";
                return;
            }

            std::cerr << "\nOperation failed: "
                      << error.what() << '\n';
        }
    }
}
