#include <stdexcept>
#include "constframe.h"
#include "frame.h"
#include "sample.h"
#include "leveler.h"

template<typename T>
Leveler<T>::Leveler()
    : Processor<T>()
    , _balance(0.0)
    , _level(1.0)
{
}

template<typename T>
Leveler<T>::~Leveler()
{
}

template<typename T>
double Leveler<T>::level() const
{
    return _level;
}

template<typename T>
void Leveler<T>::process()
{
    auto &out = *this->out();
    unsigned channels = out.channels();

    for (auto frame = out.begin(); frame != out.end(); ++frame) {
        for (unsigned i = 0; i < channels; i++) {
            double level = _level;

            if (_balance != 0. && channels == 2) {
                if (_balance > 0 && i == 0) {
                    level *= 1 - _balance;
                } else if (_balance < 0 && i == 1) {
                    level *= 1 + _balance;
                }
            }

            T sample = T(level * double(frame.at(i)));
            frame.put(i, sample);
        }
    }
}

template<typename T>
void Leveler<T>::setBalance(double value)
{
    if (value < -1 || value > 1) {
        throw std::out_of_range("Balance value out of range");
    }

    _balance = value;
}

template<typename T>
void Leveler<T>::setLevel(double value)
{
    if (value < 0) {
        throw std::out_of_range("Level value out of range");
    }

    _level = value;
}

SOUND_INSTANTIATE(Leveler)
