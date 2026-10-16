//
// Created by Kolio on 10/16/2026.
//

#ifndef GARAGESERVICE_CUSTOMER_H
#define GARAGESERVICE_CUSTOMER_H
#include <string>


class Customer {
private:
    int customerId{};
    std::string name;
    std::string phoneNumber;

public:
    Customer();
    Customer(int customerId, const std::string& name,const std::string& phoneNumber);

    [[nodiscard]] int customer_id() const {
        return customerId;
    }

    [[nodiscard]] std::string name1() const {
        return name;
    }

    [[nodiscard]] std::string phone_number() const {
        return phoneNumber;
    }

    void setCustomerId(int customerId);
    void setName(const std::string& newName);
    void setPhoneNumber(const std::string& new_phone_number);
};


#endif //GARAGESERVICE_CUSTOMER_H
