#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QString>
#include <QVector>

/**
 * @brief Simple in-memory soundfile
 */
class SoundFile
{
public:
    SoundFile();
    SoundFile(const QString &filename);
    ~SoundFile();

    typedef double Sample;
    typedef QVector<Sample> Buffer;

    inline Buffer &buffer() { return this->_data; }

    void read(const QString &filename);
    void resample(int sampleRate);
    void resize(int frames);

    bool isEmpty() const;
    int sampleRate() const;
    int channels() const;
    int frames() const;

private:
    Buffer _data;
    QString _filename;
    struct SF_INFO *_info;
};

#endif // SOUNDFILE_H
