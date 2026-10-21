//
// Created by Kolio on 10/21/2026.
//

#include "ClientHandler.h"

#include <iostream>

namespace {
    class InputClosed final : public std::exception {
        public:
        const char* what() const noexcept override {
            return "Console input is closed.";
        }
    };
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
