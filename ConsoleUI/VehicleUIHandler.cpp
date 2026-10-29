//
// Created by Kolio on 10/29/2026.
//

#include "VehicleUIHandler.h"
#include "CustomerUIHandler.h"
// Ще създадем този файл в следващата стъпка:
#include "RepairOrderUIHandler.h"
#include "../Utility/ConsoleInput.h"
#include <iostream>
#include <sstream>
int VehicleUIHandler::promptNewVehicle(int presetCustomerId) {
    const int customerId = (presetCustomerId != 0)
            ? presetCustomerId
            // Използваме вече готовия CustomerUIHandler
            : CustomerUIHandler(manager).chooseCustomer();

    if (customerId == 0) {
        return 0;
    }

    const std::string brand = ConsoleInput::readLine("Brand: ");
    const std::string model = ConsoleInput::readLine("Model: ");
    const std::string regNum = ConsoleInput::readLine("Registration number: ");
    const int year = ConsoleInput::readPositiveInt("Year of manufacture: ");

    Vehicle vehicle(
        manager.getVehiclesNextId(), customerId, brand, model, regNum, year
    );

    manager.addVehicle(vehicle);

    std::cout << "Vehicle added with ID: "
              << vehicle.getVehicleId() << '\n';

    return vehicle.getVehicleId();
}

void VehicleUIHandler::handleEditVehicle() {
    const std::string regNum = ConsoleInput::readLine("Vehicle registration number: ");

    const Vehicle& current = manager.findVehicleByRegNum(regNum);

    std::cout << "\nEditing vehicle: "
              << current.getVehicleBrand() << ' '
              << current.getVehicleModel() << '\n'
              << "Leave a field empty to keep its current value.\n";

    const std::string brand =
        ConsoleInput::readLine("Brand [" + current.getVehicleBrand() + "]: ");
    const std::string model =
        ConsoleInput::readLine("Model [" + current.getVehicleModel() + "]: ");
    const std::string newRegNum =
        ConsoleInput::readLine("Reg. number [" + current.getVehicleRegNum() + "]: ");
    const std::optional<int> year =
        ConsoleInput::readOptionalInt("Year [" +
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

int VehicleUIHandler::chooseVehicle(int ownerId) {
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

        const std::string choice = ConsoleInput::readLine("Choose: ");

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

void VehicleUIHandler::handleFindByRegNum() {
    try {
        const std::string regNum = ConsoleInput::readLine("Registration number: ");

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

        // Прехвърляме управлението към UI класа за поръчките
        RepairOrderUIHandler(manager).handleOrderActions(active->getOrderId());

    } catch (const std::exception& error) {
        std::cerr << "Operation failed: " << error.what() << '\n';
    }
}