/**
 *
 * @file Shader.cpp
 *
 * @authors : G. Tricaud, R. Soulier, G. Vigneau, A. Torres Aurora Dugo
 *
 * @date : 04/10/2014
 *
 * @version : 2.0
 *
 * @brief Shader class source file.
 *
 * @details Contain implementation of the Shader class.
 *
 * @see Shader.h
 *
 **/

#include <GL/glew.h>    // Glew Lib
#include <iostream>     // std::cout
#include <string>       // std::string
#include <fstream>      // std::stream

#include "Shader.h"     // Class header file

// Initialize the shader object (default).
Shader::Shader () : m_VertexID (0), m_FragmentID (0), m_ProgramID (0), m_VertexSource (), m_FragmentSource () {}

// Initialize the shader object (copy).
Shader::Shader(Shader const &ShaderToCopy)
{
    // Copying file sources.
    m_VertexSource = ShaderToCopy.m_VertexSource;
    m_FragmentSource = ShaderToCopy.m_FragmentSource;

    // loading the new shader.
    Load ();
}

// Initialize the shader object (custom).
Shader::Shader (const std::string &VertexSource, const std::string &FragmentSource) : m_VertexID (0), m_FragmentID (0), m_ProgramID (0),
                                                                                      m_VertexSource (VertexSource), m_FragmentSource (FragmentSource) {}

// Destroy the shader object.
Shader::~Shader ()
{
    // Destroying shader.
    glDeleteShader (m_VertexID);
    glDeleteShader (m_FragmentID);
    glDeleteProgram (m_ProgramID);
}


// Operator = overload for the shader class.
Shader& Shader::operator = (Shader const &ShaderToCopy)
{
    // Copying file sources.
    m_VertexSource = ShaderToCopy.m_VertexSource;
    m_FragmentSource = ShaderToCopy.m_FragmentSource;

    // Loading new shader.
    Load ();

    // Return the object itself;
    return *this;
}


bool Shader::Load ()
{
    // If these was an old shader, destroy it.
    if (glIsShader (m_VertexID) == GL_TRUE)
        glDeleteShader (m_VertexID);

    if (glIsShader (m_FragmentID) == GL_TRUE)
        glDeleteShader (m_FragmentID);

    if (glIsProgram (m_ProgramID) == GL_TRUE)
        glDeleteProgram (m_ProgramID);


    // Compiling the shader.
    if(! Compile (m_VertexID, GL_VERTEX_SHADER, m_VertexSource))
        return false;

    if(! Compile (m_FragmentID, GL_FRAGMENT_SHADER, m_FragmentSource))
        return false;


    // Create the program.
    m_ProgramID = glCreateProgram ();


    // Associate the shader to it.
    glAttachShader (m_ProgramID, m_VertexID);
    glAttachShader (m_ProgramID, m_FragmentID);


    // Lock shader input.
    glBindAttribLocation (m_ProgramID, 0, "in_Vertex");
    glBindAttribLocation (m_ProgramID, 1, "in_Color");
    glBindAttribLocation (m_ProgramID, 2, "in_TexCoord0");


    // Link the program.
    glLinkProgram (m_ProgramID);


    // Verify if linked.
    GLint ErrorLink (0);
    glGetProgramiv (m_ProgramID, GL_LINK_STATUS, &ErrorLink);


    // If there is an error.
    if (ErrorLink != GL_TRUE)
    {
        // Get the error size.
        GLint ErrorSize (0);
        glGetProgramiv (m_ProgramID, GL_INFO_LOG_LENGTH, &ErrorSize);


        // Dynamic memory management
        char *Error = new char[ErrorSize + 1];


        // Get error information
        glGetShaderInfoLog (m_ProgramID, ErrorSize, &ErrorSize, Error);
        Error[ErrorSize] = '\0';


        // Display error
        std::cout << Error << std::endl;


        // Free the memory
        delete[] Error;
        glDeleteProgram (m_ProgramID);

        return false;
    }
    // If not any error, all is ok.
    else
        return true;
}


// Compile the shader.
bool Shader::Compile (GLuint &Shader, GLenum Type, std::string const &SourceFile)
{
    // Creating the shader.
    Shader = glCreateShader (Type);

    // If shader was wrongly created.
    if (Shader == 0)
    {
        std::cout << "Erreur, le type de shader (" << Type << ") n'existe pas" << std::endl;
        return false;
    }

    // Open the file.
    std::ifstream File (SourceFile.c_str ());


    // If failed to open the file.
    if (! File)
    {
        std::cout << "Erreur le fichier " << SourceFile << " est introuvable" << std::endl;
        glDeleteShader (Shader);

        return false;
    }

    // Buffer used to read the file.
    std::string Line;
    std::string SourceCode;

    // Reading line by line.
    while (getline (File, Line))
        SourceCode += Line + '\n';

    // Close the file.
    File.close ();

    // Get a const char* from string.
    const GLchar* CharSourceCode = SourceCode.c_str ();

    // Send source code to the shader.
    glShaderSource (Shader, 1, &CharSourceCode, 0);

    // Compiling shader.
    glCompileShader (Shader);


    // Test if compilation went well.
    GLint CompilationError (0);
    glGetShaderiv (Shader, GL_COMPILE_STATUS, &CompilationError);


    // If something went wrong.
    if(CompilationError != GL_TRUE)
    {
        // Get error size.
        GLint ErrorSize (0);
        glGetShaderiv (Shader, GL_INFO_LOG_LENGTH, &ErrorSize);


        // Dynamic memory allocation.
        char *Error = new char[ErrorSize + 1];


        // Get the error information.
        glGetShaderInfoLog (Shader, ErrorSize, &ErrorSize, Error);
        Error[ErrorSize] = '\0';


        // Display error.
        std::cout << Error << std::endl;


        // Free the memory.
        delete[] Error;
        glDeleteShader (Shader);

        // Return error state.
        return false;
    }

    // Else, everything went well.
    else
        return true;
}


// Get the program ID.
GLuint Shader::GetProgramID () const
{
    return m_ProgramID;
}
