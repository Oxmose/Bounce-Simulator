/**
 *
 * @file CBall.cpp
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 09/10/2014
 *
 * @version : 2.0
 *
 * @brief CBall header file.
 *
 * @details Contain the implementation of the class CBall.
 *
 * @see CBall.h
 *
 **/

#include <vector>       // std::vector
#include <math.h>       // sin, cos...

#include "CBall.h"      // Class header
#include "common.h"     // Settings struct, PI

using namespace std;
using namespace nsTools;

// Will load the ball thanks to the parameters.
CBall::CBall (const Settings &Parameters)
{
    if (Parameters.Qual == LOW)
        Load (LOW);
    else if (Parameters.Qual == HIGH)
        Load (HIGH);
    else if (Parameters.Qual == EXTRA)
        Load (EXTRA);
    else
        Load (MEDIUM);
} // CBall ()

// Load the ball matrix.
void CBall::Load (Quality Q)
{
    /*
    **
    ** WE ARE USING A MODIFIED VERSION OF THE ORANGE-PEEL METHOD TO DRAW THE SPHERE
    ** MORE INFORMATION HERE : https://www.google.fr/search?q=SpiralSphere&oq=SpiralSphere&aqs=chrome..69i57j0l5.412j0j4&sourceid=chrome&es_sm=122&ie=UTF-8
    ** AND HERE : http://www.gamedev.net/topic/350823-rendering-a-sphere-using-triangle-strips/#entry3297098
    ** OR HERE : http://gamedev.stackexchange.com/questions/31308/algorithm-for-creating-spheres
    ** WE DID NOT CREATED THIS METHOD, AND WE RECEIVED SOME HELP TO IMPLEMENTS IT
    **
    */

    // Clear the vector
    m_Vertices.clear ();

    // Latitude faces number
    int SegmenNumber;
    // Longitude faces number
    int PointsNumbers;

    // Manage the quality of the ball (the number of segments and point that compose it.
    if (Q == EXTRA)
    {
        SegmenNumber = 1000;
        PointsNumbers = 30;
    }
    else if (Q == HIGH)
    {
        SegmenNumber = 500;
        PointsNumbers = 30;
    }
    else if (Q == LOW)
    {
        SegmenNumber = 50;
        PointsNumbers = 5;
    }
    else
    {
        SegmenNumber = 100;
        PointsNumbers = 10;
    }


    // Create the matrix of the ball.
    for (int DivSegments = 0; DivSegments <= SegmenNumber; DivSegments++)
    {
        // Sets the angle
        float Angle = (float) PI * DivSegments / SegmenNumber;

        for(int DivPi = 0; DivPi <= PointsNumbers; DivPi++)
        {
            // Create the points
            float b = ((float)DivPi * 2 * (float) PI / (float) PointsNumbers);
            float XPoint = (float) sin(Angle) * (float) cos(b);
            float YPoint = (float) cos(Angle);
            float ZPoint = (float) sin(Angle) * (float) sin(b);

            // Add the point to create the triangle.
            m_Vertices.push_back (XPoint);
            m_Vertices.push_back (YPoint);
            m_Vertices.push_back (ZPoint);
        }
    }

}// Load ()

// Will send the vertex array.
vector <float> CBall::GetVertices ()
{
    return m_Vertices;
}// GetVertices ()
