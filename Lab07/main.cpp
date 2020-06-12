#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "fun.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int VBOs[2], VAOs[2];

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

float firstTriangle[] = {
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
};
float secondTriangle[] = {
    0.0f, -0.5f, 0.0f,  // left
    0.9f, -0.5f, 0.0f,  // right
    0.45f, 0.5f, 0.0f   // top 
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // we skipped compile log checks this time for readability (if you do encounter issues, add the compile-checks! see previous code samples)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // the second fragment shader that outputs the color yellow
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram(); // the second shader program
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);
    // link the first program object
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    // then link the second program object using a different fragment shader (but same vertex shader)
    // this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders are equally matched.
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // now when we draw the triangle we first use the vertex and orange fragment shader from the first program
        glUseProgram(shaderProgramOrange);
        // draw the first triangle using the data from our first VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output an orange triangle
        // then we draw the second triangle using the data from the second VAO
        // when we draw the second triangle we want to use a different shader program so we switch to the shader program with our yellow fragment shader.
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output a yellow triangle

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //firstTriangle
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationLeft(firstTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationRight(firstTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationUp(firstTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationDown(firstTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        mtxMulVec(mirrorYZ, firstTriangle);
        mtxMulVec(mirrorYZ, firstTriangle + 3);
        mtxMulVec(mirrorYZ, firstTriangle + 6);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        mtxMulVec(mirrorXZ, firstTriangle);
        mtxMulVec(mirrorXZ, firstTriangle + 3);
        mtxMulVec(mirrorXZ, firstTriangle + 6);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            scale(firstTriangle + (i * 3), 1.1);
        }
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            scale(firstTriangle + (i * 3), 0.9);
        }
    }
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateZ(firstTriangle + (i * 3), 45);
        }
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateX(firstTriangle + (i * 3), 15);
        }
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateY(firstTriangle + (i * 3), 15);
        }
    }

    //secon
    else if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationLeft(secondTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationRight(secondTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationUp(secondTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            translationDown(secondTriangle + (i * 3), 0.2f);
        }
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        mtxMulVec(mirrorYZ, secondTriangle);
        mtxMulVec(mirrorYZ, secondTriangle + 3);
        mtxMulVec(mirrorYZ, secondTriangle + 6);
    }
    else if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        mtxMulVec(mirrorXZ, secondTriangle);
        mtxMulVec(mirrorXZ, secondTriangle + 3);
        mtxMulVec(mirrorXZ, secondTriangle + 6);
    }
    else if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            scale(secondTriangle + (i * 3), 1.1);
        }
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            scale(secondTriangle + (i * 3), 0.9);
        }
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateZ(secondTriangle + (i * 3), 45);
        }
    }
    else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateX(secondTriangle + (i * 3), 15);
        }
    }
    else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        for (int i = 0; i < 3; i++) {
            rotateY(secondTriangle + (i * 3), 15);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_DYNAMIC_DRAW);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}