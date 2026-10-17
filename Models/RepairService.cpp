//
// Created by Kolio on 10/17/2026.
//

#include "RepairService.h"


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

