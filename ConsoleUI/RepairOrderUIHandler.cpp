//
// Created by Kolio on 10/29/2026.
//
#include "RepairOrderUIHandler.h"
#include "CustomerUIHandler.h"
#include "VehicleUIHandler.h"
#include "../Utility/ConsoleInput.h"
#include "../utility/DateUtils.h"
#include <iostream>
#include <iomanip>
#include "../Models/RepairBill.h"

void RepairOrderUIHandler::handleAcceptVehicle() {
    try {
        const int customerId = CustomerUIHandler(manager).chooseCustomer();
        if (customerId == 0) {
            return;
        }

        const int vehicleId = VehicleUIHandler(manager).chooseVehicle(customerId);
        if (vehicleId == 0) {
            return;
        }

        const std::string statement =
            ConsoleInput::readLine("Customer complaint / description: ");

        const int orderId = manager.acceptVehicles(
            customerId, vehicleId, statement
        );

        std::cout << "Vehicle accepted. Repair order ID: "
                  << orderId << '\n';
    } catch (const std::exception &e) {
        std::cout << "Failed to accept vehicle! " << e.what() << std::endl;
    }
}

void RepairOrderUIHandler::handleOrderActions(int orderId) {
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

        const int choice = ConsoleInput::readInt("Choose: ");
        if (choice == 0) {
            return;
        }

        try {
            if (choice == 1) {
                // Използва пренаписания operator<< на RepairOrder
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

void RepairOrderUIHandler::handleUnfinishedRepairs() {
    const std::vector<RepairOrder> unfinished = manager.listUnfinishedRepairs();

    if (unfinished.empty()) {
        std::cout << "\nNo unfinished repairs found.\n";
        return;
    }

    std::cout << "\n=========== UNFINISHED REPAIRS ===========\n";

    for (const auto& order : unfinished) {
        const Vehicle& vehicle = manager.findVehicleById(order.getVehicleId());

        std::cout << "  [" << order.getOrderId() << "] "
                  << vehicle.getVehicleRegNum() << " | "
                  << vehicle.getVehicleBrand() << ' '
                  << vehicle.getVehicleModel()
                  << " | " << toString(order.getStatus())
                  << " | accepted: " << DateUtils::formatDate(order.getAcceptedDate())
                  << '\n';
    }

    std::cout << "\nEnter order ID to work on it, or 0 to go back.\n";
    const int choiceId = ConsoleInput::readInt("Choose: ");

    if (choiceId == 0) {
        return;
    }

    bool found = false;
    for (const auto& order : unfinished) {
        if (order.getOrderId() == choiceId) {
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "No unfinished repair with ID " << choiceId << std::endl;
        return;
    }

    handleOrderActions(choiceId);
}

void RepairOrderUIHandler::handleTurnover() {
    std::cout << "\n=========== TURNOVER FOR PERIOD ===========\n";

    const std::string fromText = ConsoleInput::readLine("From date (DD.MM.YYYY): ");
    const std::string toText = ConsoleInput::readLine("To date (DD.MM.YYYY): ");

    try {
        const std::tm fromDate = DateUtils::parseCalendarDate(fromText);
        const std::tm toDate = DateUtils::parseCalendarDate(toText);

        const auto from = DateUtils::toTimePoint(fromDate);
        const auto lastDay = DateUtils::toTimePoint(toDate);

        if (from > lastDay) {
            std::cout << "The start date cannot be after the end date." << std::endl;
            return;
        }

        const auto toExclusive = DateUtils::toTimePoint(toDate, true);
        const double turnover = manager.calculateTurnover(from, toExclusive);

        const auto oldFlags = std::cout.flags();
        const auto oldPrecision = std::cout.precision();

        std::cout << "\nPeriod: " << fromText
                  << " - " << toText
                  << "\nTotal turnover: "
                  << std::fixed << std::setprecision(2)
                  << turnover << '\n';

        std::cout.flags(oldFlags);
        std::cout.precision(oldPrecision);
    } catch (const std::exception& error) {
        std::cerr << "Cannot generate report: " << error.what() << '\n';
    }
}

void RepairOrderUIHandler::showBill(int orderId) const {
    const RepairOrder& order = manager.getOrderById(orderId);
    const Customer& customer = manager.findCustomerById(order.getCustomerId());
    const Vehicle& vehicle = manager.findVehicleById(order.getVehicleId());

    // Използва пренаписания operator<< на RepairBill
    std::cout << "\n" << RepairBill(order, customer, vehicle) << "\n";
}

void RepairOrderUIHandler::promptNewService(int orderId) {
    const std::string description = ConsoleInput::readLine("Service description: ");
    const double hours = ConsoleInput::readPositiveDouble("Hours: ");
    const double rate = ConsoleInput::readPositiveDouble("Hourly rate: ");

    std::cout << "Add \"" << description << "\", "
              << hours << " h x " << rate
              << " = " << hours * rate << "? (y/n): ";

    if (ConsoleInput::readLine("") == "y") {
        manager.addRepairService(orderId, RepairService(description, hours, rate));
        std::cout << "Service added.\n";
    }
}

void RepairOrderUIHandler::promptNewPart(int orderId) {
    const std::string name = ConsoleInput::readLine("Part name: ");
    const std::string number = ConsoleInput::readLine("Part number: ");
    const double price = ConsoleInput::readPositiveDouble("Unit price: ");
    const int qty = ConsoleInput::readPositiveInt("Quantity: ");

    std::cout << "Add " << name << " (" << number << "), "
              << qty << " x " << price
              << " = " << qty * price << "? (y/n): ";

    if (ConsoleInput::readLine("") == "y") {
        manager.addParts(orderId, Part(name, number, price, qty));
        std::cout << "Part added.\n";
    }
}