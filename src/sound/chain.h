#ifndef CHAIN_H
#define CHAIN_H

#include <list>
#include <memory>
#include "processor.h"


template<typename T>
class Chain: public Processor<T>
{
public:
    typedef std::shared_ptr<Processor<T>> PProcessor;

    Chain();
    ~Chain();

    void add(unsigned id, PProcessor processor);
    bool has(unsigned id);
    void remove(unsigned id);
    void swap(unsigned first, unsigned second);
    virtual void process();
    std::list<unsigned> own() const;

protected:
    virtual void init();

private:
    typedef std::pair<unsigned, PProcessor> Tagged;
    typedef std::list<Tagged> TaggedList;

    bool _active;
    TaggedList _processors;

    typename TaggedList::iterator find(unsigned id);
};

#endif // CHAIN_H
