cmake_minimum_required(VERSION 4.3)
project(GarageService)

set(CMAKE_CXX_STANDARD 26)

add_executable(GarageService main.cpp
        Models/Customer.cpp
        Models/Customer.h
        Models/Vehicle.cpp
        Models/Vehicle.h
)
