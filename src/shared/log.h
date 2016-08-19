#ifndef LOG_H
#define LOG_H

#include "../src/main/config.h"
#include <log4cplus/loggingmacros.h>

#define LOG(severity, logEvent)                                            \
    LOG4CPLUS_##severity(log4cplus::Logger::getInstance(APPLICATION_NAME), \
        logEvent)

#define _LOG_ERROR_AND_THROW(__message, __exception) \
    auto message = __message;                        \
    LOG(ERROR, message);                             \
    throw std::__exception(message);

#define RUNTIME_ERROR(__message) \
    _LOG_ERROR_AND_THROW(__message, runtime_error);

#define OUT_OF_RANGE(__message) \
    _LOG_ERROR_AND_THROW(__message, out_of_range);

#define QTR(__text) \
    QObject::tr(__text).toStdString()

#endif // LOG_H
