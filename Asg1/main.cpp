#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <thread>       // std::thread (C++11)
#include <cmath>        // math

#ifdef __APPLE__
//#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//#include <OpenGL/gl.h>
//#include <OpenGL/glext.h>
//#include <GLUT/glut.h>
// use modern GL 3.2
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define GLFW_INCLUDE_GLCOREARB  // to prevent GLFW from loading gl.h
#include <GLFW/glfw3.h>         // GL window context

#include "shader.h"

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void key_callback(GLFWwindow *window,int key,int scancode,int action,int mods)
{
  if(key == GLFW_KEY_W && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if(key == GLFW_KEY_F && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
  glViewport(0,0,w,h);
}

int main()
{
        GLFWwindow* window = NULL;

  // init GLFW library
  if(!glfwInit())
    return -1;

  // set up window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // windowed instead of full-screen
  if(!(window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr))) {
    glfwTerminate();
    return -1;
  }

  // callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // make window's context current
  glfwMakeContextCurrent(window);

  // GLSL shaders
  Shader shader("./vert.glsl", "frag.glsl");

//std::this_thread::sleep_for(std::chrono::seconds(1));

  GLfloat verts[] = {
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f  // top left
  };

  GLuint indices[] = {
     0, 1, 3,           // first tri
     1, 2, 3            // second tri
  };

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  // set up vertex array object
  GLuint vao;
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  // set up vertex buffer object
  GLuint vbo;
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // transfer (copy) verts to GPU
  // GL_STATIC_DRAW: data will most likely not change at all or very rarely
  // GL_DYNAMIC_DRAW: data is likely to change often
  // GL_STREAM_DRAW: data will every time it is drawn
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // link vertex attributes, let GL know how to use vbo
  //      index:               0: layout (location = 0)
  //       size:               3: size of vertex attribute (vec3)
  //       type:        GL_FLOAT: type of verts
  // normalized:        GL_FALSE: don't normalize
  //     stride: 3*sizeof(float): bits per attr
  //     offset:        (void*)0: offset into array

  // using named in vec3 position
  const GLint location(glGetAttribLocation(shader.getID(),"position"));
  glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
  glEnableVertexAttribArray(location);

  // loop until user closes window
  while(!glfwWindowShouldClose(window)) {

    // check for key input
    processInput(window);

    // render here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();


    // set variables used in shader
    const GLint location(glGetUniformLocation(shader.getID(),"color"));
    float time = glfwGetTime(); // time in seconds
    GLfloat green = (sin(time) / 2.0f) + 0.5f;
    glUniform4f(location, 0.0f, green, 0.0f, 1.0f);

    glBindVertexArray(vao);
    // render rectangle from indeces
    // see: https://learnopengl.com/Getting-started/Hello-Triangle
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // swap buffers
    glfwSwapBuffers(window);

    // poll for events
    glfwPollEvents();

  }

  glfwTerminate();
  return 0;
}
