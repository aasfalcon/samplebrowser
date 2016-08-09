#ifndef VERSION_H
#define VERSION_H

#include <string>

class Version {
public:
    enum Part {
        PartBuild = 0,
        PartPatch,
        PartMinor,
        PartMajor,
        PartCount
    };

    enum Compare {
        CompareGreaterOrEqual = 0,
        CompareGreater,
        CompareLowerOrEqual,
        CompareLower,
        CompareExact,
        CompareCount
    };

    Version();
    Version(const std::string &sver);
    Version(const char *csver);

    unsigned get(Part part) const;
    bool pass(Compare compare, const Version &right);
    void set(Part part, unsigned value);
    static std::string sign(Compare compare);
    std::string str() const;

    bool operator >(const Version &right) const;
    bool operator >=(const Version &right) const;
    bool operator <(const Version &right) const;
    bool operator <=(const Version &right) const;
    bool operator ==(const Version &right) const;
    bool operator !=(const Version &right) const;

private:
    typedef unsigned long long int ver_t;
    ver_t _code;

    static int compare(ver_t left, ver_t right);
};

#endif // VERSION_H
