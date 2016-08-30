#ifndef SOUND_PROCESSOR_BASE_H
#define SOUND_PROCESSOR_BASE_H

#include <memory>
#include <vector>

#include "object.h"
#include "shared/allocator.h"
#include "shared/any.h"
#include "shared/id.h"
#include "shared/value.h"

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
        Value get(Parameter::ID id) const;
        unsigned id() const;
        void processEntryPoint();
        Any property(Property::ID id) const;
        void set(Parameter::ID id, Value value);
        void setProperty(Property::ID id, const Any& value);

    protected:
        Base();

        bool hasInternalBuffer();

        virtual void commandInit();
        virtual void commandExposeParameters();

        void initVectors(Command::ID nCommands, Parameter::ID nParameters,
            Property::ID nProperties);
        void emitSignal(Signal::ID signalId, Value value = Value::Null);
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

        void emitParamteter(Parameter::ID parameterId, Value value);
    private:
        typedef void (Base::*Handler)();

        std::vector<Handler> _commands;
        unsigned _id;
        bool _isInitialized;
        static unsigned _nextId;
        std::vector<Any> _properties;
        std::vector<Value> _parameters;
        Base* _parent;
        RuntimeInfo* _runtime;

        void addCommand(Command::ID id, Handler handler);
    };
}
}

#endif // SOUND_PROCESSOR_BASE_H
