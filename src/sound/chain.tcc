#include <algorithm>

#include "chain.h"

template<typename T>
Chain<T>::Chain()
{

}

template<typename T>
Chain<T>::~Chain()
{

}

template<typename T>
void Chain<T>::addProcessor(unsigned id,
                   std::shared_ptr<Processor<T> > processor)
{
    if (_active) {
        processor->kickIn(this->out(), this->in(),
                          this->latency(), this->sampleRate());
    }

    _processors.push_back(std::make_pair(id, processor));
}

template<typename T>
typename Chain<T>::TaggedList::iterator
Chain<T>::find(unsigned id)
{
    auto findTag = [id](const Tagged &pair) { return pair.first == id; };
    auto it = std::find_if(_processors.begin(),
                           _processors.end(),
                           findTag);
    return it;
}

template<typename T>
bool Chain<T>::hasProcessor(unsigned id)
{
    return find(id) != _processors.end();
}

template<typename T>
void Chain<T>::init()
{
    _active = true;


    for (auto it = _processors.begin(); it != _processors.end(); ++it) {
        it->second->kickIn(this->out(), this->in(),
                           this->latency(), this->sampleRate());
    }
}

template<typename T>
void Chain<T>::removeProcessor(unsigned id)
{
    _processors.erase(find(id));
}

template<typename T>
void Chain<T>::swapProcessors(unsigned first,
                    unsigned second)
{
    find(first)->swap(*find(second));
}

template<typename T>
void Chain<T>::process()
{
    for (auto it = _processors.begin(); it != _processors.end(); ++it) {
        it->second->process();
    }
}

template<typename T>
std::list<unsigned> Chain<T>::processors() const
{
    std::list<unsigned> result;

    for (auto it = _processors.begin(); it != _processors.end(); ++it) {
        result.push_back(it->first);
    }

    return result;
}
