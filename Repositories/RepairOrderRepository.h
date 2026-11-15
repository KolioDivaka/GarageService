//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_REPAIRORDERREPOSITORY_H
#define GARAGESERVICE_REPAIRORDERREPOSITORY_H
#include <vector>
#include "../Models/RepairOrder.h"




class RepairOrderRepository {

private:
    std::vector<RepairOrder> repairOrders;
    int nextRepairOrderId = 1;

    void updateNextId();

public:
    RepairOrderRepository() = default;
    RepairOrderRepository(const RepairOrderRepository& other)
    : repairOrders(other.repairOrders) {}

    void addOrder(const RepairOrder& order);

    RepairOrder& findOrderById(int orderId);
    const RepairOrder& findOrderById(int orderId) const;

    const RepairOrder* findActiveRepairByVehicleId(int vehicleId) const;

    std::vector<RepairOrder> getUnfinishedRepairs() const;

    std::vector<RepairOrder> getRepairOrders() const;
    void setRepairOrders(const std::vector<RepairOrder>& orders);
    int getNextRepairOrderId() const;


};


#endif //GARAGESERVICE_REPAIRORDERREPOSITORY_H
