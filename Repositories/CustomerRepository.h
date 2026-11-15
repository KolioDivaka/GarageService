//
// Created by Kolio on 10/20/2026.
//

#ifndef GARAGESERVICE_CUSTOMERREPOSITORY_H
#define GARAGESERVICE_CUSTOMERREPOSITORY_H
#include <string>
#include <vector>

#include "../Services/FileManager.h"

class Customer;

class CustomerRepository {

private:
    std::vector<Customer> customers;
    int  nextCustomerId= 1;

public:
    CustomerRepository() = default;


    explicit CustomerRepository(const std::vector<Customer>& initialCustomers);

    void addCustomer( const Customer &customer );
    [[nodiscard]] bool customerExists(int customerId) const;
    Customer& findCustomerById(int customerId);
    void editCustomer( int customerId, const std::string& name, const std::string& phoneNumber);


    [[nodiscard]] std::vector<Customer> getCustomers() const {
        return customers;
    }

    [[nodiscard]] int getNextCustomerId() const {
        return nextCustomerId;
    }

    void setCustomers( const std::vector<Customer>& loadedCustomers ) {
       this->customers = loadedCustomers;
        updateNextCustomerId();
    }
private:
    void updateNextCustomerId();

};


#endif //GARAGESERVICE_CUSTOMERREPOSITORY_H
