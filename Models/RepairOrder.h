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
    void ensureEditable() const;

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
    void setStatus(const RepairStatus& repair_status);


    [[nodiscard]]RepairStatus getStatus() const;
    //These methods act like setters for the Status and Date
    void startRepair();
    void completeRepair();
    void payRepair();

    [[nodiscard]] int getOrderId() const {
        return orderId;
    }

    [[nodiscard]] int getCustomerId() const {
        return customerId;
    }

    [[nodiscard]] int getVehicleId() const {
        return vehicleId;
    }

    [[nodiscard]] std::string getCustomerStatement() const {
        return customerStatement;
    }

    [[nodiscard]] std::vector<RepairService> getServices() const {
        return repairServices;
    }

    [[nodiscard]] std::vector<Part> getParts() const {
        return parts;
    }


    [[nodiscard]] TimePoint getAcceptedDate() const {
        return acceptedDate;
    }

    [[nodiscard]] std::optional<TimePoint> getCompleteDate() const {
        return completedDate;
    }

    [[nodiscard]] std::optional<TimePoint> getPaidDate() const {
        return paidDate;
    }

    void addRepairService(const RepairService& service);
    void addPart(const Part& part);


    [[nodiscard]] double calculateLaborCost() const;
    [[nodiscard]] double calculatePartsCost() const;
    [[nodiscard]] double calculateTotalCost() const;
};


#endif //GARAGESERVICE_REPAIRORDER_H
