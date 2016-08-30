#ifndef SOUND_PROCESSOR_MODEL_H
#define SOUND_PROCESSOR_MODEL_H

#include <vector>

#include "watcher.h"

namespace Sound {
namespace Processor {

    class MessageBus;

    class Model : public Watcher {
    public:
        Model(MessageBus& bus, unsigned processorId);
        ~Model();

        Value get(Parameter::ID parameterId) const;
        bool isReady() const;
        void perform(Command::ID commandId) const;
        void set(Parameter::ID parameterId, Value value);

        void watch(const Event &event) override;

    private:
        MessageBus& _bus;
        bool _isReady;
        std::vector<Value> _parameters;
        std::vector<bool> _recieved;
        unsigned _processorId;
    };
}
}

#endif // SOUND_PROCESSOR_MODEL_H
