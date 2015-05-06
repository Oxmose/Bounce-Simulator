/**
 *
 * @file tools.cpp
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief Tools source file.
 *
 * @details Contain definitions of the tools function used in the project.
 *
 * @see tools.h
 *
 **/

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
#include "tools.h"          // Tools
#include "common.h"         // struct

using namespace std;

// Will return the pair (x, y) coordinates of the center of the ball at time = t.
pair <float, float> nsTools::PositionComputing (const nsTools::Settings &Sets, float Xmax) throw ()
{
    // Initialize the two coordinates
    float X (0);
    float Y (0);

    // If the ball goes from left to the right
    if (Sets.Dir == LEFTTORIGHT)
        X = Xmax + Sets.Speed * cos (Sets.Angle) * Sets.Time;
    // If from right to the left
    else
        X = Xmax - Sets.Speed * cos (Sets.Angle) * Sets.Time;

    // Always the same from left to right and from right to left
    Y = Sets.InitPos + Sets.Speed * sin (Sets.Angle) * Sets.Time - (1.0 / 2.0 * Sets.Gravity * Sets.Time * Sets.Time);

    // Create and and return the brand new pair of coordinates.
    return make_pair (X, Y);

}// PositionComputing ()

// Will return the angle of the ball and the wall or floor just before the impact.
float nsTools::AngleComputing (pair <float, float> Point1, pair <float, float> Point2, bool Vertical, Direction Dir) throw ()
{
    // Create the vector from the trajectory.
    array <float, 2> V = { {Point2.first - Point1.first, Point2.second - Point1.second} };

    // Declare the vector of the wall or floor.
    array <float, 2> U;

    // If vertical movement
    if (Vertical)
    {
       // Create the vector parts of the second line.
        U[0] = 0;
        U[1] = 1;
    }
    else
    {
        //Create the vector parts of the second line.
        if (Dir == LEFTTORIGHT)
            U[0] = 1;
        else
            U[0] = -1;

        U[1] = 0;
    }

    // Get the cos (angle)
    float CosAngle = (V[0] * U[0] + V[1] * U[1]) / sqrt((V[0] * V[0] + V[1] * V[1]) * (U[0] * U[0] + U[1] * U[1]));

    // Return the Angle in radiant.
    return acos (CosAngle);

}//AngleComputing ()

// Will return the speed for X axis and Y axis
pair <float, float> nsTools::SpeedComputing (float InitSpeed, float Angle)
{
    // Create a pair of float the first for y, the second for x
    pair <float,float> Speed;

    Speed.first = InitSpeed * sin (Angle);
    Speed.second = InitSpeed * cos (Angle);

    // Return the speed for X axis and for the Y axis
    return Speed;
}

// Test is a string only contains digits.
bool nsTools::StringIsDigit (const stringstream &sstream) throw ()
{
    // Tells if we already encountered a comma.
    bool Comma = false;

    // Test if the number size is only one and if it's not a point.
    if (sstream.str ().size() == 1 && sstream.str ()[0] == '.')
        return false;
    else
        return true;

    // Test every characters.
    for (unsigned i = 0; i < (sstream.str ()).size (); ++i)
    {
        if (Comma == true && sstream.str ()[i] == '.')
            return false;

        if (! isdigit (sstream.str ()[i]) && sstream.str ()[i] != '.')
            return false;

        if (sstream.str ()[i] == '.')
            Comma = true;
    }

    // Strings seems to be a number, return true.
    return true;

}//StringIsDigit ()

