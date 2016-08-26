#ifndef SOUND_PROCESSOR_SHORTCUTS_H
#define SOUND_PROCESSOR_SHORTCUTS_H

#ifndef PROCESSOR
#error "PROCESSOR identifier is not defined"
#endif

#define PROPERTY(a_type, a_name, a_value) \
    SOUND_REGISTER_PROPERTY(PROCESSOR, a_type, a_name, a_value)

#define PARAMETER(a_type, a_name, a_value) \
    SOUND_REGISTER_PARAMETER(PROCESSOR, a_type, a_name, a_value)

#define COMMAND(a_command) \
    SOUND_REGISTER_COMMAND(PROCESSOR, a_command)

#define INSTANTIATE \
    SOUND_INSTANTIATE(Sound::Processor::PROCESSOR)

#define INITIALIZE                  \
    this->initVectors(              \
        Command::PROCESSOR::_End,   \
        Parameter::PROCESSOR::_End,  \
        Property::PROCESSOR::_End);

#endif // SOUND_PROCESSOR_SHORTCUTS_H
