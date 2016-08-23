template <typename EnumT, typename BaseEnumT>
class InheritEnum {
public:
    InheritEnum() {}
    InheritEnum(EnumT value)
        : _value(value)
    {
    }
    InheritEnum(BaseEnumT value)
        : _baseValue(value)
    {
    }

    operator EnumT() const
    {
        return _value;
    }

private:
    union {
        EnumT _value;
        BaseEnumT _baseValue;
    };
};

namespace Command {

//struct Base {
//    enum BaseEnum {
//        Base_Begin,
//        Base_End = Base_Begin,
//    };
//};

//struct Processor : Base {
//    enum ProcessorEnum {
//        Processor_Begin = End,
//        Init,
//        Process,
//        Processor_End,
//    };
//};

enum class Base : unsigned {
    _Begin,
    _End = _Begin,
};

enum class Processor : unsigned {
    _Begin = unsigned(Base::_End),

    Init,
    Process,

    _End,
};

#define SOUND_PROCESSOR_COMMANDS(a_scope, a_base, ...) \
    namespace Command {                                \
        enum class a_scope : unsigned {                \
            _Begin = unsigned(a_base::_End),           \
            __VA_ARGS__,                               \
            _End,                                      \
        };                                             \
    }

//enum class Resampler : unsigned {
//    _Begin = unsigned(Processor::_End),

//    Start,
//    Stop,

//    _End,
//};
}

SOUND_PROCESSOR_COMMANDS(Resampler, Processor,
    Start,
    Stop);

SOUND_PROCESSOR_COMMANDS(Player, Resampler,
    Play);

#include <cassert>
template <class E>
void command(E commandId)
{
    static_assert(E::_Begin != E::_End, "Command ID is not enum or empty");
    unsigned cr = static_cast<unsigned>(commandId);
}

void test()
{
    Command::Player c = Command::Player::Play;
    command(Command::Resampler::Stop);
    command(c);
}
