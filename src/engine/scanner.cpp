#include <QCryptographicHash>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QDataStream>

#include <sndfile.hh>

#include "config.h"
#include "scanner.h"

// the whole vendor wav2png source
#include "../vendor/wav2png/src/wav2png.cpp"

Scanner::Scanner()
    : _color(WEE_SCANNER_FOREGROUND_DEFAULT)
    , _height(WEE_SCANNER_HEIGHT)
    , _line_only(WEE_SCANNER_LINE_ONLY_DEFAULT)
    , _width(WEE_SCANNER_WIDTH)
{
    this->_cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(this->_cache_path);

    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }
}

QByteArray Scanner::_getHashData() const {
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream
            << this->_color
            << this->_line_only
            << this->_width
            << this->_height
               ;
    return result;
}

QString Scanner::_getCacheFilename(QString const &in_file) const {
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(this->_getHashData());
    hash.addData(in_file.toUtf8());
    return this->_cache_path + QString(hash.result().toHex()) + ".png";
}

bool Scanner::_scan(const QString &in_file, const QString &out_file) const {
    QFileInfo in_info(in_file), out_info(out_file);

    if (!in_info.exists() || !in_info.isFile() || (out_info.exists() &&
            in_info.lastModified() > out_info.lastModified()))
    {
        SndfileHandle sample(in_file.toStdString(), SFM_READ);

        if (sample.error()) {
            return false;
        }

        png::image<png::rgba_pixel> image(this->_width, this->_height);
        int r, g, b;
        this->_color.getRgb(&r, &g, &b);

        compute_waveform(sample, image,
                         png::rgba_pixel(0, 0, 0),
                         png::rgba_pixel(r, g, b),
                         false, 0, 0, this->_line_only, 0);
        image.write(out_file.toUtf8());
    }

    return true;
}

void Scanner::clearCache() const {
    QDir dir(this->_cache_path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    foreach (QString item, dir.entryList()) {
        dir.remove(item);
    }
}

void Scanner::scanDir(const QString &in_dir) const {
    QDir dir(in_dir);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    foreach (QString in_file, dir.entryList()) {
        QString out_file = this->_getCacheFilename(in_file);
        this->_scan(in_file, out_file);
    }
}


QString Scanner::scanFile(const QString &in_file) const {
    QString out_file = this->_getCacheFilename(in_file);

    if (this->_scan(in_file, out_file)) {
        return out_file;
    }

    return QString();
}
