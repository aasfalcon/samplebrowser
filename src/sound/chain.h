#ifndef CHAIN_H
#define CHAIN_H

#include <list>
#include <memory>

#include "processor.h"

namespace Sound {

template <typename T>
class Chain
    : public Processor<T>,
      public std::list<std::shared_ptr<Processor<T> > > {
public:
    Chain();
    ~Chain();

    void process();

protected:
    void init();
};

SOUND_INSTANTIATION_DECLARE(Chain)
}

#endif // CHAIN_H
