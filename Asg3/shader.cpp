#ifdef __APPLE__
#include <OpenGl/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <fstream>
#include <iostream>
#include <thread>

#include "shader.h"

        // constructor generation the shader on the fly
	// -------------------------------------------------------
    Shader::Shader(const char* vfile, const char* ffile)
    {

        // Read the source code for vertex shader & fragment shader
        // -------------------------------------------------------	    
       try
	{
            // create vertex shader, load source code strings into shader 
	    svo = glCreateShader(GL_VERTEX_SHADER);
	    {
		    std::ifstream source_file(vfile);
		    std::string data;
		    std::getline(source_file, data, '\0');
		    const GLchar *vs(data.c_str());

		    glShaderSource(svo, 1, (const GLchar**)&vs, NULL);
	    }

	    sfo = glCreateShader(GL_FRAGMENT_SHADER);
	    {
		    std::ifstream source_file(ffile);
		    std::string data;
		    std::getline(source_file, data, '\0');
		    const GLchar *fs(data.c_str());

		    glShaderSource(sfo, 1, (const GLchar**)&fs, NULL);
	    }
	    
	}
	  	catch (std::ifstream::failure e)
	{
	    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	//2. compile shaders 
        // -------------------------------------------------------
        // vertex shader 
        glCompileShader(svo);
        checkCompileErrors(svo, "VERTEX");

        // fragment shader 
	glCompileShader(sfo);
	checkCompileErrors(sfo, "FRAGMENT");

        // shader program 
	spo = glCreateProgram();
	glAttachShader(spo, svo);
	glAttachShader(spo, sfo);
	glLinkProgram(spo);
	checkCompileErrors(spo, "PROGRAM");

	// delete the shader as they're linked into our program ow and no longer necessary
	glDeleteShader(svo);
	glDeleteShader(sfo);
    }

    // active the shader
    // -----------------------------------------------------------
    void Shader::use()
    {
        glUseProgram(spo);
    }

    // get program ID 
    GLuint Shader::getID() 
    {
        return spo;
    }
  
    // utility uniform functions
    // -----------------------------------------------------------
    void Shader::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(spo, name.c_str()), (int) value);
    }
    // -----------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(spo, name.c_str()), value);
    }
    // -----------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(spo, name.c_str()), value);
    }


    // utility function for checking shader compilation/linking errors.
    // -----------------------------------------------------------

    void Shader::checkCompileErrors(GLuint ID, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
            glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	    if(!success)
            {
                glGetShaderInfoLog(ID, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPLATION_ERROR of type: " << type << "\n" << infoLog << "\n ----------------------------------------" << std::endl;
	    }
	}
	else
	{
	    glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(!success)
	    {
	        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------" << std::endl;
	    }
	}
    }
