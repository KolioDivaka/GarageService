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
)
