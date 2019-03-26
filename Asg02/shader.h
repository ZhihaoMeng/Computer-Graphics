#ifndef SHADER_H
#define SHADER_H
#endif


class Shader  
{
public:

    // constructor generation the shader on the fly
    // -----------------------------------------------------------
    Shader(const char* vfile, const char* ffile);
	    
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
 
    // utility function for checking shader compilation/linking errors.
    // -----------------------------------------------------------
    void checkCompileErrors(GLuint ID, std::string type);
};
