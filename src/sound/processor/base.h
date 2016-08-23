#ifndef SOUND_PROCESSOR_BASE_H
#define SOUND_PROCESSOR_BASE_H

#include <list>
#include <map>
#include <memory>
#include <typeinfo>
#include <vector>

#include "object.h"
#include "processormacros.h"
#include "realtimeany.h"
#include "shared/allocator.h"
#include "shared/any.h"

namespace Sound {

SOUND_ENUM_NAMESPACE(Command);
SOUND_ENUM_NAMESPACE(Parameter);
SOUND_ENUM_NAMESPACE(Property);

namespace Processor {

    class Base : public std::list<std::shared_ptr<Base> > {
    public:
        virtual ~Base();

        void call(Command::ID commandId);
        RealtimeAny get(Parameter::ID id) const;
        unsigned id() const;
        Any property(Property::ID id) const;
        void set(Parameter::ID id, RealtimeAny value);
        void setProperty(Property::ID id, const Any& value);

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

        void setParameterCount(unsigned count);

    private:
        static std::vector<Allocator<Base> > _allocators;
        static std::map<Command::Index, Handler> _handlers;
        unsigned _id;
        static unsigned _nextId;
        std::map<Property::Index, Any> _properties;
        std::vector<RealtimeAny> _parameters;
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
