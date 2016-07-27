#include <QException>
#include <QString>

#include "error.h"

using namespace Sound;

// Error
QString Error::toStrinng() const
{
    return message;
}


// FileError
QString FileError::toStrinng() const
{
    return QString(
                "File error for path: %1\n"
                "Message: %2"
                ).arg(path, message);
}


// ChunkError
QString ChunkError::toStrinng() const
{
    return QString(
                "Invalid value in chunk: %1, field: %2\n"
                "Message: %3"
                "Supplied value is: \"%4\""
                ).arg(chunk, field, message, value);
}

// LibraryError
QString LibraryError::toStrinng() const
{
    return QString(
                "I/O library error for file: %1\n"
                "Message: %2\n"
                "Library version: %3"
                "Library log follows:\n%4"
                ).arg(path, message, version,
                      log.isEmpty() ? "[ Empty ]" : log);
}
