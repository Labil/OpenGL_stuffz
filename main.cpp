#include "SceneOpenGL.h"

int main(int argc, char *argv[])
{

    SceneOpenGL scene("Synesthesia", 1000, 800);

    if(scene.initializeWindow() == false)
    {
        return -1;
    }
    if(scene.initGL() == false)
    {
        return -1;
    }

    scene.mainLoop();

    return 0;
}
