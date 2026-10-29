//
// Created by Kolio on 10/17/2026.
//

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

RepairOrder::RepairOrder() {
    this->orderId = 0;
    this->customerId = 0;
    this->vehicleId = 0;
    this->customerStatement = "";
    this->repairServices={};
    this->parts = {};
    this->acceptedDate = std::chrono::system_clock::now();
    this->paidDate = std::nullopt;
    this->completedDate = std::nullopt;

}

RepairOrder::RepairOrder(int orderId, int customerId, int vehicleId, const std::string &customerStatement) {
    setOrderId(orderId);
    setCustomerId(customerId);
    setVehicleId(vehicleId);
    setCustomerStatement(customerStatement);
    acceptedDate = std::chrono::system_clock::now();
}

RepairOrder::RepairOrder(int orderId, int customerId, int vehicleId, const std::string &customerStatement,
    const std::vector<RepairService> &repairServices, const std::vector<Part> &parts, RepairStatus repairStatus,
    TimePoint acceptedDate, std::optional<TimePoint> completedDate, std::optional<TimePoint> paidDate) {
    setOrderId(orderId);
    setCustomerId(customerId);
    setVehicleId(vehicleId);
    setCustomerStatement(customerStatement);

    this->repairServices = repairServices;
    this->parts = parts;
    this->repairStatus = repairStatus;
    this->acceptedDate = acceptedDate;
    this->completedDate = completedDate;
    this->paidDate = paidDate;

    validateState();
}

void RepairOrder::setOrderId(int order_id) {
    if (order_id <= 0 ) {
        throw std::logic_error("OrderId cannot be negative!");
    }
    this->orderId = order_id;
}

void RepairOrder::setCustomerId(int customer_id) {
    if (customer_id <= 0) {
        throw std::logic_error("CustomerId cannot be negative!");
    }
    this->customerId = customer_id;
}

void RepairOrder::setVehicleId(int vehicle_id) {
    if ( vehicle_id <= 0 ) {
        throw std::logic_error("VehicleId cannot be negative!");
    }
    this->vehicleId = vehicle_id;
}

void RepairOrder::setCustomerStatement(const std::string& customer_statement) {
    if (customer_statement.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::logic_error("Customer statement cannot be empty!");
    }
    this->customerStatement = customer_statement;
}

void RepairOrder::setStatus(const RepairStatus &repair_status) {
    if (repairStatus == RepairStatus::Completed && repair_status == RepairStatus::Accepted) {
        throw std::invalid_argument("Cannot change status from Completed back to Accepted!");
    }

    if (repairStatus == RepairStatus::Paid && repair_status != RepairStatus::Paid) {
        throw std::invalid_argument("Cannot change status of a paid order!");
    }
    this->repairStatus = repair_status;
}

RepairStatus RepairOrder::getStatus() const {
    return repairStatus;
}

void RepairOrder::startRepair() {
    if (repairStatus != RepairStatus::Accepted) {
        throw std::logic_error("Only accepted repairs can be started!");
    }
    this->repairStatus = RepairStatus::InProgress;
}

void RepairOrder::completeRepair() {
    if (repairStatus != RepairStatus::InProgress) {
        throw std::logic_error("Only an in-progres repair can be completed!");
    }
    repairStatus = RepairStatus::Completed;
    completedDate = std::chrono::system_clock::now();
}

void RepairOrder::payRepair() {
    if (repairStatus != RepairStatus::Completed) {
        throw std::logic_error("Only completed repair can be marked as paid!");
    }
    repairStatus = RepairStatus::Paid;
    paidDate = std::chrono::system_clock::now();
}

void RepairOrder::addRepairService(const RepairService &service) {
    ensureEditable();
    repairServices.push_back(service);
}

void RepairOrder::addPart(const Part &part) {
    ensureEditable();
    parts.push_back(part);
}

double RepairOrder::calculateLaborCost() const {
    double total = 0.0;

    for (const RepairService &repairService : repairServices) {
        total += repairService.calculateCost();
    }
    return total;
}

double RepairOrder::calculatePartsCost() const {
    double total = 0.0;

    for ( const Part &part : parts) {
        total += part.calculateCost();
    }
    return total;

}

double RepairOrder::calculateTotalCost() const {
    return calculatePartsCost()+ calculateLaborCost();
}


void RepairOrder::validateState() const {
    switch (repairStatus) {
        case RepairStatus::Accepted:
        case RepairStatus::InProgress:
            if (completedDate.has_value() || paidDate.has_value()) {
                throw std::logic_error("An unfinished repair cannot have completion or payment date");
            }
            break;
            case RepairStatus::Completed:
            if (!completedDate.has_value() || paidDate.has_value()) {
                throw std::invalid_argument("A completed repair must have completion and no payment date");
            }
            break;
            case RepairStatus::Paid:
            if (!completedDate.has_value() || !paidDate.has_value()) {
                throw std::invalid_argument("A paid repair must have completion and payment date");
            }
            break;
            default:
            throw std::logic_error("Invalid repair status!");
    }

    if (completedDate.has_value() && *completedDate< acceptedDate) {
        throw std::invalid_argument("Completion date cannot be less than accepted date!");
    }

    if (paidDate.has_value() && *paidDate< *completedDate) {
        throw std::invalid_argument("Paid date cannot be less than completed date!");
    }
}

void RepairOrder::ensureEditable() const {
    if (repairStatus != RepairStatus::Accepted && repairStatus != RepairStatus::InProgress) {
        throw std::logic_error("Complete or paid orders cannot be modified!");
    }
}

std::ostream & operator<<(std::ostream &out, const RepairOrder &order) {
    out << "================ REPAIR ORDER CARD ================\n"
       << "Order ID:           " << order.orderId << '\n'
       << "Customer ID:        " << order.customerId << '\n'
       << "Vehicle ID:         " << order.vehicleId << '\n'
       << "Status:             " << toString(order.repairStatus) << '\n'
       << "Customer statement: " << order.customerStatement << '\n'
       << "Accepted on:        " << formatDate(order.acceptedDate) << '\n';

    if (order.completedDate.has_value()) {
        out << "Completed on:       "
            << formatDate(order.completedDate.value()) << '\n';
    }
    if (order.paidDate.has_value()) {
        out << "Paid on:            "
            << formatDate(order.paidDate.value()) << '\n';
    }

    out << "\n------------- Agreed services -------------\n";
    if (order.repairServices.empty()) {
        out << "No services registered yet.\n";
    } else {
        for (const RepairService& service : order.repairServices) {
            out << "| " << service << '\n';
        }
    }

    out << "\n-------------- Installed parts -------------\n";
    if (order.parts.empty()) {
        out << "No parts registered yet.\n";
    } else {
        for (const Part& part : order.parts) {
            out << "- " << part.getPartName()
                << " (" << part.getPartNumber() << "): "
                << part.getQuantity() << " x " << part.getUnitPrice()
                << " = " << part.getQuantity() * part.getUnitPrice() << '\n';
        }
    }

    if (order.repairStatus == RepairStatus::Completed
        || order.repairStatus == RepairStatus::Paid) {
        out << "\nTotal due:          " << order.calculateTotalCost() << '\n';
        }

    return out;
}
