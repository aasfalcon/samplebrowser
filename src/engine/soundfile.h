#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QString>
#include <QVector>

#include "buffer.h"

/**
 * @brief Simple in-memory soundfile
 */
class SoundFile
{
public:
    SoundFile();
    SoundFile(const QString &filename);
    ~SoundFile();

    inline Buffer &buffer() { return this->_data; }

    void read(const QString &filename);
    void resample(int sampleRate);
    void resize(int frames);

    bool isEmpty() const;
    int sampleRate() const;
    int channels() const;
    int frames() const;

    struct TextInfo {
        QString name;
        QString format;
        QString channels;
        QString codec;
        QString duration;
        QString rate;
        QString size;
    };

    TextInfo info() const;
    static TextInfo readInfo(const QString &filename);

private:
    Buffer _data;
    QString _filename;
    struct SF_INFO *_info;

    static QMap<int, QString> _Map;
    static QString _GetFormatString(int index);
};

#endif // SOUNDFILE_H
