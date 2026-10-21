//
// Created by Kolio on 10/21/2026.
//

#include "RepairBill.h"

#include <iomanip>

#include "Part.h"
#include "RepairService.h"
#include "Vehicle.h"
#include "Customer.h"
#include "RepairOrder.h"

namespace {
    using TimePoint = std::chrono::system_clock::time_point;
    std::string formatDate(const std::optional<TimePoint>& date) {
        if (!date.has_value()) {
            return "-";
        }

        const std::time_t rawTime =
            std::chrono::system_clock::to_time_t(*date);

        const std::tm* localTime = std::localtime(&rawTime);

        if (localTime == nullptr) {
            return "-";
        }

        const std::tm calendarDate = *localTime;

        std::ostringstream result;
        result << std::put_time(&calendarDate, "%Y-%m-%d");
        return result.str();
    }
}

const RepairOrder& RepairBill::getOrder() const noexcept {
    return order;
}

const Customer& RepairBill::getCustomer() const noexcept {
    return customer;
}

const Vehicle& RepairBill::getVehicle() const noexcept {
    return vehicle;
}

std::ostream& operator<<(std::ostream& os, const RepairBill& bill) {
    const auto flags = os.flags();
    const auto precision = os.precision();

    os << std::fixed << std::setprecision(2);

    os << "=========== BILL ===========\n"
       << "Order ID: " << bill.getOrder().getOrderId() << '\n'
       << "Customer: " << bill.getCustomer().getName()
       << " (" << bill.getCustomer().getPhoneNumber() << ")\n"
       << "Vehicle:  " << bill.getVehicle().getVehicleBrand() << ' '
       << bill.getVehicle().getVehicleModel()
       << " [" << bill.getVehicle().getVehicleRegNum() << "]\n"
       << "Completed: " << formatDate(bill.getOrder().getCompleteDate()) << '\n';

    os << "\nServices\n";
    os << std::setw(4) << "ID"
       << std::setw(35) << "Description"
       << std::setw(10) << "Hours"
       << std::setw(12) << "Rate"
       << std::setw(12) << "Total" << '\n';

    int serviceIndex = 1;
    for (const RepairService& s : bill.getOrder().getServices()) {
        const double total = s.calculateCost();
        os << std::setw(4) << serviceIndex++
           << std::setw(35) << s.getDerscription()
           << std::setw(10) << s.getHoursLabor()
           << std::setw(12) << s.getHourlyRate()
           << std::setw(12) << total << '\n';
    }

    os << "\nParts\n";
    os << std::setw(4) << "ID"
       << std::setw(25) << "Name"
       << std::setw(15) << "Number"
       << std::setw(6) << "Qty"
       << std::setw(12) << "Price"
       << std::setw(12) << "Total" << '\n';

    int partIndex = 1;
    for (const Part& p : bill.getOrder().getParts()) {
        const double total = p.getUnitPrice() * p.getQuantity();
        os << std::setw(4) << partIndex++
           << std::setw(25) << p.getPartName()
           << std::setw(15) << p.getPartNumber()
           << std::setw(6) << p.getQuantity()
           << std::setw(12) << p.getUnitPrice()
           << std::setw(12) << total << '\n';
    }

    os << "\n";
    os << "Subtotal services: " << bill.getOrder().calculateLaborCost() << '\n';
    os << "Subtotal parts:    " << bill.getOrder().calculatePartsCost() << '\n';
    os << "TOTAL:             " << bill.getOrder().calculateTotalCost() << '\n';

    os.flags(flags);
    os.precision(precision);
    return os;
}