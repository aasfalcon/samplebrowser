#include <ltdl.h>
#include <stdexcept>

#include "application.h"
#include "shared/log.h"

int main(int argc, char* argv[])
{
    int result = -1;
    lt_dlinit();

    try {
        Application app(argc, argv);
        result = app.run();
    } catch (std::exception e) {
        LOG(FATAL, QTR("Unhandled exception: ") << e.what());
    } catch (...) {
        LOG(FATAL, QTR("Unhandled unknown object thrown"));
    }

    lt_dlexit();

    return result;
}
