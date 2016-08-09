#define PLUGIN_NAME
#define PLUGIN_DESCRIPTION
#define PLUGIN_VERSION

#include <cstring>
#include <sstream>
#include "plugin.h"


Plugin::Plugin(const char *name, const char *version, const char *description)
    : _info({description, 0, NULL, name, version})
{
}

Plugin::~Plugin()
{
    if (_info.interfaces) {
        delete _info.interfaces;
    }
}

const IPlugin::Info *Plugin::info()
{
    if (!_info.interfaces) {
        auto names = tags();
        _info.interfaceCount = names.size();

        if (_info.interfaceCount) {
            _info.interfaces = new const char *[_info.interfaceCount];
            memcpy(_info.interfaces, names.data(),
                   _info.interfaceCount * sizeof(const char *));
        }
    }

    return &_info;
}

bool Plugin::provides(const char *interfaceName)
{
    const Info *pi = info();

    for (unsigned i = 0; i < pi->interfaceCount; i++) {
        if (strcmp(interfaceName, pi->interfaces[i]) == 0) {
            return true;
        }
    }

    return false;
}
