#ifndef ERROR_H
#define ERROR_H

#include <QException>

#include <QString>

namespace Sound {

class Error : public QException
{
public:
    ~Error() throw() {}
    QString message;

    virtual QString toStrinng() const;
};

class ResamplerError : public Error
{
};

class FileError : public Error
{
public:
    ~FileError() throw() {}
    QString path;

    QString toStrinng() const;
};

class ChunkError : public Error
{
public:
    ~ChunkError() throw() {}
    QString chunk;
    QString field;
    QString value;

    QString toStrinng() const;
};

class LibraryError : public FileError
{
public:
    ~LibraryError() throw() {}
    QString error;
    QString log;
    QString version;

    QString toStrinng() const;
};

} // namespace Sound

#endif // ERROR_H
