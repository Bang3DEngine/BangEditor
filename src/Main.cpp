#include "Bang/Application.h"

using namespace Bang;

int main(int argc, char **argv)
{
    Application app(argc, argv);
    app.CreateWindow();
    return app.MainLoop();
}
