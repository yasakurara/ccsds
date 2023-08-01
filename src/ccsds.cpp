#include <iostream>
#include "ccsds.hpp"
#include "logging/Logging.hpp"

void ccsds() {
    std::string msg = "Hello World!";
    LOG_CRITICAL("CRITICAL %s", msg);
    LOG_INFO("INFO %s", msg);
    LOG_DEBUG("DEBUG %s", msg);
}