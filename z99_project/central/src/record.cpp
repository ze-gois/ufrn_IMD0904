#include "./time.h"
#include "./record.h"

static bool do_record = true;

void record(std::string msg) {
    if (!do_record) return;
    Serial.print(msg.c_str());
}

std::string recordf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int size = std::vsnprintf(nullptr, 0, format, args) + 1; 
    va_end(args);
    
    std::string result(size, '\0'); 
    
    va_start(args, format);
    std::vsnprintf(&result[0], size, format, args);
    va_end(args);

    result.pop_back();
    record(result);
    return result;
}

std::string recordfln(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int size = std::vsnprintf(nullptr, 0, format, args) + 1;
    va_end(args);

    std::string result(size, '\0');

    va_start(args, format);
    std::vsnprintf(&result[0], size, format, args);
    va_end(args);

    result.pop_back();
    record(result + "\n");
    return result;
}