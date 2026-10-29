//
// Created by Kolio on 10/29/2026.
//

#include "DateUtils.h"
namespace DateUtils {
    std::string formatDate(const std::optional<TimePoint> &date) {
        if (!date.has_value()) {
            return "-";
        }

        const std::time_t rawTime =
            std::chrono::system_clock::to_time_t(*date);

        const std::tm* localTime = std::localtime(&rawTime);

        if (localTime == nullptr) {
            return "-";
        }

        const std::tm calendarDate = *localTime;

        std::ostringstream result;
        result << std::put_time(&calendarDate, "%Y-%m-%d");
        return result.str();
    }

    std::tm parseCalendarDate(const std::string &text) {
        if (text.size() != 10
                || text[2] != '.'
                || text[5] != '.') {
            throw std::invalid_argument(
                "Invalid date format. Use DD.MM.YYYY."
            );
                }

        for (std::size_t i = 0; i < text.size(); ++i) {
            if (i == 2 || i == 5) {
                continue;
            }

            if (text[i] < '0' || text[i] > '9') {
                throw std::invalid_argument(
                    "The date must contain digits and dots only."
                );
            }
        }

        int day = 0;
        int month = 0;
        int year = 0;
        char firstDot = '\0';
        char secondDot = '\0';

        std::istringstream stream(text);

        if (!(stream >> day >> firstDot
                     >> month >> secondDot >> year)) {
            throw std::invalid_argument("Cannot read the date.");
                     }

        if (year < 1900
            || month < 1 || month > 12
            || day < 1 || day > 31) {
            throw std::invalid_argument("Invalid calendar date.");
            }

        std::tm calendarDate{};
        calendarDate.tm_mday = day;
        calendarDate.tm_mon = month - 1;
        calendarDate.tm_year = year - 1900;
        calendarDate.tm_isdst = -1;

        const std::time_t timestamp = std::mktime(&calendarDate);

        if (timestamp == static_cast<std::time_t>(-1)) {
            throw std::invalid_argument(
                "Date is outside the supported range."
            );
        }

        // mktime може да премести невалидна дата
        // в следващ месец — отхвърляме такава промяна.
        if (calendarDate.tm_mday != day
            || calendarDate.tm_mon != month - 1
            || calendarDate.tm_year != year - 1900) {
            throw std::invalid_argument(
                "This calendar date does not exist."
            );
            }

        return calendarDate;
    }

    TimePoint toTimePoint(std::tm date, bool nextDay) {if (nextDay) {
        ++date.tm_mday;
    }

        date.tm_isdst = -1;

        const std::time_t timestamp = std::mktime(&date);

        if (timestamp == static_cast<std::time_t>(-1)) {
            throw std::invalid_argument(
                "Date is outside the supported range."
            );
        }

        return std::chrono::system_clock::from_time_t(timestamp);
    }
}
