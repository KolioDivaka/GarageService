//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_REPAIRORDER_H
#define GARAGESERVICE_REPAIRORDER_H
#include <chrono>
#include <vector>

#include "Part.h"
#include "RepairService.h"
#include "RepairStatus.h"


class RepairOrder {
private:
    //Id's of the order, customer and vehicle
    int orderId{};
    int customerId{};
    int vehicleId{};
    std::string customerStatement;

    //Where we store the labor hours and parts used
    std::vector<RepairService> repairServices;
    std::vector<Part> parts;
    // The status if it is Accepted, InProgres, Completed and Paid
    RepairStatus repairStatus = RepairStatus::Accepted;

    using TimePoint = std::chrono::system_clock::time_point;
    //We have the date when we accepted the order
    TimePoint acceptedDate;
    
    // The time of completion and paying off the order (null on creation)
    std::optional<TimePoint> completedDate;
    std::optional<TimePoint> paidDate;

    //
    void validateState() const;

public:
    RepairOrder();
    RepairOrder(int orderId,int customerId,int vehicleId,const std::string& customerStatement);
    RepairOrder(int orderId,
        int customerId,
        int vehicleId,
        const std::string& customerStatement,
        const std::vector<RepairService>& repairServices,
        const std::vector<Part>& parts,
        RepairStatus repairStatus,
        TimePoint acceptedDate,
        std::optional<TimePoint> completedDate,
        std::optional<TimePoint> paidDate);

    void setOrderId(int order_id);
    void setCustomerId(int customer_id);
    void setVehicleId(int vehicle_id);
    void setCustomerStatement(const std::string& customer_statement);


    [[nodiscard]]RepairStatus getStatus() const;
    void startRepair();
    void completeRepair();
    void payRepair();

};


#endif //GARAGESERVICE_REPAIRORDER_H
