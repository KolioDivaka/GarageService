//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_PART_H
#define GARAGESERVICE_PART_H
#include <string>


class Part {
private:
    std::string partName;
    std::string partNumber;
    double unitPrice{};
    int quantity{};

public:
    Part();
    Part(const std::string& partName,const std::string& partNumber, double unitPrice, int quantity);

    void setPartName(const std::string& part_name);
    void setPartNumber(const std::string& part_number);
    void setUnitPrice(double unit_price);
    void setQuantity(int newQuantity);

    [[nodiscard]] std::string part_name() const {
        return partName;
    }

    [[nodiscard]] std::string part_number() const {
        return partNumber;
    }

    [[nodiscard]] double unit_price() const {
        return unitPrice;
    }

    [[nodiscard]] int quantity1() const {
        return quantity;
    }


    [[nodiscard]]double calculateCost() const {
        return unitPrice * quantity;
    }

};


#endif //GARAGESERVICE_PART_H
