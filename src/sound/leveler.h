#ifndef LEVELER_H
#define LEVELER_H

#include "processor.h"

template<typename T>
class Leveler: public Processor<T>
{
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

#include "leveler.tcc"

#endif // LEVELER_H
