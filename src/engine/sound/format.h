#ifndef FORMAT_H
#define FORMAT_H

#include <QMap>

class QString;

namespace Sound {

class Format
{
public:
    struct Info {
        QString extension;
        QString name;
    };

    typedef QMap<int, Info> InfoMap;

    Format();
    Format(int code);

    int typeCode() const;
    int subtypeCode() const;

    QString extension() const;
    QString simple() const;
    QString type() const;
    QString subtype() const;

    operator int() const;
    int operator =(int value);

    static InfoMap Simples();
    static InfoMap Subtypes();
    static InfoMap Types();

    static Info Simple(int code);
    static Info Subtype(int code);
    static Info Type(int code);

private:
    enum InfoKind {
        KindSimple,
        kindSubtypes,
        KindTypes,
        KindCount
    };

    int _code;
    static const int _CountCommands[KindCount];
    static const int _Commands[KindCount];
    static InfoMap _Maps[KindCount];

    static const InfoMap &map(InfoKind kind);
};

} // namespace Sound

#endif // FORMAT_H
