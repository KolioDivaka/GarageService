//
// Created by Kolio on 10/29/2026.
//

#ifndef GARAGESERVICE_DATEUTILS_H
#define GARAGESERVICE_DATEUTILS_H
#include <chrono>
#include <string>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

namespace DateUtils {
    std::string formatDate(const std::optional<TimePoint>& date);
    std::tm parseCalendarDate(const std::string& text);

    TimePoint toTimePoint(std::tm date, bool nextDay = false);
};


#endif //GARAGESERVICE_DATEUTILS_H
