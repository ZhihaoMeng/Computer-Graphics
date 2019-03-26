#ifndef AW_T_H
#define AW_T_H

#include <algorithm>

#ifdef __APPLE__
#include <OpenGl/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
class aw_t
{
    public:
        // constructor
	// -----------------------------
	aw_t();

	// load .obj file
	// -----------------------------
	void  loadObj(const char *filename);

	// get index of vert
	std::vector<unsigned int> getVi();

	// get group name 
	std::string getGroup();
	
	// get vertices coordinates
	// --------------------
	std::vector<glm::vec3> getVerts();


	// pack all the data to a GLFloat array to be ready for VBO
	// --------------------------------------------------------	
	GLfloat* pack();
	
	// get size of packed data
	// ----------------------
	GLuint getDataSize();
	
	// check data vt existed
	// --------------------
	bool isVt();

	// check data vn existed
	// --------------------
	bool isVn();
	
	// get the center of geometry
	// -------------------------
	GLfloat* getCenter();
	
        // get stide length 
	GLuint getVERTEXLength();
    private:
        //indices of vertices for each face
        // ---------------------------------
        std::vector<unsigned int> vi;

        // indices of texture coordinates
        // ------------------------------
        std::vector<unsigned int> vti;

        // indices of normal coordintes
        // ----------------------------
        std::vector<unsigned int> vni;

        // group name 
        std::string group;

        // vertices coordinates
        // --------------------
        std::vector<glm::vec3> verts;

        // coordinates of texture vertices
        // -------------------------------
        std::vector<glm::vec2> vt;

        // normal coordintes
        std::vector<glm::vec3> vn;
	
	// VERTEX LENGTH
	// -------------
	GLuint VERTEX_LENGTH;
	
	// GLfloat array size
	GLuint dataSize;

};


#endif
