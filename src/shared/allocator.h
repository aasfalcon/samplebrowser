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

    void addTag(const std::string& tag,
        Constructor create, Destructor destroy = nullptr)
    {
        _records[tag] = { create, destroy };
    }

    virtual T* create(const std::string& tag)
    {
        auto& record = _records.at(tag);
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
    std::map<std::string, Record> _records;

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
};

#endif // ALLOCATOR_H
