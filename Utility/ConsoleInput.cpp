//
// Created by Kolio on 10/29/2026.
//

#include "ConsoleInput.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace ConsoleInput {
    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string text;

        if (!std::getline(std::cin, text)) {
            throw InputClosed();
        }
        return text;
    }

    int readInt(const std::string& prompt) {
        while (true) {
            const std::string text = readLine(prompt);
            std::istringstream input(text);
            int value;

            if (input >> value) {
                input >> std::ws;

                if (input.eof()) {
                    return value;
                }
            }

            std::cout << "Enter a valid whole number.\n";
        }
    }

    int readPositiveInt(const std::string& prompt) {
        while (true) {
            const int value = readInt(prompt);
            if (value > 0) {
                return value;
            }
            std::cout << "Enter a valid whole number.\n";
        }
    }

    std::optional<int> readOptionalInt(const std::string& prompt) {
        while (true) {
            const std::string text = readLine(prompt);

            if (text.find_first_not_of(" \t\r\n") == std::string::npos) {
                return std::nullopt;
            }

            std::istringstream input(text);
            int value;

            if (input >> value) {
                input >> std::ws;
                if (input.eof()) {
                    return value;
                }
            }

            std::cout << "Enter a whole number or leave empty.\n";
        }
    }

    double readPositiveDouble(const std::string& prompt) {
        while (true) {
            const std::string raw = readLine(prompt);

            try {
                std::size_t consumed = 0;
                const double value = std::stod(raw, &consumed);

                if (consumed != raw.size()) {
                    throw std::invalid_argument("");
                }
                if (value <= 0.0) {
                    std::cout << "The number must be positive. Try again.\n";
                    continue;
                }

                return value;
            }
            catch (const std::exception&) {
                std::cout << "Please enter a valid number (e.g. 12.50).\n";
            }
        }
    }
}

