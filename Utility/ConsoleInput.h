//
// Created by Kolio on 10/29/2026.
//

#ifndef GARAGESERVICE_CONSOLEINPUT_H
#define GARAGESERVICE_CONSOLEINPUT_H
#include <exception>
#include <optional>
#include <string>

class InputClosed final : public std::exception {
public:
    const char* what() const noexcept override {
        return "Console input is closed.";
    }
};

namespace  ConsoleInput {
    std::string readLine(const std::string &prompt);
    int readInt(const std::string &prompt);
    int readPositiveInt(const std::string &prompt);
    std::optional<int> readOptionalInt(const std::string &prompt);
    double readPositiveDouble(const std::string &prompt);
}


#endif //GARAGESERVICE_CONSOLEINPUT_H
