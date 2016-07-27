#ifndef HELPER_H
#define HELPER_H

#include <QThread>
#include <QVariant>

class Helper : public QThread {
    Q_OBJECT
public:
    Helper(QObject *parent, bool isRepeating = false);
    ~Helper();

    void perform(const QString &job);
    int period() const;
    void repeat();
    void setPeriod(int value);

protected:
    QString _job;
    bool _isFinishing;
    bool _isRepeating;
    int _period;

    void finish();
    void run();

signals:
    void performing(QString job);
    void repeating();
};

#endif // HELPER_H
