/***************************************************************************/
/* This is a simple demo program written for CSE 167 by Ravi Ramamoorthi   */
/* This program corresponds to the first OpenGL lecture.                   */
/*                                                                         */
/* Successive lectures/iterations make this program more complex.          */
/* This is the first simple program to draw a ground plane allowing zooming*/  
/* The intent is to show how to draw a simple scene.                       */
/* Modified in August 2016 by Hoang Tran to exclusively use modern OpenGL  */
/***************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

// Usage of degrees is deprecated. Use radians for glm functions.
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"


int mouseoldx, mouseoldy ; // For mouse motion
GLfloat eyeloc = 2.0 ; // Where to look from; initially 0 -2, 2
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
GLuint projectionPos, modelviewPos; // Locations of uniform variables
glm::mat4 projection, modelview; // The mvp matrices themselves

/***************   BASIC SET UP FOR DRAWING OBJECTS ***********************/

const int numobjects = 2 ; // number of objects for buffer 
const int numperobj  = 3 ; // Vertices, colors, indices
GLuint VAOs[numobjects]; // A VAO for each object
GLuint buffers[numperobj*numobjects] ; // List of buffers for geometric data 
GLuint objects[numobjects] ; // For each object
GLenum PrimType[numobjects] ; // Primitive Type (quads, polygons etc.)
GLsizei NumElems[numobjects] ; // Number of geometric elements

// Floor Geometry is specified with a vertex array

enum {Vertices, Colors, Elements} ; // For arrays for object 
enum {FLOOR, FLOOR2} ; // For objects, for the floor

const GLfloat floorverts[4][3] = {
  {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, {0.5,-0.5, 0.0}
} ; 
const GLfloat floorcol[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
} ; 

const GLubyte floorinds[1][6] = { {0, 1, 2, 0, 2, 3} } ; 

const GLfloat floorverts2[4][3] = {
  {0.5, 0.5, 1.0}, {-0.5, 0.5, 1.0}, {-0.5, -0.5, 1.0}, {0.5, -0.5, 1.0}
} ; 
const GLfloat floorcol2[4][3] = {
  {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
} ; 
const GLubyte floorinds2[1][6] = { { 0, 1, 2, 0, 2, 3 } } ;

// Treat this as a destructor function. Delete any dynamically allocated memory here
void deleteBuffers() 
{
	glDeleteVertexArrays(numobjects, VAOs);
	glDeleteBuffers(numperobj*numobjects, buffers);
}

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 
void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type)
{
  int offset = object * numperobj;
  glBindVertexArray(VAOs[object]);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
  // Use layout location 0 for the vertices
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ; 
  glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
  // Use layout location 1 for the colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
  PrimType[object] = type;
  NumElems[object] = sizeind;
  // Prevent further modification of this VAO by unbinding it
  glBindVertexArray(0);
}

void drawobject(GLuint object) 
{
  glBindVertexArray(VAOs[object]);
  glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0); 
  glBindVertexArray(0);
}

/****************   BASIC SETUP FOR DRAWING OBJECTS ***********************/
void display(GLFWwindow* window)
{
  /* Clear all pixels  */
  glClear (GL_COLOR_BUFFER_BIT);

  // draw polygon (square) of unit length centered at the origin
  // Note that vertices must generally go counterclockwise
  // This code draws each vertex in a different color.  
  // The hardware will blend between them.  
  // This is a useful debugging trick.  I can make sure that each vertex 
  // appears exactly where I expect it to appear.

  // The old OpenGL code of using glBegin... glEnd no longer appears. 
  // The new version uses vertex array and vertex buffer objects from init.   
  drawobject(FLOOR);
  drawobject(FLOOR2) ; 
  
}

/* Defines a Mouse callback to zoom in and out */
/* This is done by modifying gluLookAt         */
/* The actual motion is in mousedrag           */
/* mouse simply sets state for mousedrag       */


/* Defines what to do when various mouse */
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action == GLFW_RELEASE)
	{
      // Do Nothing ;
    }
    else if (action == GLFW_PRESS)
	{
	  double x, y;
	  glfwGetCursorPos(window, &x, &y);

      mouseoldx = (int)x ; 
	  mouseoldy = (int)y ; // so we can move wrt x , y 
    }
  }
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
  { 
	// Reset gluLookAt
    eyeloc = 2.0 ;
	modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
	// Send the updated matrix to the shader
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  }
}

