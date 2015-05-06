/**
 *
 * @file Shader.h
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief Shader class header file.
 *
 * @details Contain definitions of the Shader class.
 *
 * @see Shader.cpp
 *
 **/

#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>    // Glew Lib
#include <iostream>     // std::cout
#include <string>       // std::string
#include <fstream>      // std::stream

// Shader class
class Shader
{
    public:

        // Initialize the shader object (default).
        Shader ();
        // Initialize the shader object (copy).
        Shader (Shader const &ShaderToCopy);
        // Initialize the shader object (custom).
        Shader (const std::string &VertexSource, const std::string &FragmentSource);
        // Destroy the shader object.
        ~Shader ();


        // Operator = overload for the shader class.
        Shader& operator = (Shader const &ShaderToCopy);

        // Load the shader.
        bool Load ();

        // Compile the shader.
        bool Compile (GLuint &Shader, GLenum Type, std::string const &SourceFile);

        // Get the program ID.
        GLuint GetProgramID () const;

    private:


        // Vertex ID
        GLuint m_VertexID;


        // Fragment ID
        GLuint m_FragmentID;

        // Program ID
        GLuint m_ProgramID;

        // Vertex source
        std::string m_VertexSource;

        // Fragment source
        std::string m_FragmentSource;
};

#endif
