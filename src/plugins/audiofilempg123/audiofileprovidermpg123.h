#ifndef AUDIOFILEPROVIDERMPG123_H
#define AUDIOFILEPROVIDERMPG123_H

#include "../audiofile/audiofileprovider.h"

struct mpg123_handle_struct;

class AudioFileProviderMPG123 : public AudioFileProvider
{
public:
    AudioFileProviderMPG123();
    ~AudioFileProviderMPG123();

    void close();
    void open(const char *filename, Mode mode);

    unsigned read(void *buffer, unsigned frames);
    unsigned seek(int offset, SeekWhence sw, SeekType st = SeekTypeDefault);

private:
    mpg123_handle_struct *_mpghandle;
    bool _mpgmode;

    std::string mpgError(const std::string &userMessage, int errorNumber = 0);
};

#endif // AUDIOFILEPROVIDERMPG123_H
