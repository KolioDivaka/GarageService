//
// Created by Kolio on 10/29/2026.
//

#ifndef GARAGESERVICE_CUSTOMERUIHANDLER_H
#define GARAGESERVICE_CUSTOMERUIHANDLER_H

#include "../Services/AutoWorkshopManager.h"
#include <vector>

class CustomerUIHandler {
private:
    AutoWorkshopManager& manager;

public:
    explicit CustomerUIHandler(AutoWorkshopManager& manager) : manager(manager) {}

    int promptNewCustomer();
    void handleEditCustomer();
    int chooseCustomer();

private:
    int pickCustomerId(const std::vector<Customer>& matches) const;
};


#endif //GARAGESERVICE_CUSTOMERUIHANDLER_H
