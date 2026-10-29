//
// Created by Kolio on 10/29/2026.
//

#include "CustomerUIHandler.h"
#include "../Utility/ConsoleInput.h"
#include <iostream>
int CustomerUIHandler::promptNewCustomer() {
    try {
        const std::string name = ConsoleInput::readLine("Customer name: ");
        const std::string phone = ConsoleInput::readLine("Phone: ");

        Customer customer(manager.getCustomersNextId(), name, phone);
        manager.addCustomer(customer);

        std::cout << "Customer added with ID: "
                  << customer.getCustomerId() << '\n';

        return customer.getCustomerId();
    } catch (const std::exception &e) {
        std::cout << "Failed to add customer! " << e.what() << '\n';
        return 0;
    }
}

void CustomerUIHandler::handleEditCustomer() {
    std::string searchParam = ConsoleInput::readLine("Search customer's name: ");
    try {
        std::vector<Customer> foundCustomers = manager.findCustomersByName(searchParam);

        int customerId = pickCustomerId(foundCustomers);
        if (customerId == 0) {
            return; // Прекратяваме, ако не е избран клиент
        }

        std::cout << "(Leave blank to keep current value)\n";
        std::string newCustomerName = ConsoleInput::readLine("New customer name: ");
        std::string newCustomerPhoneNumber = ConsoleInput::readLine("New customer phone number: ");

        manager.editCustomer(customerId, newCustomerName, newCustomerPhoneNumber);
        std::cout << "Customer edited successfully!\n";

    } catch (const std::exception &e) {
        std::cout << "Failed to edit customer. " << e.what() << std::endl;
    }
}

int CustomerUIHandler::chooseCustomer() {
    while (true) {
        const std::string namePart =
          ConsoleInput::readLine("Search customer by name (or 'n' for new): ");

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
            std::cout << "No match. Press Enter to search again or 'n' to create new customer.\n";
            continue;
        }

        const int customerId = pickCustomerId(matches);
        if (customerId != 0) {
            return customerId;
        }
    }
}

int CustomerUIHandler::pickCustomerId(const std::vector<Customer> &matches) const {
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


    const int choice = ConsoleInput::readInt("Choose [1-" +
        std::to_string(matches.size()) + "] or 0 to cancel: ");

    if (choice == 0) {
        return 0;
    }

    if (choice < 0 || static_cast<std::size_t>(choice) > matches.size()) {
        std::cout << "Invalid choice.\n";
        return 0;
    }

    return matches[choice - 1].getCustomerId();
}
