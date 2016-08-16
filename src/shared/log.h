#ifndef LOG_H
#define LOG_H

#include <log4cplus/loggingmacros.h>
#include "../src/main/config.h"

#define LOG(severity, logEvent) \
    LOG4CPLUS_##severity(log4cplus::Logger::getInstance(APPLICATION_NAME), \
                         logEvent)

#endif // LOG_H
