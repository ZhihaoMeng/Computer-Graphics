#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <cmath>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <glm/detail/setup.hpp>
#define	GLM_SWIZZLE_XYZW
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "camera.h"

camera_t::camera_t(glm::vec3 pos, glm::vec3 ref, glm::vec3 vup)
{
	this->pos = pos;
	this->ref = ref;
	this->vup = vup;
}

void camera_t::tilt(float angle)
{	
	glm::mat4	M = glm::mat4(1.0f);
	glm::vec3	vref;
	glm::vec3	axis;

	vref = glm::normalize(ref - pos);
	axis = glm::normalize(glm::cross(vref,vup));
	
	//M = glm::rotate(M,(float)glm::radians(angle), glm::vec3(1.0f,0.0f,0.0f));
	M = glm::rotate(M,(float)glm::radians(angle), axis);
	
	glm::vec4 ref4(M * glm::vec4((ref-pos),1.0f));
	ref.x = ref4.x + pos.x;
	ref.y = ref4.y + pos.y;
	ref.z = ref4.z + pos.z;
	

	//ref = (M * glm::vec4((ref-pos),1.0f)).xyz + pos;
	//vup = (M * glm::vec4(vup,1.0f));
	glm::vec4 vup4(M * glm::vec4(vup, 1.0f));
	vup.x = vup4.x;
	vup.y = vup4.y;
	vup.z = vup4.z;
	
}

void camera_t::pan(float angle)
{
	glm::mat4	M = glm::mat4(1.0f);
	//glm::vec3	vref;
	
	//vref = glm::normalize(ref-pos);
	
	M = glm::rotate(M,(float)glm::radians(angle), vup);
	glm::vec4 ref4(M * glm::vec4((ref-pos), 1.0f));
	ref.x = ref4.x + pos.x;
	ref.y = ref4.y + pos.y;
	ref.z = ref4.z + pos.z;
	
} 

void camera_t::roll(float angle)
{
	glm::mat4	M = glm::mat4(1.0f);
	glm::vec3	vref;
	vref = -glm::normalize(ref-pos);
	
	M = glm::rotate(M, (float)glm::radians(angle),vref);
	glm::vec4 vup4(M * glm::vec4(vup,1.0f));
	vup.x = vup4.x;
	vup.y = vup4.y;
	vup.z = vup4.z;



}

void camera_t::dolly(float zoffset)
{
	pos.z += zoffset;
	ref.z += zoffset;
}

void camera_t::pedestal(float yoffset)
{
	pos.y += yoffset;
	ref.y += yoffset;
}

void camera_t::truck(float xoffset)
{
	pos.x += xoffset;
	ref.x += xoffset;
}

void camera_t::reset(glm::vec3 pos, glm::vec3 ref, glm::vec3 vup)
{
	this->pos = pos;
	this->ref = ref;
	this->vup = vup;
}

glm::mat4 camera_t::lookAt()
{
	return glm::lookAt(pos, ref, vup);
}
glm::vec3 camera_t::Pos()
{
	return pos;
}
