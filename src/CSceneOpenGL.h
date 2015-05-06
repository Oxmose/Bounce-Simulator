/**
 *
 * @file CSceneOpenGL.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief CSceneOpenGL header file.
 *
 * @details Contain declaration of the class CSceneOpenGL.
 *
 * @see CSceneOpenGL.cpp
 *
 **/

#ifndef __CSCENEOPENGL_H__
#define __CSCENEOPENGL_H__

#include <SDL2/SDL.h>   // SDL Lib
#include <string>       // std::string
#include <iostream>     // std::cout
#include <utility>      // std::pair

/* GLEW INCLUDES START */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* GLEW INCLUDES END */

#include "Shader.h"         // Shader class
#include "CSceneOpenGL.h"   // SceneOpenGL class
#include "tools.h"          // Tools (Matrix transformation, computing functions)
#include "common.h"         // Settings structure
#include "CGrad.h"           // Grad class
#include "CBall.h"          // CBall class


/*
** CSceneOpenGl class that contains them main part of the program.
*/
class CSceneOpenGL
{
    public :

        // Initialize the CSceneOpenGl class.
        CSceneOpenGL (std::string WindowTitle, int WindowWidth, int WindowHeight);

        // Destroy the CSceneOpenGl class.
        ~CSceneOpenGL ();

        // Initialize the window that will be the OpenGl context.
        bool InitWindow ();

        // Initialize the GLEW LIB
        bool InitGL ();

        // Contain the main operation we will use in the project.
        void MainLoop ();

    private :

        // Contains the window title.
        std::string m_WindowTitle;

        // Contain the window width.
        int m_WindowWidth;

        // Contain the window height.
        int m_WindowHeight;

        // Pointer on the window.
        SDL_Window* m_Window;

        // OpenGl context.
        SDL_GLContext m_OpenGlContext;

        // SDL event variable.
        SDL_Event  m_Event;

};
#endif // __CSCENEOPENGL_H__
