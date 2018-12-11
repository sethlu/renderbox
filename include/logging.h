#ifndef RENDERBOX_LOGGING_H
#define RENDERBOX_LOGGING_H


// Adapted from: https://cs.chromium.org/chromium/src/base/logging.h


#import <iostream>


namespace renderbox {

    int getMinLogLevel();

    void setMinLogLevel();

    class LogVoidify {
    public:
        // This has to be an operator with a precedence lower than << but higher than ?:
        // Resolves issue with ?: having void and std::ostream differently typed operands
        void operator&(std::ostream&) {}
    };

}


#define LOG_SEVERITY_DEBUG      0
#define LOG_SEVERITY_INFO       1
#define LOG_SEVERITY_WARNING    2
#define LOG_SEVERITY_FATAL      3

#define LOG_STREAM_DEBUG        std::clog << __FILE__ << ":" << __LINE__ << " DEBUG "
#define LOG_STREAM_INFO         std::clog << __FILE__ << ":" << __LINE__ << " INFO "
#define LOG_STREAM_WARNING      std::clog << __FILE__ << ":" << __LINE__ << " WARNING "
#define LOG_STREAM_FATAL        std::clog << __FILE__ << ":" << __LINE__ << " FATAL "

#define LOG_SEVERITY(severity) LOG_SEVERITY_ ## severity

#define MIN_LOG_LEVEL_DEFAULT LOG_SEVERITY(WARNING)

#define LOG_STREAM(severity) LOG_STREAM_ ## severity

#define LAZY_STREAM(stream, condition) !(condition) ? (void) 0 : renderbox::LogVoidify() & (stream)

#define LOG_IS_ON(severity) LOG_SEVERITY(severity) >= renderbox::getMinLogLevel()

#define LOG(severity) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))

#define LOG_IF(severity, condition) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity) && (condition))

#define LOG_ASSERT(condition) LOG_IF(FATAL, !(condition)) << "Assert failed: " #condition "."


#endif //RENDERBOX_LOGGING_H
