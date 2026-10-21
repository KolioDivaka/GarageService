//
// Created by Kolio on 10/17/2026.
//

#include "RepairService.h"
#include <ostream>
#include <sstream>
#include <string>

RepairService::RepairService() {
    this->description = "Repair Unknow!";
    this->hoursLabor = 0;
    this->hourlyRate = 0;
}

RepairService::RepairService(const std::string &description, double hoursLabor, double hourlyRate) {
    setDescription(description);
    setHoursLabor(hoursLabor);
    setHourlyRate(hourlyRate);
}

std::ostream & operator<<(std::ostream &out, const RepairService &service) {
    out << service.getDerscription()
        << " — " << service.getHoursLabor() << "h x " << service.getHourlyRate()
        << " = " << service.calculateCost();
    return out;

}