// Set the variable struct that contain the environment parameters.
nsTools::Settings nsTools::SetParameters (void) throw ()
{
    //Creating the struct Parameters
    Settings Parameters;

    /*
    **
    ** FILLING THE STRUCT
    **
    */

    /*
    ** SPEED
    */

    cout << "Entrez la vitesse de la balle (en m/s) : ";
    stringstream UserSpeed;
    string SUserSpeed;
    cin >> SUserSpeed;
    UserSpeed.str (SUserSpeed);

    while (! StringIsDigit (UserSpeed))
    {
        cout << "Erreur: la valeur doit etre un chiffre positif entre 0 et 100." << endl;

        cout << "Entrez la vitesse de la balle (en m/s) : ";
        cin >> SUserSpeed;
        UserSpeed.str (SUserSpeed);
    }

    UserSpeed >> Parameters.Speed;
    if (Parameters.Speed > 100)
        Parameters.Speed = 100;

    /*
    ** ANGLE
    */

    cout << "Entrez l'angle de lancement de la balle (en degres) : ";
    stringstream UserAngle;
    string SUserAngle;
    cin >> SUserAngle;
    UserAngle.str (SUserAngle);

    while (! StringIsDigit (UserAngle))
    {
        cout << "Erreur: la valeur doit etre un chiffre positif" << endl;

        cout << "Entrez l'angle de lancement de la balle (en degres) : ";
        cin >> SUserAngle;
        UserAngle.str (SUserAngle);
    }
    UserAngle >> Parameters.Angle;

    // Test angle is between 0 and 360.
    if (Parameters.Angle > 360)
        Parameters.Angle = (int) Parameters.Angle % 360;
    if (Parameters.Angle < 0)
        Parameters.Angle = 0;

    // From deg to rad
    Parameters.Angle = Parameters.Angle * (float) PI / 180.0;

    /*
    ** INIT POSITION
    */

    cout << "Entrez la position initiale en y de la balle : ";
    stringstream UserPos;
    string SUserPos;
    cin >> SUserPos;
    UserPos.str (SUserPos);

    while (! StringIsDigit (UserPos))
    {
        cout << "Erreur: la valeur doit etre un chiffre" << endl;
        cout << "Entrez la position initial de la balle : ";
        cin >> SUserPos;
        UserPos.str (SUserPos);
    }
    UserPos >> Parameters.InitPos;

    if (Parameters.InitPos > 20)
    {
        Parameters.InitPos = 20;
    }

    /*
    ** GRAVITY
    */

    cout << "Entrez la force de gravite desire (en m/s) : ";
    stringstream UserGravity;
    string SUserGravity;
    cin >> SUserGravity;
    UserGravity.str (SUserGravity);

   while (! StringIsDigit (UserGravity))
    {
        if (! StringIsDigit (UserGravity))
            cout << "Erreur: la valeur doit etre un chiffre positif" << endl;

        cout << "Entrez la force de gravite desire (en m/s) : ";
        cin >> SUserGravity;
        UserGravity.str (SUserGravity);
    }
    UserGravity >> Parameters.Gravity;

    /*
    ** COEFFICIENT OF RESTITUTION
    */

    cout << "Entrez le coefficient de rebond de la balle : ";
    stringstream UserCoef;
    string SUserCoef;
    cin >> SUserCoef;
    UserCoef.str (SUserCoef);

   while (! StringIsDigit (UserCoef))
    {
        if (! StringIsDigit (UserCoef))
            cout << "Erreur: la valeur doit etre un chiffre positif" << endl;

        cout << "Entrez la force de gravite desire (en m/s) : ";
        cin >> SUserCoef;
        UserCoef.str (SUserCoef);
    }
    UserCoef >> Parameters.RestitutionCoef;

    /*
    ** BALL QUALITY
    */

    cout << "Entrez la qualite d'affichage de la balle (Low, Medium, High, Extra): ";
    string Test;
    cin >> Test;
    if (Test == "low")
        Parameters.Qual = LOW;
    else if (Test == "high")
        Parameters.Qual = HIGH;
    else if (Test == "extra")
        Parameters.Qual = EXTRA;
    else
        Parameters.Qual = MEDIUM;


    return (Parameters);

}// SetParameters ()


// autre fonction rotate
/*
// Rotate a matrix.
void nsTools::Rotate (float *Mat, unsigned MatSize, float Angle, float x, float y, float z) throw ()
{
   float RotateMat[4][4];


   RotateMat [0][0] = x*x*(1-cos(Angle))+cos(Angle);
   RotateMat [0][1] = x*y*(1-cos(Angle))-z*sin(Angle);
   RotateMat [0][2] = x*z*(1-cos(Angle))+y*sin(Angle);
   RotateMat [0][3] = 0;

   RotateMat [1][0] = x*y*(1-cos(Angle))+z*sin(Angle);
   RotateMat [1][1] = y*y*(1-cos(Angle))+cos(Angle);
   RotateMat [1][2] = y*z*(1-cos(Angle))-x*sin(Angle);
   RotateMat [1][3] = 0;

   RotateMat [2][0] = x*z*(1-cos(Angle))-y*sin(Angle);
   RotateMat [2][1] = y*z*(1-cos(Angle))+x*sin(Angle);
   RotateMat [2][2] = z*z*(1-cos(Angle))+cos(Angle);
   RotateMat [2][3] = 0;

   RotateMat [3][0] = 0;
   RotateMat [3][1] = 0;
   RotateMat [3][2] = 0;
   RotateMat [3][3] = 1;


   // Translate the matrice
    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertice)
        float OldMatPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points
        Multiply (OldMatPart, RotateMat);

        // Change the value of the coordinates in the matrix.
        Mat [i] = OldMatPart [0];
        Mat [i + 1] = OldMatPart [1];
        Mat [i + 2] = OldMatPart [2];
    }
}// Rotate ()   */


