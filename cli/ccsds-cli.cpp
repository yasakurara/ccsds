#include "logging/Logging.hpp"

int main(int argc, char* argv[]) {
//   ccsds();
    std::string msg = "Hello World!";
    LOG_CRITICAL("CRITICAL %s", msg);
    LOG_INFO("INFO %s", msg);
    LOG_DEBUG("DEBUG %s", msg);
    return 0;
}