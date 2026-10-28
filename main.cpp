#include <iostream>
#include <exception>


#include "Services/AutoWorkshopManager.h"
#include "Services/ClientHandler.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    try {
        FileManager fileManager;

        CustomerRepository customerRepo;
        VehicleRepository vehicleRepo;
        RepairOrderRepository orderRepo;

        AutoWorkshopManager manager(
            fileManager,
            customerRepo,
            vehicleRepo,
            orderRepo
        );

        ClientHandler handler(manager);
        handler.run();

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Application error: "
                  << error.what() << '\n';

        return 1;
    }

}