#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QtGui/qcolor.h>

class Scanner
{
public:
    Scanner();
    typedef bool (*Progress)(int percent);

    void clearCache() const;
    void scanDir(const QString &in_dir) const;
    QString scanFile(const QString &in_file) const;

private:
    QColor _color;
    QString _cache_path;
    int _height;
    bool _line_only;
    int _width;

    QByteArray _getHashData() const;
    QString _getCacheFilename(QString const &in_file) const;
    bool _scan(const QString &in_file, const QString &out_file) const;
};

#endif // SCANNER_H
