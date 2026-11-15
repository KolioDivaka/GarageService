#include "StringUtils.h"
#include <cctype>

namespace StringUtils {

    std::string normalizeForSearch(const std::string& text) {
        std::string result;
        result.reserve(text.size());

        for (const unsigned char ch : text) {
            // Използваме std::isblank от <cctype>, за да филтрираме интервали и табулации
            if (!std::isblank(ch)) {
                result += static_cast<char>(std::tolower(ch));
            }
        }

        return result;
    }

    bool isBlank(const std::string& text) {
        return text.empty() || text.find_first_not_of(" \t\r\n") == std::string::npos;
    }

}   