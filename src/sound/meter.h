#ifndef METER_H
#define METER_H

#include "processor.h"

template <typename T>
class ConstFrame;

template <typename T>
class Meter : public Processor<T> {
public:
    Meter();
    ~Meter();
    ConstFrame<T> peaks() const;
    void process();

protected:
    void init();

private:
    Buffer<T> _peaks;
};

SOUND_INSTANTIATION_DECLARE(Meter);

#endif // METER_H
