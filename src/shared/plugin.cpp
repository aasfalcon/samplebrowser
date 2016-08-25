#define PLUGIN_NAME
#define PLUGIN_DESCRIPTION
#define PLUGIN_VERSION

#include <cstring>
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

Interface *Plugin::create(const char *interfaceName)
{
    return Allocator<Interface>::create(interfaceName);
}

void Plugin::destroy(Interface *object)
{
    Allocator<Interface>::destroy(object);
}

const IPlugin::Info *Plugin::info()
{
    if (!_info.interfaces) {
        if (!_tags) {
            _tags = std::make_shared<std::list<std::string> >(tags());
        }

        _info.interfaceCount = _tags->size();

        if (_info.interfaceCount) {
            _info.interfaces = new const char *[_info.interfaceCount];
            unsigned index = 0;

            for (auto it = _tags->begin(); it != _tags->end(); it++) {
                _info.interfaces[index++] = it->c_str();
            }
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