/* Defines what to do when various mousedrag */
void mousedrag_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		int yloc = (int)y - mouseoldy;	// We will use the y coord to zoom in/out

		eyeloc += 0.005f*yloc;		// Where do we look from

		if (eyeloc < 0)
			eyeloc = 0.0;

		mouseoldy = (int)y;

		// Set the eye location 
		modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));

		// Send the updated matrix over to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
	}
}

/* Defines what to do when various keys are pressed */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
  {
	  deleteBuffers();
	  glfwSetWindowShouldClose(window, true);
  }
}

/* Reshapes the window appropriately */
void reshape_callback(GLFWwindow* window, int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  
  // Think about the rationale for this choice
  // What would happen if you changed near and far planes? 
  // Note that the field of view takes in a radian angle!
  
  if (h > 0)
  {
	  projection = glm::perspective(30.0f / 180.0f * glm::pi<float>(), (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);
	  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  }
 
}

void init (void) 
{
  /* select clearing color 	*/
  glClearColor (0.0, 0.0, 0.0, 0.0);

  /* initialize viewing values  */
  projection = glm::mat4(1.0f); // The identity matrix

  // Think about this.  Why is the up vector not normalized?
  modelview = glm::lookAt(glm::vec3(0,-eyeloc,eyeloc), glm::vec3(0,0,0), glm::vec3(0,1,1));

  // Now create the buffer objects to be used in the scene later
  // Remember to delete all the VAOs and VBOs that you create when the program terminates!
  glGenVertexArrays(numobjects, VAOs);
  glGenBuffers(numperobj*numobjects, buffers);

  // Initialize the floors
  initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof (floorcol), (GLubyte *) floorinds, sizeof (floorinds), GL_TRIANGLES) ; 
  initobject(FLOOR2, (GLfloat *) floorverts2, sizeof(floorverts2), (GLfloat *) floorcol2, sizeof (floorcol2), (GLubyte *) floorinds2, sizeof (floorinds2), GL_TRIANGLES) ;

  std::string vertex_shader_path;
  std::string fragment_shader_path;

#ifdef __unix__         
  vertex_shader_path = "//home//manuel//Documents//Projects//OpenGL//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.vert";
  fragment_shader_path = "//home//manuel//Documents//Projects//OpenGL//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glut//shaders//nop.frag";

#elif defined(_WIN32) || defined(WIN32) 
  vertex_shader_path = "C://Users//juanv//Documents//CG-CMAKE//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.vert";
  fragment_shader_path = "C://Users//juanv//Documents//CG-CMAKE//GLFW_GLAD_GLUT_GLEW_cmake_project//src//mytest1_glfw//shaders//nop.frag";
#endif


  // Initialize the shader program
  vertexshader = initshaders(GL_VERTEX_SHADER, vertex_shader_path.c_str());
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, fragment_shader_path.c_str());

  shaderprogram = initprogram(vertexshader, fragmentshader) ;

  // Get the positions of the uniform variables
  projectionPos = glGetUniformLocation(shaderprogram, "projection");
  modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
  // Pass the projection and modelview matrices to the shader
  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

}

int main(int argc, char** argv)
{
  // Requests the type of buffers (Single, RGB).
  // Think about what buffers you would need...

  glfwInit(); //glutInit(&argc, argv);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(500, 500, "Simple Demo with Shaders", NULL, NULL);
  
  if (window == NULL)
  {
	  std::cout << "Failed to create GLFW window" << std::endl;
	  glfwTerminate();
	  return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowPos(window, 100, 100);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  std::cout << "Failed to initialize GLAD" << std::endl;
	  return -1;
  }

  init (); // Always initialize first

  glfwSetFramebufferSizeCallback(window, reshape_callback);	//glutReshapeFunc(reshape) ;
  glfwSetKeyCallback(window, key_callback);					//glutKeyboardFunc(keyboard);
  glfwSetMouseButtonCallback(window, mouse_callback);		//glutMouseFunc(mouse) ;
  glfwSetCursorPosCallback(window, mousedrag_callback);		//glutMotionFunc(mousedrag) ;

  // First scene render
  reshape_callback(window, 500, 500);
  display(window); //glutDisplayFunc(display); 
  

  // render loop
  // Start the main code
  while (!glfwWindowShouldClose(window)) //glutMainLoop(); 
  {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  display(window); //glutDisplayFunc(display); 

	  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	  // -------------------------------------------------------------------------------
	  glfwSwapBuffers(window);
	  glfwPollEvents();

	  //glFlush();
  }
  
  deleteBuffers(); // Program termination. Delete the buffers generated in init().
  
  glfwDestroyWindow(window);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  
  return 0;   /* ANSI C requires main to return int. */

}
