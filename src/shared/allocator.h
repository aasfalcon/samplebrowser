#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <map>
#include <memory>
#include <list>
#include <vector>

#include "shared/iplugin.h"

class Allocator: public IPlugin
{
public:
    typedef Interface *(*Create)();
    typedef void (*Destroy)(Interface *);

    Allocator();
    ~Allocator();

    Interface *create(const char *tag);
    void destroy(Interface *object);
    void destroyAll();
    bool isOwner(Interface *object) const;

protected:
    void addTag(const std::string &tag, Create create, Destroy destroy);
    std::vector<const char *> tags() const;

private:
    struct Record {
        Create create;
        Destroy destroy;
    };

    std::map<Interface *, std::shared_ptr<Interface>> _allocated;
    std::map<std::string, Record> _records;
};

#endif // ALLOCATOR_H
