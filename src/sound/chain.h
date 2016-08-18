#ifndef CHAIN_H
#define CHAIN_H

#include <list>
#include <memory>

#include "processor.h"

template <typename T>
class Chain : public Processor<T> {
public:
    Chain();
    ~Chain();

    void addProcessor(unsigned id, std::shared_ptr<Processor<T> > processor);
    bool hasProcessor(unsigned id);
    void removeProcessor(unsigned id);
    void swapProcessors(unsigned first, unsigned second);
    virtual void process();
    std::list<unsigned> processors() const;

protected:
    virtual void init();

private:
    typedef std::pair<unsigned, std::shared_ptr<Processor<T> > > Tagged;
    typedef std::list<Tagged> TaggedList;

    bool _active;
    TaggedList _processors;

    typename TaggedList::iterator find(unsigned id);
};

SOUND_INSTANTIATION_DECLARE(Chain)

#endif // CHAIN_H
