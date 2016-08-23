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

namespace Command {
    enum class Base : unsigned {
        _Begin,
        _End = _Begin,
    };
}

namespace Property {
    enum class Base : unsigned {
        _Begin,
        _End = _Begin,
    };
}

namespace Processor {

    class Base : public std::list<std::shared_ptr<Base> > {
    public:
        virtual ~Base();

        template <class E>
        void call(E commandId)
        {
            static_assert(E::_Begin != E::_End, "Wrong command ID type");
            callCommand(static_cast<unsigned>(commandId));
        }

        static std::shared_ptr<Base> allocate(
            const std::string& className, Type format);

        template <class E>
        Any get(E propertyId) const
        {
            static_assert(E::_Begin != E::_End, "Wrong property ID type");
            return property(static_cast<unsigned>(propertyId));
        }

        unsigned id() const;

        template <class E>
        void set(E propertyId, const Any& value)
        {
            static_assert(E::_Begin != E::_End, "Wrong property ID type");
            setProperty(static_cast<unsigned>(propertyId), value);
        }

    protected:
        typedef void (Base::*Handler)();

        Base();

        virtual void commandInit();
        bool hasInternalBuffer();
        virtual void process() = 0;

        template <typename E, typename D>
        static void addCommand(E commandId, void (D::*handler)())
        {
            static_assert(E::_Begin != E::_End, "Wrong command ID type");
            _handlers[static_cast<unsigned>(commandId)]
                = static_cast<Handler>(handler);
        }

    private:
        static std::vector<Allocator<Base> > _allocators;
        static std::map<unsigned, Handler> _handlers;
        unsigned _id;
        static unsigned _nextId;
        std::map<unsigned, Any> _properties;

        void callCommand(unsigned commandId);
        Any property(unsigned propertyId) const;
        void setProperty(unsigned propertyId, const Any& value);
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
