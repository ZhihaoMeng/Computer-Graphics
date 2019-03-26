#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_INCLUDE_GLCOREARB 
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"
#include "aw_t.h"
#include "camera.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // init GL
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);



    // build and compile our shader zprogram
    // ------------------------------------
    Shader shader("passthru.vert", "passthru.frag");
    Shader normals("normals.vert", "normals.geom", "normals.frag");
    Shader phong("phong.vert", "phong.frag");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // use parser to read .obj file 
    aw_t myaw;
    myaw.loadObj("pyramid.obj");
    //myaw.loadObj("cube.obj");
    GLfloat *data=myaw.pack(true);
    GLuint dataSize=myaw.getDataSize();
    GLuint VERTEX_LENGTH = myaw.getVERTEXLength();
    //std::vector<glm::vec3> vertices = myaw.getVerts();
    //std::vector<unsigned int> indices = myaw.getVi();	
    GLfloat* oc=myaw.getCenter();
    //std::cout<<"vertices size is "<< vertices.size()<<std::endl;
    //std::cout<<"indices size is " << indices.size() << std::endl;
    std::cout<<"data size is "<<dataSize<<std::endl;
    std::cout<<"VERTEX_LENGTH is "<< VERTEX_LENGTH<<std::endl;

    float tc[] = {
	    0.0f, 0.0f, 0.0f
    };
    //unsigned int VBO, VAO, EBO;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(GLfloat), data, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void*)((VERTEX_LENGTH-3) * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);


    // -------------------------------------------------------------------------------------------
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    normals.use(); 
    phong.use();
    
    // load and create a texture
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // GL_TEXTURE_2D operations on texture object
    // set tge texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);// flip texture 
    unsigned char *img = stbi_load("naroto.jpg", &width, &height, &nrChannels, 0);
    if (img)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(img);
   
    glm::vec3 pos(0.0f, 0.0f, 4.0f);
    glm::vec3 ref(0.0f, 0.0f,-1000.0f);
    glm::vec3 vup(0.0f, 1.0f, 0.0f);
    camera_t cam_t(pos,ref,vup); 	 
    glm::mat4 v;
    v = cam_t.lookAt(); 	
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
	
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
	    cam_t.reset(pos, ref, vup);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
	    cam_t.truck(-0.1f);
	    v = cam_t.lookAt();
	}	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
	    cam_t.truck(0.1f);
	    v = cam_t.lookAt();
	}
       	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
	    cam_t.dolly(-0.1f);
	    v = cam_t.lookAt();
	}
       	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
	    cam_t.dolly(0.1f);
	    v = cam_t.lookAt();
	}
       	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
	    cam_t.pedestal(-0.1f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
	    cam_t.pedestal(0.1f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
	    cam_t.tilt(-3.0f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
	    cam_t.tilt(3.0f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
	    cam_t.roll(-3.0f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
	    cam_t.roll(3.0f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
	    cam_t.pan(-3.0f);
	    v = cam_t.lookAt();
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
	    cam_t.pan(3.0f);
	    v = cam_t.lookAt();
	}



	// glm matrix 
	int width, height;
	float ratio;
	glm::mat4 m, p, t1, t2, t3;
	//glm::mat4 v =glm::mat4(1.0f);
	glm::mat4 mvp = glm::mat4(1.0f);
	

	glfwGetFramebufferSize(window, &width, &height);
	ratio = (float) width/ (float) height;
	// seeting t3
	tc[0] = cos((float)glfwGetTime()*1/2.0);
        tc[1] = sin((float)glfwGetTime()*1/2.0);
	// using glm
	m = glm::mat4(1.0f);
	m = glm::rotate(m, 1*(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	t1 = glm::translate(glm::mat4(1.0f), glm::vec3(-oc[0], -oc[1], -oc[2]));
	t2 = glm::translate(glm::mat4(1.0f), glm::vec3( oc[0],  oc[1],  oc[2]));
	t3 = glm::translate(glm::mat4(1.0f), glm::vec3( tc[0],  tc[1],  tc[2]));
	// l,r,b,t,n,f	
	//p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	p = glm::perspective(glm::radians(60.0f), ratio, 0.001f, 10.0f);
	//glm::vec3 pos(0.0f, 0.0f, 4.0f);
	//glm::vec3 ref(0.0f, 0.0f,-1000.0f);
	//glm::vec3 vup(0.0f, 1.0f, 0.0f);
	//v = glm::lookAt(glm:vec3(0,-1,3),glm:vec3(0,0,-1000),glm::vec3(0,1,0));
	//v = glm::lookAt(pos,ref,vup);
	//mvp = p*(t3*(t2*m*t1));
	//mvp = p*(t3*(t2*m*t1));
	mvp = p*v*(t3*(t2*m*t1));
	
	
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        // bind Texture
	glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
	
	// passthru shader
        shader.use();	
	
	// set variables used in shader
        // const GLint location_color(glGetUniformLocation(shader.getID(),"color"));
        float time = glfwGetTime(); // time in seconds
        GLfloat green = (sin(time) / 2.0f) + 0.5f;
        //glUniform4f(location_color, 1.0f, green, 0.31f, 1.0f);
	//set v

	GLint location = glGetUniformLocation(shader.getID(), "MVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mvp));
	// Draw
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES,0,dataSize/VERTEX_LENGTH);



	// phong shader
	phong.use();
	// set variables used in phong shader
	// color in phong.frag
	// const GLint location_phong_color(glGetUniformLocation(phong.getID(),"color"));
        //glUniform4f(location_phong_color, 1.0f, 0.5, 0.31f, 1.0f);
	
	// L_world in phong.frag
	const GLint location_phong_L_world(glGetUniformLocation(phong.getID(),"L_world"));
        glUniform3f(location_phong_L_world, 0.0f, 2.0f,3.0f);
	
	glm::vec3 pos = cam_t.Pos(); 
	// V_world in phong.frag
	const GLint location_phong_V_world(glGetUniformLocation(phong.getID(),"V_world"));
        glUniform3f(location_phong_V_world, pos.x, pos.y,pos.z);
	

	// MVP in phong.vert
	GLint location_phong = glGetUniformLocation(phong.getID(), "MVP");
	glUniformMatrix4fv(location_phong, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mvp));	
	//M in phong.vert
	GLint location_phong_M = glGetUniformLocation(phong.getID(), "M");
	glUniformMatrix4fv(location_phong_M, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(m));
	// V in phong.vert
	GLint location_phong_V = glGetUniformLocation(phong.getID(), "V");
	glUniformMatrix4fv(location_phong_V, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(v));
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,dataSize/VERTEX_LENGTH);
	


	// draw normals
	normals.use();
	// set variables used in normals
        const GLint location_geom_color(glGetUniformLocation(normals.getID(),"color"));
        glUniform4f(location_geom_color, 0.0f, green ,0.0f, 1.0f);
	//set v

	GLint location_geom = glGetUniformLocation(normals.getID(), "mvp");
	glUniformMatrix4fv(location_geom, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES,0,dataSize/VERTEX_LENGTH);



	
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
