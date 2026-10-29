# GarageService

A C++ console application for auto workshop management — handling customers, vehicles, service repair orders, and financial reports.

## Features

- Add and edit customers and vehicles.
- Accept a vehicle for repair and create a service repair order.
- Search by registration number with direct access to repair actions.
- Add performed services and spare parts used during repairs.
- Complete repairs, generate bills, and register payments.
- List unfinished repairs and select an order to proceed.
- Calculate total turnover for a selected period.
- Persistent storage and data loading using text files.

## Order Lifecycle

During the standard workflow, a repair order transitions through the following states:

`Accepted → InProgress → Completed → Paid`

Unfinished repairs consist of orders with either the `Accepted` or `InProgress` status.

Turnover is calculated from paid orders based on their payment date. The period is entered in `DD.MM.YYYY` format and is inclusive of the end date.

## Project Structure

- `Models/` — core domain objects: customers, vehicles, repair orders, services, parts, and bills.
- `Repositories/` — in-memory data storage and lookup mechanisms.
- `ConsoleUI/` — modular user interface handlers separated by domain (Customer, Vehicle, Repair Order).
- `Services/` — business logic layer (`AutoWorkshopManager`), data loaders, and file I/O operations (`FileManager`).
- `Utility/` — helper utilities for console input validation and date/time manipulation.
- `main.cpp` — dependency injection and application entry point.

## Compilation

Requires CMake and a compiler supporting **C++17** or newer.

From the root directory of the project:

    cmake -S . -B build
    cmake --build build

Run the generated executable or use the Run configuration in CLion.

## Usage

The main menu provides the following commands:

1. Accept vehicle for repair.
2. Search by registration number.
3. Edit customer.
4. Edit vehicle.
5. List unfinished repairs.
6. Calculate turnover for a period.
0. Exit.

Selecting or finding an active repair order opens a dynamic submenu tailored to the order's current status.

## Data Persistence

`FileManager` handles saving and loading customers, vehicles, and repair orders using atomic file operations (via temporary files). Modifying operations within the manager automatically trigger data persistence.

File paths (`CUSTOMERS_FILE`, `VEHICLES_FILE`, `REPAIR_ORDERS_FILE`) are resolved relative to the application's working directory.

## Author

Kolio
