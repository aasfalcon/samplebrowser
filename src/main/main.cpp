#include "application.h"

#include "shared/log.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    return app.run();
}
