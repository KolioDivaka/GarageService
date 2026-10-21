//
// Created by Kolio on 10/21/2026.
//

#ifndef GARAGESERVICE_CLIENTHANDLER_H
#define GARAGESERVICE_CLIENTHANDLER_H
#include "AutoWorkshopManager.h"

using namespace std;


class ClientHandler {
private:
    AutoWorkshopManager& manager;

    static string readLine(const string& prompt);
    static int readInt(const string& prompt);
    static int readPositiveInt(const string& prompt);
    static optional<int> readOptionalInt(const string& prompt);
    double readPositiveDouble(const std::string& prompt) const;

    int pickCustomerId(const vector<Customer>& matches) const;
    int chooseCustomer();
    int chooseVehicle(int ownerId );

    void showBill(int orderId) const;
    //Customer Handling
    int promptNewCustomer();
    int promptNewVehicle(int presetCustomerId = 0);
    //Vehicle Handling
    void handleEditVehicle();
    void handleEditCustomer();

    //Order logic
    void promptNewService(int orderId);
    void promptNewPart(int orderId);


    //Buisnes Logic
    void handleAcceptVehicle();
    void handleOrderActions(int orderId);
    void handleFindByRegNum();
public:
    ClientHandler(AutoWorkshopManager& manager):manager(manager) {};


};


#endif //GARAGESERVICE_CLIENTHANDLER_H
