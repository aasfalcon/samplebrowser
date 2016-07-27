#include "meter.h"

Meter::Meter(QWidget *parent) : QVUMeter(parent)
{
    setMinValue(0);
    setMaxValue(1);
}
