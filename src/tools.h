/**
 *
 * @file tools.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief Tools header file.
 *
 * @details Contain declaration of the tools function used in the project.
 *
 * @see tools.cpp
 *
 **/

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <SDL2/SDL.h>   // SDL
#include <string>       // std::string
#include <iostream>     // std::cout
#include <math.h>       // cos, sin
#include <utility>      // std::pairs
#include <sstream>      // std::stringstream
#include <array>        // std::array

/* GLEW INCLUDES START */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* GLEW INCLUDES END */

#include "Shader.h"         // Shaders class
#include "CSceneOpenGL.h"   // Scene OpenGL
#include "common.h"         // struct

namespace nsTools
{
    // Will return the pair (x, y) coordinates of the center of the ball at time = t.
    std::pair <float, float> PositionComputing (const Settings &Sets, float Xmax) throw ();

    // Will return the angle of the ball and the wall or floor just before the impact.
    float AngleComputing (std::pair <float, float> Point1, std::pair <float, float> Point2, bool Vertical, Direction Dir) throw ();

    // Will return the speed for X axis and Y axis
    std::pair <float, float> SpeedComputing (float InitSpeed, float Angle);

    // Test is a string only contains digits.
    bool StringIsDigit (const std::stringstream &sstream) throw ();

    // Set the variable struct that contain the environment parameters.
    Settings SetParameters (void) throw ();

    // Translate a matrix.
    void Translate (float *Mat, unsigned MatSize, std::pair <float, float> OldCenterPosition, std::pair <float, float> NewCenterPosition, float ZMovement = 0) throw ();

    // Rotate a matrix.
    void Rotate (float *Mat, unsigned MatSize, float AngleX, float AngleY, float AngleZ) throw ();

    // Resize a matrix.
    void Resize (float *Mat, unsigned MatSize, float XFactor, float YFactor, float ZFactor) throw ();

    // Multiply two matrix.
    void Multiply (float *MatToMul, float TransMat[4][4]) throw ();

    // Collision detection on the two walls.
    unsigned CollisionDetectionBorder (float XObj) throw ();

    // Collision detection on the floor.
    unsigned CollisionDetectionBottomTop (float YObj) throw ();

    // Will display information about the simulation.
    void DisplayInformation (Settings parameter, float CurrentAngle, std::pair <float, float> New);
}
#endif // __TOOLS_H__
