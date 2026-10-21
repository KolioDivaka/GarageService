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
    int pickCustomerId(const vector<Customer>& matches) const;
    int chooseCustomer();
    int chooseVehicle(int ownerId );

    //Customer Handling
    int promptNewCustomer();
    int promptNewVehicle(int presetCustomerId = 0);

    //Vehicle Handling
    void handleEditVehicle();
    void handleEditCustomer();

    //Buisnes Logic
    void handleAcceptVehicle();
public:
    ClientHandler(AutoWorkshopManager& manager):manager(manager) {};


};


#endif //GARAGESERVICE_CLIENTHANDLER_H
