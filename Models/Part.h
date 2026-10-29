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

    [[nodiscard]] std::string getPartName() const;
    [[nodiscard]] std::string getPartNumber() const;
    [[nodiscard]] double getUnitPrice() const;
    [[nodiscard]] int getQuantity() const;

    void setPartName(const std::string& part_name);
    void setPartNumber(const std::string& part_number);
    void setUnitPrice(double unit_price);
    void setQuantity(int newQuantity);



    [[nodiscard]]double calculateCost() const {
        return unitPrice * quantity;
    }
};


#endif //GARAGESERVICE_PART_H
