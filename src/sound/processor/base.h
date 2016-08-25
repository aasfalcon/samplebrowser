#ifndef SOUND_PROCESSOR_BASE_H
#define SOUND_PROCESSOR_BASE_H

#include <memory>
#include <vector>

#include "object.h"
#include "realtimeany.h"
#include "shared/allocator.h"
#include "shared/any.h"
#include "shared/id.h"

namespace Sound {

ID_ENUM_NAMESPACE(Command);
ID_ENUM_NAMESPACE(Parameter);
ID_ENUM_NAMESPACE(Property);
ID_ENUM_NAMESPACE(Signal);

namespace Processor {
    class MessageBus;

    struct RuntimeInfo {
        MessageBus* bus;
        unsigned channelsInput;
        unsigned channelsOutput;
        unsigned frames;
        unsigned latency;
        void* rawInput;
        void* rawOutput;
        Type rawSampleFormat;
        Type sampleFormat;
        unsigned sampleRate;
        unsigned status;
        double time;
    };

    class Base : public std::list<std::shared_ptr<Base> > {
    public:
        virtual ~Base();

        void perform(Command::ID id);
        RealtimeAny get(Parameter::ID id) const;
        unsigned id() const;
        void processEntryPoint();
        Any property(Property::ID id) const;
        void set(Parameter::ID id, RealtimeAny value);
        void setProperty(Property::ID id, const Any& value);

    protected:
        Base();

        bool hasInternalBuffer();
        virtual void commandInit();
        void initVectors(Command::ID nCommands, Parameter::ID nParameters,
            Property::ID nProperties);
        void send(Signal::ID id, RealtimeAny value);
        Base* parent() const;
        virtual void process() = 0;
        virtual void processChildrenParallel() = 0;
        virtual void processChildrenSerial() = 0;
        virtual void processPre();
        virtual void processPost();

        const RuntimeInfo& runtime();

        template <typename D>
        void addCommand(Command::ID id, void (D::*handler)())
        {
            addCommand(id, static_cast<Handler>(handler));
        }

    private:
        typedef void (Base::*Handler)();

        std::vector<Handler> _commands;
        unsigned _id;
        bool _isInitialized;
        static unsigned _nextId;
        std::vector<Any> _properties;
        std::vector<RealtimeAny> _parameters;
        Base* _parent;

        void addCommand(Command::ID id, Handler handler);
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
