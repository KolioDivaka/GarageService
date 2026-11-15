//
// Created by Kolio on 10/20/2026.
//

#include "CustomerRepository.h"
#include "../Models/Customer.h"
#include <limits>

#include <stdexcept>

#include "../Utility/StringUtils.h"


CustomerRepository::CustomerRepository(const std::vector<Customer> &initialCustomers) {
    customers = initialCustomers;
    updateNextCustomerId();
}

void CustomerRepository::addCustomer(const Customer &customer) {
    if (customerExists(customer.getCustomerId())) {
        throw std::invalid_argument( "Customer ID already exists: " + std::to_string(customer.getCustomerId()) );
    }

    if (customer.getCustomerId() == std::numeric_limits<int>::max()) {
        throw std::overflow_error("Cannot add customer: ID limit reached!");
    }

    customers.push_back(customer);
    updateNextCustomerId();


}

bool CustomerRepository::customerExists(int customerId) const {
        for (const Customer& customer :customers) {
        if (customer.getCustomerId() == customerId) {
            return true;
        }
    }
    return false;
}

Customer & CustomerRepository::findCustomerById(int customerId) {
    Customer* customer = nullptr;

    for (Customer& c : customers) {
        if (c.getCustomerId()==customerId) {
            customer = &c;
            break;
        }
    }
    if (!customer) {
        throw std::invalid_argument("Cannot find customer with ID:"+ std::to_string(customerId));
    }
    return *customer;
}

void CustomerRepository::editCustomer(int customerId, const std::string &name, const std::string &phoneNumber) {
    Customer& customer = findCustomerById(customerId);

    if (StringUtils::isBlank(name) && StringUtils::isBlank(phoneNumber)) {
        return;
    }

    Customer updatedCustomer = customer;
    if (!StringUtils::isBlank(name)) {
        updatedCustomer.setName(name);
    }
    if (!StringUtils::isBlank(phoneNumber)) {
        updatedCustomer.setPhoneNumber(phoneNumber);
    }
    customer = updatedCustomer;
}

void CustomerRepository::updateNextCustomerId() {
    int maxCustomerId = 0;

    for (const Customer& customer : customers) {
        maxCustomerId = std::max(maxCustomerId, customer.getCustomerId());
    }

    constexpr int maxInt = std::numeric_limits<int>::max();


    if (maxCustomerId == maxInt) {
        throw std::overflow_error(
            "Cannot generate repair order IDs: int limit reached!");
    }
    nextCustomerId = maxCustomerId+1;
}
