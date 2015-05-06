/**
 *
 * @file main.cpp
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief Common header file.
 *
 * @details Contain the settings main () of the project.
 *
 * @see main.cpp
 *
 **/

#include "CSceneOpenGL.h"   //Scene OpengGL
#include "common.h"         //Common macro

using namespace nsTools;

int main (int argc, char **argv)
{
    // Creating the scene.
    CSceneOpenGL Scene ("Bounce Simulator", WINDOW_WIDTH, WINDOW_HEIGHT);

    // If window initialization failed.
    if (Scene.InitWindow () == false)
        return -1;

    // If OpenGl context initialization failed.
    if (Scene.InitGL () == false)
        return -1;

    // Launch the main loop.
    Scene.MainLoop ();

    // Everything went well...
    return 0;
}
