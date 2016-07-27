#include <QCryptographicHash>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QDataStream>

#include <sndfile.hh>

#include "config.h"
#include "scanner.h"

#include <wav2png/src/wav2png.hpp>
#include <wav2png/src/wav2png.cpp>

Scanner::Scanner(QObject *parent)
    : Helper(parent)
    , _color(WEE_SCANNER_FOREGROUND_DEFAULT)
    , _height(WEE_SCANNER_HEIGHT)
    , _lineOnly(WEE_SCANNER_LINE_ONLY_DEFAULT)
    , _width(WEE_SCANNER_WIDTH)
{
    _cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(_cachePath);

    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    connect(this, &Scanner::performing,
            this, &Scanner::scanDir);
}

QByteArray Scanner::getHashData() const {
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream
            << _color
            << _lineOnly
            << _width
            << _height
               ;
    return result;
}

QString Scanner::getCacheFilename(QString const &in_file) const {
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(getHashData());
    hash.addData(in_file.toUtf8());
    return _cachePath + QDir::separator() + QString(hash.result().toHex()) + ".png";
}

bool Scanner::scan(const QString &in_file, const QString &out_file) const {
    QFileInfo in_info(in_file), out_info(out_file);

    if (!out_info.exists() || !out_info.isFile() || (out_info.exists() &&
            in_info.lastModified() > out_info.lastModified()))
    {
        SndfileHandle sample(in_file.toStdString(), SFM_READ);

        if (sample.error()) {
            return false;
        }

        png::image<png::rgba_pixel> image(_width, _height);
        int r, g, b;
        _color.getRgb(&r, &g, &b);

        compute_waveform(sample, image,
                         png::rgba_pixel(0, 0, 0),
                         png::rgba_pixel(r, g, b),
                         false, 0, 0, _lineOnly, 0);
        image.write(out_file.toUtf8());
    }

    return true;
}

void Scanner::clearCache() const {
    QDir dir(_cachePath);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    foreach (QString item, dir.entryList()) {
        dir.remove(item);
    }
}

void Scanner::scanDir(const QString &path) {
    QFileInfo info(path);

    if (info.exists() && info.isDir()) {
        QDir dir(path);
        dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

        foreach (QString in_file, dir.entryList()) {
            QString in_path = path + QDir::separator() + in_file;
            QString out_path = getCacheFilename(in_path);
            scan(in_path, out_path);

            if (_job != path || _isFinishing) {
                return;
            }
        }

        _job.clear();
    }
}


QString Scanner::scanFile(const QString &in_file) const {
    QString out_file = getCacheFilename(in_file);

    if (scan(in_file, out_file)) {
        return out_file;
    }

    return QString();
}
