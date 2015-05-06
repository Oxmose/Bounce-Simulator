/**
 *
 * @file CGrad.cpp
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
 * @see CGrad.h
 *
 **/

/* GLEW INCLUDES START */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* GLEW INCLUDES END */

#include <string>       // std::string

#include "Shader.h"     // Shader header class
#include "CGrad.h"       // Grad header class

using namespace glm;
using namespace std;

// Create the object.
CGrad::CGrad (float Size, string const &VertexShader, string const &FragmentShader) : m_Shader(VertexShader, FragmentShader)
{
    // Load the shader.
    m_Shader.Load();

    // Divide size by two.
    Size /= 2;


    // Create the temporary vertex.
    float verticesTmp[] = {-Size, -Size, -Size,   Size, -Size, -Size,   Size, Size, -Size,     // Face 1
                           -Size, -Size, -Size,   -Size, Size, -Size,   Size, Size, -Size,     // Face 1

                           Size, -Size, Size,   Size, -Size, -Size,   Size, Size, -Size,       // Face 2
                           Size, -Size, Size,   Size, Size, Size,   Size, Size, -Size,         // Face 2

                           -Size, -Size, Size,   Size, -Size, Size,   Size, -Size, -Size,      // Face 3
                           -Size, -Size, Size,   -Size, -Size, -Size,   Size, -Size, -Size,    // Face 3

                           -Size, -Size, Size,   Size, -Size, Size,   Size, Size, Size,        // Face 4
                           -Size, -Size, Size,   -Size, Size, Size,   Size, Size, Size,        // Face 4

                           -Size, -Size, -Size,   -Size, -Size, Size,   -Size, Size, Size,     // Face 5
                           -Size, -Size, -Size,   -Size, Size, -Size,   -Size, Size, Size,     // Face 5

                           -Size, Size, Size,   Size, Size, Size,   Size, Size, -Size,         // Face 6
                           -Size, Size, Size,   -Size, Size, -Size,   Size, Size, -Size};      // Face 6


    // Create the temporary colors.
    float couleursTmp[] = {1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 1
                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 1

                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 2
                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 2

                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 3
                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 3

                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 4
                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 4

                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 5
                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 5

                           1, 1, 1,   1, 1, 1,   1, 1, 1,   // Face 6
                           1, 1, 1,   1, 1, 1,   1, 1, 1};  // Face 6


    // Save the temporary information.
    for(int i(0); i < 108; i++)
    {
        m_Vertex [i] = verticesTmp[i];
        m_Colours[i] = couleursTmp[i];
    }
}

// Display the square.
void CGrad::Display (glm::mat4 &projection, glm::mat4 &modelview)
{
    // Activate the shader.
    glUseProgram(m_Shader.GetProgramID());

        // Send the vertex.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_Vertex);
        glEnableVertexAttribArray(0);

        // Send the colors.
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, m_Colours);
        glEnableVertexAttribArray(1);

        // Send the view matrix.
        glUniformMatrix4fv(glGetUniformLocation(m_Shader.GetProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(m_Shader.GetProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

        // Render the view.
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Disable the array of vertex and colors.
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

    // Disable the shader.
    glUseProgram(0);

}// Display ()

// Set the vertex matrix.
void CGrad::SetMat (float *Mat)
{
    for (unsigned i = 0; i < 108; ++i)
        m_Vertex[i] = Mat[i];

}// SetMat ()

// Return the vertex matrix.
float* CGrad::GetMat (void)
{
    return m_Vertex;
}// GetMat ()

