#include <algorithm>
#include <stdexcept>

#include "allocator.h"

Allocator::Allocator()
{   
}

Allocator::~Allocator()
{
}

void Allocator::addTag(const char *tag, Allocator::Create create, Allocator::Destroy destroy)
{
    _records[tag] = { create, destroy };
}

std::vector<const char *> Allocator::tags() const
{
    std::vector<const char *> result;

    for (auto it = _records.begin(); it != _records.end(); ++it) {
        result.push_back(it->first.c_str());
    }

    return result;
}

Interface *Allocator::create(const char *tag)
{
    const Record &record = _records.at(tag);
    std::shared_ptr<Interface> ptr((*record.create)(), record.destroy);
    Interface *object = ptr.get();
    _map[object] = ptr;
    return object;
}

void Allocator::destroy(Interface *object)
{
    if (!isOwner(object)) {
        throw std::runtime_error("Attempt to deallocate unknown object");
    }

    _map.erase(object);
}

void Allocator::destroyAll()
{
    _map.clear();
}

bool Allocator::isOwner(Interface *object) const
{
    return _map.find(object) != _map.end();
}

