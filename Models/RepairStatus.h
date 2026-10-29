//
// Created by Kolio on 10/17/2026.
//

#ifndef GARAGESERVICE_REPAIRSTATUS_H
#define GARAGESERVICE_REPAIRSTATUS_H


enum class RepairStatus {
    Accepted,
    InProgress,
    Completed,
    Paid    
};
inline std::string toString(RepairStatus status) {
    switch (status) {
        case RepairStatus::Accepted: return "Accepted";
        case RepairStatus::InProgress: return "In Progress";
        case RepairStatus::Completed: return "Completed";
        case RepairStatus::Paid: return "Paid";
        default: return "Unknown";
    }
}


#endif //GARAGESERVICE_REPAIRSTATUS_H
