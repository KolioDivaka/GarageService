

#ifndef GARAGESERVICE_STRINGUTILS_H
#define GARAGESERVICE_STRINGUTILS_H

#include <string>

namespace StringUtils {

    // Премахва интервалите и преобразува всички букви в малки за лесно търсене
    std::string normalizeForSearch(const std::string& text);

    // Проверява дали низът е празен или съдържа само интервали, табулации и нови редове
    bool isBlank(const std::string& text);

}

#endif //GARAGESERVICE_STRINGUTILS_H