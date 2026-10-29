//
// Created by Kolio on 10/17/2026.
//

#include "Part.h"

#include <stdexcept>

Part::Part() {
    this->partName = "Unknown part!";
    this->partNumber = "Unknown number!";
    this->quantity = 0;
    this->unitPrice = 0;
}

Part::Part(const std::string& partName,const std::string& partNumber, double unitPrice, int quantity) {
    setPartName(partName);
    setPartNumber(partNumber);
    setUnitPrice(unitPrice);
    setQuantity(quantity);
}

void Part::setPartName(const std::string& part_name) {

    if (part_name.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Part name cannot be empty!");

    }
    this->partName = part_name;
}

void Part::setPartNumber(const std::string &part_number) {
    if (part_number.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Part number cannot be empty!");
    }
    this->partNumber = part_number;
}

void Part::setUnitPrice(double unit_price) {
    if (unit_price < 0.0) {
        throw std::invalid_argument("Unit price cannot be negative !");
    }
    this->unitPrice = unit_price;
}

void Part::setQuantity(int newQuantity) {
    if (newQuantity < 0.0) {
        throw std::invalid_argument("Quantity cannot be negative !");
    }
    this->quantity = newQuantity;
}

std::string Part::getPartName() const {
    return this->partName;
}

std::string Part::getPartNumber() const {
    return partName;
}

double Part::getUnitPrice() const {
    return  unitPrice;
}

int Part::getQuantity() const {
    return quantity;
}


