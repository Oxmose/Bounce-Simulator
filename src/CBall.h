/**
 *
 * @file CBall.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 09/10/2014
 *
 * @version : 2.0
 *
 * @brief CBall header file.
 *
 * @details Contain declaration of the class CBall.
 *
 * @see CBall.cpp
 *
 **/

#ifndef __CBALL_H__
#define __CBALL_H__

#include <vector>       // std::vector
#include <math.h>       // sin, cos...

#include "common.h"     // Settings struct, PI

// CBall class
class CBall
{
    public :
        // Will load the ball thanks to the parameters.
        CBall (const nsTools::Settings &Parameters);

        // Load the ball matrix.
        void Load (nsTools::Quality);

        // Will send the vertex array.
        std::vector <float> GetVertices ();

    private :
        // The vertex array.
        std::vector <float> m_Vertices;

};
#endif // CBALL_H_INCLUDED
