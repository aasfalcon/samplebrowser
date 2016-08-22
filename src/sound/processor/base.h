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
    namespace Base {
        enum {
            _Begin = 0,
            _End = _Begin,
        };
    }
}

namespace Property {
    namespace Base {
        enum {
            _Begin = 0,
            _End = _Begin,
        };
    }
}

namespace Processor {

    class Base : public std::list<std::shared_ptr<Base> > {
    public:
        virtual ~Base();

        void call(unsigned commandId);
        static std::shared_ptr<Base> allocate(
            const std::string& className, Type format);
        Any get(unsigned propertyId) const;
        unsigned id() const;
        void set(unsigned propertyId, const Any& value);

    protected:
        typedef void (Base::*Handler)();

        Base();

        virtual void commandInit();
        bool hasInternalBuffer();
        virtual void process() = 0;

        template <typename D>
        static void registerCommand(unsigned commandId, void (D::*handler)())
        {
            _handlers[commandId] = static_cast<Handler>(handler);
        }

    private:
        static std::vector<Allocator<Base> > _allocators;
        static std::map<unsigned, Handler> _handlers;
        unsigned _id;
        static unsigned _nextId;
        std::map<unsigned, Any> _properties;
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
