/**
 *
 * @file CGrad.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 09/10/2014
 *
 * @version : 2.0
 *
 * @brief CGrad header file.
 *
 * @details Contain the declaration of the class CGrad.
 *
 * @see CGrad.cpp
 *
 **/

#ifndef __GRAD_H__
#define __GRAD_H__


/* GLEW INCLUDES START */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* GLEW INCLUDES END */

#include <string>       // std::string

#include "Shader.h"     // Shader header class

// CGrad class, display square.
class CGrad
{
    public:
        // Create the object.
        CGrad (float Size, std::string const &VertexShader, std::string const &FragmentShader);

        // Display the square.
        void Display (glm::mat4 &projection, glm::mat4 &modelview);

        // Set the vertex matrix.
        void SetMat (float *Mat);

        // Return the vertex matrix.
        float *GetMat (void);


    private:
        // Shader
        Shader m_Shader;

        // Vertex matrix.
        float m_Vertex [108];
        // Color matrix.
        float m_Colours [108];
};
#endif // __GRAD_H__
