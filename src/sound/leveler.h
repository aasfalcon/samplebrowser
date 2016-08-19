#ifndef LEVELER_H
#define LEVELER_H

#include "processor.h"

namespace Sound {

template <typename T>
class Leveler : public Processor<T> {
public:
    Leveler();
    ~Leveler();
    double balance() const;
    double level() const;
    virtual void process();
    void setBalance(double value);
    void setLevel(double value);

private:
    double _balance;
    double _level;
};

SOUND_INSTANTIATION_DECLARE(Leveler);
}

#endif // LEVELER_H
