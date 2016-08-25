#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "log.h"

template <typename T>
class Allocator {
public:
    typedef T* (*Constructor)();
    typedef void (*Destructor)(T*);

    struct Record {
        Constructor create;
        Destructor destroy;
    };

    Allocator() {}
    Allocator(const std::map<std::string, Record>& initializer)
        : _records(initializer)
    {
    }

    virtual ~Allocator() {}

    T* create(const std::string& tag)
    {
        const Record& record = this->record(tag);
        T* object = (*record.create)();
        _allocated[object] = std::shared_ptr<T>(object, record.destroy);
        return object;
    }

    void destroy(T* object)
    {
        if (!isOwner(object)) {
            RUNTIME_ERROR("Attempt to deallocate unknown object");
        }

        _allocated.erase(object);
    }

    bool isOwner(T* object) const
    {
        return _allocated.find(object) != _allocated.end();
    }

protected:
    void addTag(const std::string& tag,
        Constructor create, Destructor destroy = nullptr)
    {
        _records[tag] = { create, destroy };
    }

    const Record &record(const std::string &tag)
    {
        return _records.at(tag);
    }

    std::list<std::string> tags() const
    {
        std::list<std::string> result;

        for (auto it = _records.cbegin(); it != _records.cend(); ++it) {
            result.push_back(it->first);
        }

        return result;
    }

private:
    std::map<T*, std::shared_ptr<T> > _allocated;
    std::map<std::string, Record> _records;
};

#endif // ALLOCATOR_H
