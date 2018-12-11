#include "logging.h"


#define MIN_LOG_LEVEL_UNSET INT_MAX


namespace {

    int minLogLevel = MIN_LOG_LEVEL_UNSET;

}


namespace renderbox {

    int getMinLogLevel() {
        if (minLogLevel == MIN_LOG_LEVEL_UNSET) {
            if (const char *envRenderBoxLogLevel = std::getenv("RENDERBOX_LOG_LEVEL")) {
                minLogLevel = std::stoi(envRenderBoxLogLevel);
            } else {
                minLogLevel = MIN_LOG_LEVEL_DEFAULT;
            }
        }

        return minLogLevel;
    }

    void setMinLogLevel(int level) {
        minLogLevel = level;
    }

}
