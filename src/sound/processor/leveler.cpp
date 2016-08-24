#include "leveler.h"

#define PROCESSOR Leveler
#include "shortcuts.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Leveler<T>::Leveler()
{
    USE_PARAMETERS;
    PARAMETER(double, Balance, 0);
    PARAMETER(double, Level, 1);
}

template <typename T>
Leveler<T>::~Leveler()
{
}

template <typename T>
void Leveler<T>::process()
{
    auto& out = this->output();
    double balance = this->get(Parameter::Leveler::Balance);
    double level = this->get(Parameter::Leveler::Level);
    unsigned channels = out.channels();

    for (auto frame = out.begin(); frame != out.end(); ++frame) {
        for (unsigned i = 0; i < channels; i++) {

            if (balance != 0. && channels == 2) {
                if (balance > 0 && i == 0) {
                    level *= 1 - balance;
                } else if (balance < 0 && i == 1) {
                    level *= 1 + balance;
                }
            }

            frame[i] = T(double(T(frame[i])) * level);
        }
    }
}

INSTANTIATE;
