#include "allocator.h"

Allocator::Allocator()
{
}

Allocator::~Allocator()
{
    destroyAll();
}

void Allocator::addTag(const std::string &tag,
                       Allocator::Create create,
                       Allocator::Destroy destroy)
{
    _records[tag] = { create, destroy };
}

std::vector<const char *> Allocator::tags() const
{
    std::vector<const char *> result;

    for (auto it = _records.cbegin(); it != _records.cend(); ++it) {
        result.push_back(it->first.c_str());
    }

    return result;
}

Interface *Allocator::create(const char *tag)
{
    const Record &record = _records.at(tag);
    Interface *object = (*record.create)();
    _allocated[object] = std::shared_ptr<Interface>(object, record.destroy);
    return object;
}

void Allocator::destroy(Interface *object)
{
    if (!isOwner(object)) {
        throw std::runtime_error("Attempt to deallocate unknown object");
    }

    _allocated.erase(object);
}

void Allocator::destroyAll()
{
    _allocated.clear();
}

bool Allocator::isOwner(Interface *object) const
{
    return _allocated.find(object) != _allocated.end();
}
