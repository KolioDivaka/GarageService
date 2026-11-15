cmake_minimum_required(VERSION 4.3)
project(GarageService)

set(CMAKE_CXX_STANDARD 26)

add_executable(GarageService main.cpp
        Models/Customer.cpp
        Models/Customer.h
        Models/Vehicle.cpp
        Models/Vehicle.h
        Models/RepairOrder.cpp
        Models/RepairOrder.h
        Models/RepairService.cpp
        Models/RepairService.h
        Models/Part.cpp
        Models/Part.h
        Models/RepairStatus.h
        Services/AutoWorkshopManager.cpp
        Services/AutoWorkshopManager.h
        Services/FileManager.cpp
        Services/FileManager.h
        Repositories/CustomerRepository.cpp
        Repositories/CustomerRepository.h
        Repositories/VehicleRepository.cpp
        Repositories/VehicleRepository.h
        Repositories/RepairOrderRepository.cpp
        Repositories/RepairOrderRepository.h
        Services/DataLoader.cpp
        Services/DataLoader.h
        Services/ClientHandler.cpp
        Services/ClientHandler.h
        Models/RepairBill.cpp
        Models/RepairBill.h
        Utility/ConsoleInput.cpp
        Utility/ConsoleInput.h
        Utility/DateUtils.cpp
        Utility/DateUtils.h
        ConsoleUI/CustomerUIHandler.cpp
        ConsoleUI/CustomerUIHandler.h
        ConsoleUI/VehicleUIHandler.cpp
        ConsoleUI/VehicleUIHandler.h
        ConsoleUI/RepairOrderUIHandler.cpp
        ConsoleUI/RepairOrderUIHandler.h

 "Utility/StringUtils.h" "Utility/StringUtils.cpp")
