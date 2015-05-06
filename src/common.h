/**
 *
 * @file common.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 1.0
 *
 * @brief Common header file.
 *
 * @details Contain the settings and common enums of the project.
 *
 * @see main.cpp
 *
 **/

#ifndef __COMMON_H__
#define __COMMON_H__

namespace nsTools
{
    // WINDOW WIDTH AND HEIGHT PARAMETERS.
    #define WINDOW_WIDTH    1024
    #define WINDOW_HEIGHT   720

    // PI value
    #define PI              3.1415926

    // The allowed ball qualities.
    typedef enum{LOW, MEDIUM, HIGH, EXTRA} Quality;

    // The allowed directions.
    typedef enum{LEFTTORIGHT, RIGHTTOLEFT} Direction;

    // Store the settings.
    struct Settings
    {
        float Speed;            //the speed of the ball
        float Angle;            //the angle the ball is being launched with.
        float InitPos;          //the initial position of the ball before being launched.
        float Time;             //the time elapsed since the ball was launched.
        float TotalTime;        //the time elapsed since the beginning of the simulation.
        float Gravity;          //the gravity the ball is attracted by.
        Direction Dir;          //tells is the ball is moving from the left to the right or from the right to the left.
        float RestitutionCoef;  //the coefficient the ball have when touching an object.
        Quality Qual;           //the number of vertices of the ball
    };
}
#endif // __COMMON_H__
