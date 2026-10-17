//
// Created by Kolio on 10/17/2026.
//

#include "RepairOrder.h"



RepairOrder::RepairOrder() {
    this->orderId = 0;
    this->customerId = 0;
    this->vehicleId = 0;
    this->customerStatement = "";
    this->repairServices={};
    this->parts = {};
    this->acceptedDate = {};
    this->paidDate = std::nullopt;
    this->completedDate = std::nullopt;

}

RepairOrder::RepairOrder(int orderId, int customerId, int vehicleId, const std::string &customerStatement) {
    setOrderId(orderId);
    setCustomerId(customerId);
    setVehicleId(vehicleId);
    setCustomerStatement(customerStatement);
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
    if (order_id < 0 ) {
        throw std::logic_error("OrderId cannot be negative!");
    }
    this->orderId = order_id;
}

void RepairOrder::setCustomerId(int customer_id) {
    if (customer_id < 0) {
        throw std::logic_error("CustomerId cannot be negative!");
    }
    this->customerId = customer_id;
}

void RepairOrder::setVehicleId(int vehicle_id) {
    if ( vehicle_id < 0 ) {
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

RepairStatus RepairOrder::getStatus() const {
    return repairStatus;
}

void RepairOrder::startRepair() {
    repairStatus = RepairStatus::InProgress;
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
                throw std::invalid_argument("A completed repair must have completion and payment date");
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