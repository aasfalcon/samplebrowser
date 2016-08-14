#ifndef METER_H
#define METER_H

#include "processor.h"

template<typename T> class ContFrame;

template<typename T>
class Meter: public Processor<T>
{
public:
    Meter();
    ~Meter();
    const ConstFrame<T> peaks() const;
    void process();

protected:
    void init();

private:
    Buffer<T> _peaksBuffer;
};

SOUND_INSTANTIATE_DECLARATION(Meter)

#endif // METER_H
