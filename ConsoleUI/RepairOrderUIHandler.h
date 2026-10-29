//
// Created by Kolio on 10/29/2026.
//

#ifndef GARAGESERVICE_REPAIRORDERUIHANDLER_H
#define GARAGESERVICE_REPAIRORDERUIHANDLER_H

#include "../Services/AutoWorkshopManager.h"

class RepairOrderUIHandler {
private:
    AutoWorkshopManager& manager;

public:
    explicit RepairOrderUIHandler(AutoWorkshopManager& manager) : manager(manager) {}

    void handleAcceptVehicle();
    void handleOrderActions(int orderId);
    void handleUnfinishedRepairs();
    void handleTurnover();

private:
    void showBill(int orderId) const;
    void promptNewService(int orderId);
    void promptNewPart(int orderId);

};


#endif //GARAGESERVICE_REPAIRORDERUIHANDLER_H
