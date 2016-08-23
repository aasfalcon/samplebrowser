#ifndef SOUND_PROCESSOR_BASE_H
#define SOUND_PROCESSOR_BASE_H

#include <list>
#include <map>
#include <memory>
#include <vector>

#include "object.h"
#include "shared/allocator.h"
#include "shared/any.h"

namespace Sound {

SOUND_ENUM_NAMESPACE(Command);
SOUND_ENUM_NAMESPACE(Property);

namespace Processor {

    class Base : public std::list<std::shared_ptr<Base> > {
    public:
        virtual ~Base();

        void call(Command::ID commandId);
        static std::shared_ptr<Base> allocate(
            const std::string& className, Type format);
        Any get(Property::ID propertyId) const;
        unsigned id() const;
        void set(Property::ID propertyId, const Any& value);

    protected:
        typedef void (Base::*Handler)();

        Base();

        virtual void commandInit();
        bool hasInternalBuffer();
        virtual void process() = 0;

        template <typename D>
        static void addCommand(Command::ID commandId, void (D::*handler)())
        {
            _handlers[commandId] = static_cast<Handler>(handler);
        }

    private:
        static std::vector<Allocator<Base> > _allocators;
        static std::map<Command::Index, Handler> _handlers;
        unsigned _id;
        static unsigned _nextId;
        std::map<Property::Index, Any> _properties;
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
