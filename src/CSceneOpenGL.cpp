/**
 *
 * @file CSceneOpenGl.cpp
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief CSceneOpenGl source file
 *
 * @details Contain the implementation of the CSceneOpenGl class.
 *
 * @see CSceneOpenGl.h
 *
 **/

#include <GL/glew.h>    // GLEW Lib
#include <SDL2/SDL.h>   // SDL Lib
#include <string>       // std::string
#include <iostream>     // std::cout
#include <utility>      // std::pair
#include <vector>       // std::vector
#include <ctime>        // std::time

/* GLEW INCLUDES START */
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* GLEW INCLUDES END */

#include "Shader.h"         // Shaders class
#include "CSceneOpenGL.h"   // SceneOpenGL class
#include "tools.h"          // Tools (Matrix tranformation, computing functions)
#include "common.h"         // Settings structure
#include "CGrad.h"          // CGrad class
#include "CBall.h"          // CBall class


using namespace glm;

// Initialize the CSceneOpenGl class that contains them main part of the program.
CSceneOpenGL::CSceneOpenGL (std::string WindowTitle, int WindowWidth, int WindowHeight) : m_WindowTitle (WindowTitle), m_WindowWidth (WindowWidth),
                                                                                          m_WindowHeight (WindowHeight), m_Window (0), m_OpenGlContext (0)
{

}

// Destroy the CSceneOpenGl class.
CSceneOpenGL::~CSceneOpenGL ()
{
    // Delete OpenGL context, destroy the window it was into, quit the SDL library.
    SDL_GL_DeleteContext (m_OpenGlContext);
    SDL_DestroyWindow (m_Window);
    SDL_Quit ();
}

// Initialize the window that will be the OpenGl context.
bool CSceneOpenGL::InitWindow ()
{
    // Initialize the SDL library
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        // Display the error.
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError () << std::endl;

        // Quit the SDL context.
        SDL_Quit ();

        return false;
    }


    // Set the maximum and minimum OpenGl version.
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Enable the double buffer and the depth field size.
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute (SDL_GL_ACCELERATED_VISUAL, 1);

    //Activate anti aliasing.
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);


    // Create the window.
    m_Window = SDL_CreateWindow (m_WindowTitle.c_str (), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    // If the initialization of the window failed.
    if (m_Window == 0)
    {
        // Display the error.
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError () << std::endl;

        // Quit
        SDL_Quit ();

        return false;
    }


    // Creating OpenGl context.
    m_OpenGlContext = SDL_GL_CreateContext (m_Window);

    // If failed to create the context
    if (m_OpenGlContext == 0)
    {
        // Display the error.
        std::cout << SDL_GetError () << std::endl;

        // Destroy the created window and quit.
        SDL_DestroyWindow (m_Window);
        SDL_Quit ();

        return false;
    }

    // Initialization went well.
    return true;
}

// Initialize the GLEW LIB
bool CSceneOpenGL::InitGL ()
{

    // Initialize Glew32 Library.
    GLenum InitGLEW (glewInit ());


    // If the step failed.
    if (InitGLEW != GLEW_OK)
    {
        // Display error.
        std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString (InitGLEW) << std::endl;

        // Remove the context, then the window and quit the SDL library.
        SDL_GL_DeleteContext (m_OpenGlContext);
        SDL_DestroyWindow (m_Window);
        SDL_Quit ();

        return false;
    }

    // Enable depth field.
    glEnable (GL_DEPTH_TEST);

    return true;
}

