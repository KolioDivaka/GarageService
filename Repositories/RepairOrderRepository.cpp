//
// Created by Kolio on 10/21/2026.
//

#include "RepairOrderRepository.h"
#include "../Models/RepairOrder.h"
#include <stdexcept>

void RepairOrderRepository::updateNextId() {
    int maxRepairOrderId = 0;

    for (const RepairOrder& order : repairOrders) {
        maxRepairOrderId = max(maxRepairOrderId, order.getOrderId());
    }

    constexpr int maxInt = numeric_limits<int>::max();

    if (maxRepairOrderId == maxInt) {
        throw overflow_error("Cannot generate repair order IDs: int limit reached!");
    }

    nextRepairOrderId = maxRepairOrderId + 1;
}

void RepairOrderRepository::addOrder(const RepairOrder& order) {
    for (const auto& existingOrder : repairOrders) {
        if (existingOrder.getOrderId() == order.getOrderId()) {
            throw invalid_argument("Repair order ID already exists: " + to_string(order.getOrderId()));
        }
    }

    repairOrders.push_back(order);
    updateNextId();

}

RepairOrder & RepairOrderRepository::findOrderById(int orderId) {

    for (RepairOrder& order : repairOrders) {
        if (order.getOrderId() == orderId) {
            return order;
        }
    }
    throw invalid_argument("Cannot find an order with ID: " + to_string(orderId));
}

const RepairOrder& RepairOrderRepository::findOrderById(int orderId) const {
    for (const RepairOrder& order : repairOrders) {
        if (order.getOrderId() == orderId) {
            return order;
        }
    }
    throw invalid_argument("Cannot find an order with ID: " + to_string(orderId));
}

const RepairOrder * RepairOrderRepository::findActiveRepairByVehicleId(int vehicleId) const {
    for (const RepairOrder& order : repairOrders) {
        if (order.getVehicleId() == vehicleId &&
            (order.getStatus() == RepairStatus::Accepted || order.getStatus() == RepairStatus::InProgress)) {
            return &order;
            }
    }
    return nullptr;
}

vector<RepairOrder> RepairOrderRepository::getUnfinishedRepairs() const {
    vector<RepairOrder> unfinishedRepairs;
    for (const RepairOrder& order : repairOrders) {
        if (order.getStatus() == RepairStatus::Accepted || order.getStatus() == RepairStatus::InProgress) {
            unfinishedRepairs.push_back(order);
        }
    }
    return unfinishedRepairs;
}

const vector<RepairOrder> & RepairOrderRepository::getRepairOrders() const {
    return repairOrders;
}

void RepairOrderRepository::setRepairOrders(const vector<RepairOrder> &orders) {
    repairOrders = orders;
    updateNextId();
}

int RepairOrderRepository::getNextRepairOrderId() const {
    return nextRepairOrderId;
}

