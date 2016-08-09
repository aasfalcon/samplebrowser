#include <string>
#include <cstring>

#include "version.h"

Version::Version()
    : _code(0)
{
}

Version::Version(const std::string &sver)
    : Version(sver.c_str())
{
}

Version::Version(const char *csver)
    : Version()
{
    unsigned length = strlen(csver) + 1;
    char buffer[length];
    unsigned parts[PartCount];

    memset(parts, 0, sizeof parts);
    memcpy(buffer, csver, length);
    unsigned count = 0;

    char *token = strtok(buffer, ".");

    while (token && count < PartCount) {
        parts[count++] = std::stoul(token);
        token = strtok(NULL, ".");
    }

    for (unsigned i = PartMajor; i < PartCount; i++) {
        _code |= parts[i] << (i << 4);
    }
}

int Version::compare(Version::ver_t left, Version::ver_t right)
{
    signed long long int diff = left - right;

    if (diff < 0) {
        return -1;
    } else if (diff > 0) {
        return 1;
    }

    return 0;
}

unsigned Version::get(Part part) const {
    return 0xFFFF & (_code >> ((unsigned)part << 0x4));
}

bool Version::pass(Version::Compare compare, const Version &right)
{
    return  (compare == CompareExact                && *this == right)
            || (compare == CompareGreater           && *this > right)
            || (compare == CompareGreaterOrEqual    && *this >= right)
            || (compare == CompareLower             && *this < right)
            || (compare == CompareLowerOrEqual      && *this <= right)
            ;
}

void Version::set(Part part, unsigned value) {
    unsigned shift = part << 0x4;
    _code &= ~(0xFFFFULL << shift);
    _code |= (ver_t)value << shift;
}

std::string Version::sign(Version::Compare compare)
{
    const char *signs[CompareCount] = {
        ">=", ">", "<=", "<", "=",
    };

    return signs[compare];
}

std::string Version::str() const
{
    std::string result
            = std::to_string(get(PartMajor)) + '.'
            + std::to_string(get(PartMinor)) + '.'
            + std::to_string(get(PartPatch));

    unsigned build = get(PartBuild);

    if (build) {
        result += '.' + std::to_string(build);
    }

    return result;
}

bool Version::operator >(const Version &right) const {
    return compare(_code, right._code) > 0; }
bool Version::operator >=(const Version &right) const {
    return compare(_code, right._code) >= 0; }
bool Version::operator <(const Version &right) const {
    return compare(_code, right._code) < 0; }
bool Version::operator <=(const Version &right) const {
    return compare(_code, right._code) <= 0; }
bool Version::operator ==(const Version &right) const {
    return compare(_code, right._code) == 0; }
bool Version::operator !=(const Version &right) const {
    return compare(_code, right._code) != 0; }
