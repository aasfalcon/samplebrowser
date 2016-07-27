#ifndef SCANNER_H
#define SCANNER_H

#include <QColor>

#include "helper.h"

class Scanner : public Helper
{
    Q_OBJECT
public:
    Scanner(QObject *parent);

    void clearCache() const;
    QString scanFile(const QString &in_file) const;

private:
    QColor _color;
    QString _cachePath;
    int _height;
    bool _lineOnly;
    int _width;

    QByteArray getHashData() const;
    QString getCacheFilename(QString const &in_file) const;
    bool scan(const QString &in_file, const QString &out_file) const;
    void scanDir(const QString &path);
};

#endif // SCANNER_H
