#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

class camera_t
{
	public:

	//constructors 
	/*
	camera_t(glm::vec3,glm::vec3, glm::vec3): \
		pos( 0.0f, 0.0f, 4.0f), \
		ref( 0.0f, 0.0f,-1000.0f), \
		vup( 0.0f, 1.0f, 0.0f) \
		{}
    */
	// a bit of a hack...  ??????
	//friend int main(int argc, char **argv);

	camera_t(glm::vec3, glm::vec3, glm::vec3);
	// public member functions
	void roll(float);	// rotate about z-axis
	void pan(float);	// rotate about y-axis
	void tilt(float); // rotate about x-axis
	void dolly(float);	// translation along z-axis
	void pedestal(float); // translation along y-axis
	void truck(float);	// translation along x-axis
	glm::mat4 lookAt();	
	void reset(glm::vec3, glm::vec3, glm::vec3);  // reset to original lookAt
	glm::vec3 Pos();

	private:
	
	glm::vec3	pos;
	glm::vec3	ref;
	glm::vec3	vup;
};

#endif
