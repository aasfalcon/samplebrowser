#include "helper.h"
#include "config.h"

Helper::Helper(QObject *parent, bool isRepeating)
    : QThread(parent)
    , _isFinishing(false)
    , _isRepeating(isRepeating)
    , _period(WEE_HELPER_SLEEP_TIME)
{
    start();
}

Helper::~Helper() {
    finish();
}

void Helper::finish() {
    _isFinishing = true;
    wait();
}

void Helper::perform(const QString &job) {
    _job = job;
    _isRepeating = false;
}

int Helper::period() const {
    return _period;
}

void Helper::repeat() {
    _job.clear();
    _isRepeating = true;
}

void Helper::run() {
    return;
    while (!_isFinishing) {
        if (_isRepeating) {
            emit repeating();
        } else if (!_job.isNull()) {
            emit performing(_job);
        }

        usleep(_period);
    }
}

void Helper::setPeriod(int value) {
    _period = value;
}
