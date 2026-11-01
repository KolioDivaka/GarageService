//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_REPAIRORDERREPOSITORY_H
#define GARAGESERVICE_REPAIRORDERREPOSITORY_H
#include "../Models/RepairOrder.h"
#include <vector>

using namespace std;

class RepairOrderRepository {

private:
    vector<RepairOrder> repairOrders;
    int nextRepairOrderId = 1;

    void updateNextId();

public:
    RepairOrderRepository() = default;
    explicit RepairOrderRepository(const vector<RepairOrder>& initialOrders);

    void addOrder(const RepairOrder& order);

    RepairOrder& findOrderById(int orderId);
    const RepairOrder& findOrderById(int orderId) const;

    const RepairOrder* findActiveRepairByVehicleId(int vehicleId) const;
    vector<RepairOrder> getUnfinishedRepairs() const;

    const vector<RepairOrder>& getRepairOrders() const;
    void setRepairOrders(const vector<RepairOrder>& orders);
    int getNextRepairOrderId() const;


};


#endif //GARAGESERVICE_REPAIRORDERREPOSITORY_H
