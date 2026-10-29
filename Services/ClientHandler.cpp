//
// Created by Kolio on 10/21/2026.
//

#include "ClientHandler.h"
#include "../ConsoleUI/CustomerUIHandler.h"
#include "../ConsoleUI/VehicleUIHandler.h"
#include "../ConsoleUI/RepairOrderUIHandler.h"
#include "../Utility/ConsoleInput.h"
#include <iostream>
using namespace  ConsoleInput;

void ClientHandler::run() {
CustomerUIHandler customerUI(*manager);
    VehicleUIHandler vehicleUI(*manager);
    RepairOrderUIHandler orderUI(*manager); // или RepairOrderUIHandler

    while (true) {
        std::cout << "\n===== GARAGE SERVICE MENU =====\n"
                  << "1. Add new customer\n"
                  << "2. Edit customer\n"
                  << "3. Register new vehicle\n"
                  << "4. Edit vehicle\n"
                  << "5. Accept vehicle for repair\n"
                  << "6. Find vehicle by registration number\n"
                  << "7. List unfinished repairs\n"
                  << "8. Calculate turnover for period\n"
                  << "0. Exit\n";

        try {
            const int choice = ConsoleInput::readInt("Choose option: ");

            switch (choice) {
                case 1:
                    customerUI.promptNewCustomer();
                    break;
                case 2:
                    customerUI.handleEditCustomer();
                    break;
                case 3:
                    vehicleUI.promptNewVehicle();
                    break;
                case 4:
                    vehicleUI.handleEditVehicle();
                    break;
                case 5:
                    orderUI.handleAcceptVehicle();
                    break;
                case 6:
                    vehicleUI.handleFindByRegNum();
                    break;
                case 7:
                    orderUI.handleUnfinishedRepairs();
                    break;
                case 8:
                    orderUI.handleTurnover();
                    break;
                case 0:
                    std::cout << "Exiting application. Goodbye!\n";
                    return;
                default:
                    std::cout << "Invalid option. Try again.\n";
            }
        } catch (const InputClosed&) {
            std::cout << "\nInput stream closed. Exiting...\n";
            return;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
}