// Rotate a matrix.
void nsTools::Rotate (float *Mat, unsigned MatSize, float AngleX, float AngleY, float AngleZ) throw ()
{
    // Create the matrix to allow rotate on X axis by multiplying it to the old matrix.
    float RotateXMat [4][4];

     // Fill the X rotation matrix.
    RotateXMat [0][0] = 1;
    RotateXMat [0][1] = 0;
    RotateXMat [0][2] = 0;
    RotateXMat [0][3] = 0;

    RotateXMat [1][0] = 0;
    RotateXMat [1][1] = cos (AngleX);
    RotateXMat [1][2] = -sin (AngleX);
    RotateXMat [1][3] = 0;

    RotateXMat [2][0] = 0;
    RotateXMat [2][1] = sin (AngleX);
    RotateXMat [2][2] = cos (AngleX);
    RotateXMat [2][3] = 0;

    RotateXMat [3][0] = 0;
    RotateXMat [3][1] = 0;
    RotateXMat [3][2] = 0;
    RotateXMat [3][3] = 1;

    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertex).
        float OldMatXPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points.
        Multiply (OldMatXPart, RotateXMat);

        //Change the value of the coordinates in the matrix.
        Mat [i] = OldMatXPart [0];
        Mat [i + 1] = OldMatXPart [1];
        Mat [i + 2] = OldMatXPart [2];
    }

    // Create the matrix to allow rotate on Y axis by multiplying it to the old matrix.
    float RotateYMat [4][4];

    // Fill the Y rotation matrix.
    RotateYMat [0][0] = cos (AngleY);
    RotateYMat [0][1] = 0;
    RotateYMat [0][2] = sin (AngleY);
    RotateYMat [0][3] = 0;

    RotateYMat [1][0] = 0;
    RotateYMat [1][1] = 1;
    RotateYMat [1][2] = 0;
    RotateYMat [1][3] = 0;

    RotateYMat [2][0] = -sin (AngleY);
    RotateYMat [2][1] = 0;
    RotateYMat [2][2] = cos (AngleY);
    RotateYMat [2][3] = 0;

    RotateYMat [3][0] = 0;
    RotateYMat [3][1] = 0;
    RotateYMat [3][2] = 0;
    RotateYMat [3][3] = 1;

    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertex)
        float OldMatYPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points.
        Multiply (OldMatYPart, RotateYMat);

        // Change the value of the coordinates in the matrix.
        Mat [i] = OldMatYPart [0];
        Mat [i + 1] = OldMatYPart [1];
        Mat [i + 2] = OldMatYPart [2];
    }

    // Create the matrix to allow rotate on Z axis by multiplying it to the old matrix.
    float RotateZMat [4][4];

    // Fill the Z rotation matrix
    RotateZMat [0][0] = cos (AngleZ);
    RotateZMat [0][1] = -sin (AngleZ);
    RotateZMat [0][2] = 0;
    RotateZMat [0][3] = 0;

    RotateZMat [1][0] = sin (AngleZ);
    RotateZMat [1][1] = cos (AngleZ);
    RotateZMat [1][2] = 0;
    RotateZMat [1][3] = 0;

    RotateZMat [2][0] = 0;
    RotateZMat [2][1] = 0;
    RotateZMat [2][2] = 1;
    RotateZMat [2][3] = 0;

    RotateZMat [3][0] = 0;
    RotateZMat [3][1] = 0;
    RotateZMat [3][2] = 0;
    RotateZMat [3][3] = 1;

    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertex)
        float OldMatZPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points
        Multiply (OldMatZPart, RotateZMat);

        // Change the value of the coordinates in the matrix.
        Mat [i] = OldMatZPart [0];
        Mat [i + 1] = OldMatZPart [1];
        Mat [i + 2] = OldMatZPart [2];
    }
}// Rotate ()

// Translate a matrix.
void nsTools::Translate (float *Mat, unsigned MatSize, std::pair <float, float> OldCenterPosition, std::pair <float, float> NewCenterPosition, float ZMovement/* = 0*/) throw ()
{
    // Give the movement on X an Y to do the job on the matrix.
    float XMovement = NewCenterPosition.first - OldCenterPosition.first;
    float YMovement = NewCenterPosition.second - OldCenterPosition.second;

    // Create the matrix to allow to translate by multiplying it to the old matrix.
    float TransMat [4][4];

    // Fill the translation matrix
    TransMat [0][0] = 1;
    TransMat [0][1] = 0;
    TransMat [0][2] = 0;
    TransMat [0][3] = XMovement;

    TransMat [1][0] = 0;
    TransMat [1][1] = 1;
    TransMat [1][2] = 0;
    TransMat [1][3] = YMovement;

    TransMat [2][0] = 0;
    TransMat [2][1] = 0;
    TransMat [2][2] = 1;
    TransMat [2][3] = ZMovement;

    TransMat [3][0] = 0;
    TransMat [3][1] = 0;
    TransMat [3][2] = 0;
    TransMat [3][3] = 1;

    // Translate the matrice
    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertice)
        float OldMatPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points
        Multiply (OldMatPart, TransMat);

        // Change the value of the coordinates in the matrix.
        Mat [i] = OldMatPart [0];
        Mat [i + 1] = OldMatPart [1];
        Mat [i + 2] = OldMatPart [2];
    }

}// Translate ()

