#ifndef METER_H
#define METER_H

#include "constframe.h"
#include "processor.h"

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

#include "meter.tcc"

#endif // METER_H
