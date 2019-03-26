#ifndef SHADER_H
#define SHADER_H


class Shader  
{
public:

    // constructor generation the shader on the fly
    // -----------------------------------------------------------
    Shader(const char* passthru_vert, const char* passthru_frag);

    // constructor reloaded for normals
    // -----------------------------------------------------------
    Shader(const char* normals_vert, const char* normals_geom, const char* normals_frag);	    
    // active the shader 
    // -----------------------------------------------------------
    void use();

    // get program ID 
    // -----------------------------------------------------------
    GLuint getID();
 
    // utility uniform functions
    // -----------------------------------------------------------
    void setBool(const std::string &name, bool value) const;
    // -----------------------------------------------------------
    void setInt(const std::string &name, int value) const;
    // -----------------------------------------------------------
    void setFloat(const std::string &name, float value) const;

private:

    	GLuint spo;
	GLuint svo;
	GLuint sfo;
	GLuint sgo;
 
    // utility function for checking shader compilation/linking errors.
    // -----------------------------------------------------------
    void checkCompileErrors(GLuint ID, std::string type);
};
#endif
