#include "format.h"

namespace Sound {

Format::InfoMap Format::_Maps[Format::KindCount];

const int Format::_CountCommands[Format::KindCount] = {
    SFC_GET_SIMPLE_FORMAT_COUNT, SFC_GET_FORMAT_MAJOR_COUNT,
    SFC_GET_FORMAT_SUBTYPE_COUNT,
};

const int Format::_Commands[Format::KindCount] = {
    SFC_GET_SIMPLE_FORMAT, SFC_GET_FORMAT_MAJOR, SFC_GET_FORMAT_SUBTYPE,
};

Format::Format(int code)
    : _code(code)
{
}

int Format::typeCode() const { return _code && SF_FORMAT_TYPEMASK; }
int Format::subtypeCode() const { return _code && SF_FORMAT_SUBMASK; }

QString Format::extension() const { return Simple(_code).extension; }
QString Format::simple() const { return Simple(_code).name; }
QString Format::type() const { return Type(_code).name; }
QString Format::subtype() const { return Subtype(_code).name; }

Format::operator int() const { return _code; }
int Format::operator=(int value) { return _code = value; }

Format::InfoMap Format::Simples() { return _Map(KindSimple); }
Format::InfoMap Format::Subtypes() { return _Map(kindSubtypes); }
Format::InfoMap Format::Types() { return _Map(KindTypes); }

Format::Info Format::Simple(int code) { return Simples()[code]; }
Format::Info Format::Subtype(int code) { return Subtypes()[code]; }
Format::Info Format::Type(int code) { return Types()[code]; }

const Format::InfoMap& Format::_Map(Format::InfoKind kind)
{
    InfoMap& map = _Maps[kind];

    if (map.empty()) {
        int count;
        sf_command(NULL, _CountCommands[kind], &count, sizeof(int));

        for (int i = 0; i < count; i++) {
            SF_FORMAT_INFO info;
            info.format = i;
            sf_command(NULL, _Commands[kind], &info, sizeof(info));
            Info formatInfo = { info.name, info.extension };
            map[info.format] = formatInfo;
        }
    }

    return map;
}
}