void nsTools::Resize (float *Mat, unsigned MatSize, float XFactor, float YFactor, float ZFactor) throw ()
{
    // Create the martix to allow to resize by multiplying it to the old matrix.
    float ResMatrix [4][4];

    // Fill the resize matrix.
    ResMatrix [0][0] = XFactor;
    ResMatrix [0][1] = 0;
    ResMatrix [0][2] = 0;
    ResMatrix [0][3] = 0;

    ResMatrix [1][0] = 0;
    ResMatrix [1][1] = YFactor;
    ResMatrix [1][2] = 0;
    ResMatrix [1][3] = 0;

    ResMatrix [2][0] = 0;
    ResMatrix [2][1] = 0;
    ResMatrix [2][2] = ZFactor;
    ResMatrix [2][3] = 0;

    ResMatrix [3][0] = 0;
    ResMatrix [3][1] = 0;
    ResMatrix [3][2] = 0;
    ResMatrix [3][3] = 1;

    // Resize the matrix.
    for (unsigned i = 0; i < MatSize; i += 3)
    {
        // Take a part of the old mat (a vertice)
        float OldMatPart [4] = {Mat [i], Mat [i + 1], Mat [i + 2], 1};

        // Apply the transformation on the points
        Multiply (OldMatPart, ResMatrix);

        // Change the value of the coordinates in the matrix.
        Mat [i] = OldMatPart [0];
        Mat [i + 1] = OldMatPart [1];
        Mat [i + 2] = OldMatPart [2];
    }

}// Resize ()

// Multiply two matrix.
void nsTools::Multiply (float *MatToMul, float TransFormMat[4][4]) throw ()
{
    // Create a temporary matrix not to loose information
    float TempsMat [4];
    TempsMat [0] = MatToMul [0];
    TempsMat [1] = MatToMul [1];
    TempsMat [2] = MatToMul [2];
    TempsMat [3] = MatToMul [3];

    MatToMul [0] = 0;
    MatToMul [1] = 0;
    MatToMul [2] = 0;
    MatToMul [3] = 0;

    // Multiply columns by lines
    for (unsigned i = 0; i < 4; ++i)
    {
        for (unsigned j = 0; j < 4; ++j)
        {
            MatToMul [i] = MatToMul [i] + TempsMat [j] * TransFormMat [i][j];
        }
    }
}// Multiply ()

// Collision detection on the two walls.
unsigned nsTools::CollisionDetectionBorder (float XObj) throw ()
{
    // If left
    if (XObj < 0)
        return 1;
    //If right
    if (XObj > 77)
        return 2;

    // Else
    return 0;

}// CollisionDetectionBorder ()

// Collision detection on the floor and roof.
unsigned nsTools::CollisionDetectionBottomTop (float YObj) throw ()
{
    // If bottom
    if (YObj < 0)
        return 1;
    // If top
    if (YObj > 54)
        return 2;

    // Else
    return 0;

}// CollisionDetectionBottom ()

// Will display information about the simulation.
void nsTools::DisplayInformation (Settings Parameters, float CurrentAngle, std::pair <float, float> New)
{
    // Clear console
    system ("cls");

    // Get speed on axis
    std::pair <float, float> SpeedAxis = SpeedComputing(Parameters.Speed, CurrentAngle);

    // Display
    std::cout << "Temps : " << Parameters.TotalTime << std::endl
              << "Angle : " << 180 / PI * CurrentAngle << std::endl
              << "Vitesse : " << Parameters.Speed << std::endl
              << "Coordonnees : " << New.first << ", " << New.second << std::endl
              << "Gravitee : " << Parameters.Gravity << std::endl
              << "Coefficient de restitution a l'impact : " << Parameters.RestitutionCoef << std::endl
              << "Vitesse sur l'axe X : " << SpeedAxis.first << std::endl
              << "Vitesse sur l'axe Y : " << SpeedAxis.second << std::endl;

}// DisplayInformation
