//
// Created by Kolio on 10/16/2026.
//

#include "Customer.h"

#include <regex>
#include <stdexcept>

Customer::Customer() {
    this->customerId = 0;
    this->name = "Unknown";
    this->phoneNumber = "Unknown";
}
Customer::Customer(int customerId, const std::string &name,const std::string &phoneNumber) {
    setCustomerId(customerId);
    setName(name);
    setPhoneNumber(phoneNumber);
}

void Customer::setCustomerId(int customer_id) {
    if ( customer_id < 0) {
       throw std::invalid_argument("Customer Id cannot be negative!!!");
    }
        this->customerId = customer_id;


}

void Customer::setName(const std::string& newName) {
    if (newName.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Name cannot be empty!");
    }

    this->name = newName;
}
void Customer::setPhoneNumber(const std::string &new_phone_number) {
    // Uses universal regex for phone numbers
    const std::regex pattern(R"(^\+?[\s\-\(\).]*(?:[0-9][\s\-\(\).]*){6,14}[0-9]$)");

    if (std::regex_match(new_phone_number, pattern)) {
         this->phoneNumber = new_phone_number;
        return;
    }
    throw std::invalid_argument("Invalid phone number format: " + new_phone_number);

}
