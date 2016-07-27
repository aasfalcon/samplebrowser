#ifndef VERSION
#define VERSION

#include <string>
#include <sstream>
#include <vector>

class Version {
public:
    static int compare(unsigned left, unsigned right)
    {
        return (int)left - (int)right;
    }

    static int compare(const std::string &left, const std::string &right)
    {
        return compare(encode(left), encode(right));
    }

    static std::string decode(unsigned version)
    {
        return std::to_string(0xFF & (version >> 0x10)) + '.'
             + std::to_string(0xFF & (version >> 0x08)) + '.'
             + std::to_string(0xFF & version);

    }

    static unsigned encode(const std::string &version)
    {

        std::istringstream ss(version);
        std::vector<std::string> parts;
        std::string part;

        while(std::getline(ss, part, '.')) {
            parts.push_back(part);
        }

        unsigned result = 0;

        switch (parts.size()) {
            case 3: result |= std::stoi(parts[2]);
            case 2: result |= std::stoi(parts[1]) << 0x08;
            case 1: result |= std::stoi(parts[0]) << 0x10;
        }

        return result;
    }

};

#endif // VERSION