// Contain the main operation we will use in the project.
void CSceneOpenGL::MainLoop ()
{
    // Set the variable to true to enter activate the infinite loop.
    bool Exit = false;

    // Tells the iteration number of the loop.
    unsigned Iter = 0;

    // Set framerate (change the second number to set it : 1000/50 = 50 FPS.
    unsigned int FPS = 50;
    unsigned int FrameRate (1000 / FPS);

    // Manage framerate.
    Uint32 LoopBeginning (0);
    Uint32 LoopEnding (0);
    Uint32 ElapsedTime (0);

    // Create the struct that contains trajectory variables.
    struct nsTools::Settings Settings;

     /*SPEED TESTS*/
    /*Settings.Speed = 20;
    Settings.Angle = 45 * PI / 180.0;
    Settings.InitPos = 20;
    Settings.Gravity = 9.81;
    Settings.RestitutionCoef = .85;
    Settings.Qual = nsTools::EXTRA;*/


    // Ask the user to enter his parameters to create the simulation
    Settings = nsTools::SetParameters ();

    // Set the last parameters.
    Settings.Time = 0;
    Settings.TotalTime = 0;
    Settings.Dir = nsTools::LEFTTORIGHT;

    // Save settings to start again.
    struct nsTools::Settings SaveSettings = Settings;

    // Create the ball.
    CBall Ball (Settings);

    // Get the vertex array.
    std::vector <float> VerticesTemps = Ball.GetVertices ();
    unsigned ArraySize = VerticesTemps.size ();

    float vertices [ArraySize];
    float couleurs [ArraySize];

    // Initialize random seed.
    srand (time (NULL));

    // Get the vertex of the ball.
    for (unsigned i = 0; i < ArraySize; ++i)
    {
        float RandomNumber = rand() % 255;
        vertices [i] = VerticesTemps [i];
        couleurs [i] = RandomNumber / 255.0;
    }
    // Activate shader.
    Shader ColorShader ("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    ColorShader.Load ();


    // Create "view" matrix.
    mat4 Projection;
    mat4 Modelview;

    // Set the angle of the camera and other settings such as near and far information.
    Projection = perspective (70.0, (double) m_WindowWidth / m_WindowHeight, 1.0, 10000.0);

    // Set the  first camera settings.
    Modelview = lookAt (vec3 (1, 1, 1), vec3 (0, 0, 0), vec3 (0, 1, 0));

    // To manage collision on X axis.
    float OldX = 0.0;

    // Old an new point (center of our ball).
    std::pair <float, float> Old;
    std::pair <float, float> New;

    // Tells if it the first time running the loop.
    bool FirstLoop = true;

    // Manage is simulation is pause or not.
    bool Paused = false;

     // Get the current angle to manage speed variation.
    float CurrentAngle = 0;

    // Set the position of the camera.
    float PosCam[3] = {38.4, 26.8, 41};
    float LookAtPos[3] = {PosCam[0], PosCam[1], 0};
    bool CamAngle[3] = {false, false, false};

    while (! Exit)
    {
        //Get the tick at loop beginning.
        LoopBeginning = SDL_GetTicks ();

        /*
        ** EVENT MANAGEMENT
        ** MANAGE THE DIFFERENT ALLOWED EVENTS
        ** START
        */
            // Event management
            SDL_PollEvent (&m_Event);

            // If user clicked the cross button, quit.
            if (m_Event.window.event == SDL_WINDOWEVENT_CLOSE)
                Exit = true;
            // If user pressed space bar and wanted the simulation to stop/start again.
            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_SPACE)
            {
                // If already paused
                if (Paused)
                {
                    // If object stopped, restart the simulation
                    if (Settings.Speed < 0.000001)
                    {
                        // Set back the attributes at their initial position.
                        FirstLoop = true;
                        Settings = SaveSettings;
                        OldX = 0.0;
                        Iter = 0;

                        // Puts the camera back to her initial position.
                        PosCam[0] = 38.4;
                        PosCam[1] = 26.8;
                        PosCam[2] = 41;
                        LookAtPos[0] = PosCam[0];
                        LookAtPos[1] = PosCam[1];
                        LookAtPos[2] = 0;
                        CamAngle[0] = false;
                        CamAngle[1] = false;
                        CamAngle[2] = false;

                        // Puts the ball back to the first position
                        nsTools::Translate (vertices, ArraySize, Old, std::make_pair (0, 0), 0.0);
                    }

                    DisplayInformation (Settings, CurrentAngle, New);

                    // Restart
                    Paused = false;
                }
                else
                {
                    DisplayInformation (Settings, CurrentAngle, New);
                    std::cout << std::endl << "Simulation en pause." << std::endl;
                    Paused = true;
                }

            }
            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_UP)
            {

                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0), -0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = PosCam[2];*/
                }
                else if (CamAngle[1])
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.5));
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
                else
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.5));
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }

            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_DOWN)
            {
                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.0), 0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = PosCam[2];*/
                }
                else if (CamAngle[1])
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, -0.5));
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
                else
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, -0.5));
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }

            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_LEFT)
            {
                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.0), 0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = PosCam[2];*/
                }
                else if (CamAngle[1])
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0), 0.5);
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = PosCam[2];
                }
                else
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (-0.5, 0));
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }


            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_RIGHT)
            {
                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.0), 0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = PosCam[2];*/
                }
                else if (CamAngle[1])
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0), -0.5);
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = PosCam[2];
                }
                else
                {
                    nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0.5, 0));
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }

            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_s)
            {
                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, -0.1), 0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = 0;*/
                }

                if (CamAngle[1])
                {
                    nsTools::Resize(PosCam, 3, 1.01, 1.01, 1.01);
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
                else
                {
                    nsTools::Resize(PosCam, 3, 1.01, 1.01, 1.01);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }

            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_z)
            {
                if (CamAngle[0])
                {
                    /*nsTools::Translate(PosCam, 3, std::make_pair (0, 0), std::make_pair (0, 0.0), 0.5);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = PosCam[2];*/
                }

                else if (CamAngle[1])
                {
                    nsTools::Resize(PosCam, 3, 0.99, 0.99, 0.99);
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
                else
                {
                    nsTools::Resize(PosCam, 3, 0.99, 0.99, 0.99);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                }
            }

            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_e)
            {

                if (CamAngle[1])
                {
                    nsTools::Rotate(PosCam, 3, 0.0, -PI / 4, 0.0);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                    CamAngle[1] = false;
                }
                else
                {
                    nsTools::Rotate(PosCam, 3, 0.0, PI / 4, 0.0);
                    LookAtPos[0] = 0;
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                    CamAngle[1] = true;
                }

            }
            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_d)
            {
                if (CamAngle[0])
                {
                    nsTools::Rotate(PosCam, 3, PI / 3, -0.0, 0.0);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = PosCam[1];
                    LookAtPos[2] = 0;
                    CamAngle[0] = false;
                }
                else
                {
                    nsTools::Rotate(PosCam, 3, - PI / 3, 0.0, 0.0);
                    LookAtPos[0] = PosCam[0];
                    LookAtPos[1] = 0;
                    LookAtPos[2] = 0;
                    CamAngle[0] = true;
                }
            }


            else if (m_Event.key.type == SDL_KEYDOWN && m_Event.key.keysym.sym == SDLK_c)
                nsTools::Rotate(PosCam, 3, 0.1, 0.0, 0.0);



        /*
        ** EVENT MANAGEMENT
        ** MANAGE THE DIFFERENT ALLOWED EVENTS
        ** END
        */
        // If user didn't paused the simulation.
        if (! Paused)
        {
            // Set the total time of the simulation
            Settings.TotalTime += 1.0 / (float) FPS;

            /*
            ** TRAJECTORY COMPUTING
            ** CONTAIN ANGLE COMPUTING, COLLISION DETECTION AND TRAJECTORY ANALYSIS
            ** START
            */

                // If no one point were created.
                if (FirstLoop)
                    CurrentAngle = Settings.Angle;

                // Else if we can create the vector with old and new point to get the angle.
                else
                    CurrentAngle = nsTools::AngleComputing (Old, New, false, Settings.Dir);


                // Compute the point at t = t.
                Old = nsTools::PositionComputing (Settings, OldX);

                // Add time
                Settings.Time += 1.0 / (float) FPS;

                // Compute the point at t = t + 1 / FPS.
                New = nsTools::PositionComputing (Settings, OldX);


                /*
                ** COLLISION DETECTION
                ** START
                */

                    /* RIGHT WALL */
                    if (nsTools::CollisionDetectionBorder (New.first) == 2)
                    {
                        /*
                        ** COMPUTE THE NEW TRAJECTORY EVERY TIME THE BALL TOUCH A BORDER
                        */

                        // Compute the new speed of the ball.
                        Settings.Speed =  Settings.Speed * Settings.RestitutionCoef;

                        // ANGLE COMPUTING //

                        // If ball going down.
                        if (New.second < Old.second)
                            Settings.Angle = nsTools::AngleComputing (Old, New, true, nsTools::RIGHTTOLEFT) - PI;
                        // If ball going up.
                        else
                            Settings.Angle = nsTools::AngleComputing (Old, New, true, nsTools::RIGHTTOLEFT);

                        // ANGLE COMPUTING END //

                        // Get the new initial position.
                        Settings.InitPos = Old.second;

                        // Reset time for the new trajectory.
                        Settings.Time = 0;

                        // We have to change direction, touched right border, then go to the left.
                        Settings.Dir = nsTools::RIGHTTOLEFT;

                        // X maximum value is the value just after touching the right wall.
                        OldX = Old.first;
                    }

                    /* LEFT WALL */
                    if (nsTools::CollisionDetectionBorder (New.first) == 1)
                    {
                        /*
                        ** COMPUTE THE NEW TRAJECTORY EVERY TIME THE BALL TOUCH A BORDER
                        */

                        // ANGLE COMPUTING //

                        // Compute the new speed of the ball.
                        Settings.Speed =  Settings.Speed * Settings.RestitutionCoef;

                        //If ball going down
                        if (New.second < Old.second)
                            Settings.Angle = nsTools::AngleComputing (Old, New, true, nsTools::LEFTTORIGHT) - PI;
                        //If ball going up.
                        else
                            Settings.Angle = nsTools::AngleComputing (Old, New, true, nsTools::LEFTTORIGHT);

                        // ANGLE COMPUTING END //

                        // Get the new initial position.
                        Settings.InitPos = Old.second;

                        // Reset time for the new trajectory.
                        Settings.Time = 0;

                        // We have to change direction, touched left border, then go to the right.
                        Settings.Dir = nsTools::LEFTTORIGHT;

                        // X minimum value is the value just after touching the left wall.
                        OldX = 0;
                    }

                    /* BOTTOM FLOOR */
                    if (nsTools::CollisionDetectionBottomTop (New.second) == 1)
                    {
                        /*
                        ** COMPUTE THE NEW TRAJECTORY EVERY TIME THE BALL TOUCH THE FLOOR
                        */

                        // Compute the new speed of the ball.
                        Settings.Speed =  Settings.Speed * Settings.RestitutionCoef;

                        // Get the new angle of the ball.
                        Settings.Angle = nsTools::AngleComputing (Old, New, false, Settings.Dir);

                        // Get the new initial position.
                        Settings.InitPos = 0;

                        // Reset time for the new trajectory.
                        Settings.Time = 0;

                        // Use OldX as last bouncing point to avoid the ball from doing strange things.
                        OldX = New.first;
                    }

                    /* TOP ROOF */
                    if (nsTools::CollisionDetectionBottomTop (New.second) == 2)
                    {
                        /*
                        ** COMPUTE THE NEW TRAJECTORY EVERY TIME THE BALL TOUCH THE FLOOR
                        */

                        // Compute the new speed of the ball.
                        Settings.Speed =  Settings.Speed * Settings.RestitutionCoef;

                        // Get the new angle of the ball.
                        Settings.Angle = -nsTools::AngleComputing (Old, New, false, Settings.Dir);

                        // Get the new initial position.
                        Settings.InitPos = Old.second;

                        // Reset time for the new trajectory.
                        Settings.Time = 0;

                        // Use OldX as last bouncing point to avoid the ball from doing strange things.
                        OldX = New.first;
                    }

                    // Compute the new point, just after bouncing on the floor.
                    New = nsTools::PositionComputing (Settings, OldX);

                /*
                ** COLLISION DETECTION
                ** END
                */

            /*
            ** TRAJECTORY COMPUTING
            ** CONTAIN ANGLE COMPUTING, COLLISION DETECTION AND TRAJECTORY ANALYSIS
            ** END
            */

            /*
            ** DISPLAY OPENGL VIEW, PLACE POINTS AND MATRIX
            ** DISPLAY CONSOLE INFORMATION
            ** START
            */
                // Translate the object
                // If firstloop, put the object at the right place.
                if (FirstLoop)
                    nsTools::Translate (vertices, ArraySize, std::make_pair (0, 0), Old, 0.0);

                // Else move it.
                else
                    nsTools::Translate (vertices, ArraySize, Old, New, 0.0);

                // Clear the window view and the depth buffer.
                glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Set the camera settings.c
                Modelview = lookAt (vec3 (PosCam[0], PosCam[1], PosCam[2]), vec3 (LookAtPos[0], LookAtPos[1], LookAtPos[2]), vec3 (0, 1, 0));


                // Tell which shader to use.
                glUseProgram (ColorShader.GetProgramID ());

                // Activate and fill the vertex array of the object.
                glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
                glEnableVertexAttribArray (0);

                // Activate and fill the vertex colors array of the object.
                glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);
                glEnableVertexAttribArray (1);

                // Send the two "view" matrix to the shader.
                glUniformMatrix4fv (glGetUniformLocation (ColorShader.GetProgramID (), "modelview"), 1, GL_FALSE, value_ptr (Modelview));
                glUniformMatrix4fv (glGetUniformLocation (ColorShader.GetProgramID (), "projection"), 1, GL_FALSE, value_ptr (Projection));

                /* PRINT THE VERTEX MATRIX ON THE SCREEN */

                // Print the object
                glDrawArrays (GL_TRIANGLE_STRIP, 0, ArraySize / 3);


                // Create the graduation
                // ON X BOTTOM AXIS
                CGrad GraduationX (0.5, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag");

                float *GradMat = GraduationX.GetMat();
                nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (-1.4, -1.5));


                for (unsigned i = 0; i < 80; ++i)
                {
                    GraduationX.Display (Projection, Modelview);
                    GradMat = GraduationX.GetMat();
                    nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (1, 0));
                    GraduationX.SetMat (GradMat);
                }

                // ON X TOP AXIS
                nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (-79, 56.5));
                for (unsigned i = 0; i < 79; ++i)
                {
                    GraduationX.Display (Projection, Modelview);
                    GradMat = GraduationX.GetMat();
                    nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (1, 0));
                    GraduationX.SetMat (GradMat);
                }

                //ON Y LEFT AXIS
                CGrad GraduationY (0.5, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
                GradMat = GraduationY.GetMat();
                nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (-1.4, -1.0));
                for (unsigned i = 0; i < 57; ++i)
                {
                    GraduationY.Display (Projection, Modelview);
                    GradMat = GraduationY.GetMat();
                    nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (0, 1));
                    GraduationY.SetMat (GradMat);
                }

                //ON Y RIGHT AXIX
                nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (79.4, -57));
                for (unsigned i = 0; i < 57; ++i)
                {
                    GraduationY.Display (Projection, Modelview);
                    GradMat = GraduationY.GetMat();
                    nsTools::Translate(GradMat, 108, std::make_pair (0, 0), std::make_pair (0, 1));
                    GraduationY.SetMat (GradMat);
                }

                // Disable unused shader.
                glUseProgram (0);


                // Refresh the window view.
                SDL_GL_SwapWindow (m_Window);

                /* PRINT THE SIMULATION INFORMATION ON THE CONSOLE*/

                // Display information every 3 seconds.
                if (Iter % FPS * 300 == 0)
                {
                    Iter = 0;
                    DisplayInformation (Settings, CurrentAngle, New);
                }


            /*
            ** DISPLAY OPENGL VIEW, PLACE POINTS AND MATRIX
            ** DISPLAY CONSOLE INFORMATION
            ** END
            */

            /*
            ** FPS MANAGEMENT / MISC
            ** START
            */

                // Get time elapsed during the current loop.
                LoopEnding = SDL_GetTicks ();
                ElapsedTime = LoopEnding - LoopBeginning;

                // If too quick, wait.
                if (ElapsedTime < FrameRate)
                    SDL_Delay (FrameRate - ElapsedTime);

                // If it was the first time looping
                FirstLoop = false;

                // Set iter ti iter + 1
                ++Iter;

            /*
            ** FPS MANAGEMENT / MISC
            ** START
            */
        }
    }

    // Simulation ended.
    std::cout << std::endl << "Simulation terminée." << std::endl;
}
