#ifndef LOG_H
#define LOG_H

#include "../src/main/config.h"
#include <log4cplus/loggingmacros.h>

#define LOG(severity, logEvent)                                            \
    LOG4CPLUS_##severity(log4cplus::Logger::getInstance(APPLICATION_NAME), \
        logEvent)

#define LOG_ERROR_AND_THROW(a_message, a_exception) \
    auto message = a_message;                        \
    LOG(ERROR, message);                             \
    throw std::a_exception(message);

#define LOGIC_ERROR(a_message) \
    LOG_ERROR_AND_THROW(a_message, logic_error);

#define RUNTIME_ERROR(a_message) \
    LOG_ERROR_AND_THROW(a_message, runtime_error);

#define OUT_OF_RANGE(a_message) \
    LOG_ERROR_AND_THROW(a_message, out_of_range);

#define OVERFLOW_ERROR(a_message) \
    LOG_ERROR_AND_THROW(a_message, overflow_error);

#define BAD_CAST(a_message) \
    LOG(ERROR, a_message); \
    throw std::bad_cast();

#define ERROR_IF(a_expression, a_error, a_message) \
    if (a_expression) { a_error(a_message); }

#define LOG_IF(a_expression, a_severity, a_logEvent) \
    if (a_expression) { LOG(a_severity, a_logEvent); }

#define QTR(a_text) \
    QObject::tr(a_text).toStdString()

#endif // LOG_H
