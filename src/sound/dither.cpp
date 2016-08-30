#include "dither.h"

using namespace Sound;

std::random_device Dither::_device;
std::mt19937_64 Dither::_generator(Dither::_device());
std::uniform_real_distribution<Precise> Dither::_pdis(0, 1);
std::uniform_real_distribution<Precise> Dither::_ndis(-1, 0);
