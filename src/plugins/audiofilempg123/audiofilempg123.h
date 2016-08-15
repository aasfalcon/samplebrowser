#ifndef AUDIOFILEMPG123_H
#define AUDIOFILEMPG123_H

#include "../audiofilesndfile/audiofilesndfile.h"

struct mpg123_handle_struct;

class AudioFileMPG123 : public AudioFileSndfile
{
public:
    AudioFileMPG123();
    ~AudioFileMPG123();

    void close();
    void open(const char *filename, Mode mode);

    unsigned read(void *buffer, unsigned frames);
    unsigned seek(int offset, SeekWhence sw, SeekType st = SeekTypeDefault);

private:
    mpg123_handle_struct *_mpghandle;
    bool _mpgmode;

    std::string mpgError(const std::string &userMessage, int errorNumber = 0);
};

#endif // AUDIOFILEMPG123_H
