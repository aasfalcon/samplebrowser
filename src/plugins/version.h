#ifndef VERSION_H
#define VERSION_H

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

class Version {
public:
    typedef std::int64_t ver_t;

    static int compare(ver_t left, ver_t right)
    {
        ver_t diff = left - right;

        if (diff < 0) {
            return -1;
        } else if (diff > 0) {
            return 1;
        }

        return 0;
    }

    static int compare(const std::string &left, const std::string &right)
    {
        return compare(encode(left), encode(right));
    }

    static std::string decode(ver_t version, bool showBuild = false)
    {
        std::string result =
                std::to_string(0xFFFF & (version >> 0x30)) + '.'
                + std::to_string(0xFFFF & (version >> 0x20)) + '.'
                + std::to_string(0xFFFF & (version >> 0x10));

        if (showBuild) {
            result += '.' + std::to_string(0xFFFF & version);
        }

        return result;
    }

    static ver_t encode(const std::string &version)
    {

        std::istringstream ss(version);
        std::vector<std::string> parts;
        std::string part;

        while(std::getline(ss, part, '.')) {
            parts.push_back(part);
        }

        ver_t result = 0;

        switch (parts.size()) {
            case 4: result |= (ver_t)std::stoi(parts[3]);
            case 3: result |= (ver_t)std::stoi(parts[2]) << 0x10;
            case 2: result |= (ver_t)std::stoi(parts[1]) << 0x20;
            case 1: result |= (ver_t)std::stoi(parts[0]) << 0x30;
        }

        return result;
    }

};

#endif // VERSION_H
