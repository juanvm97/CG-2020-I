#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>

#include "fun.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void transform_triangle();
void transform_square();
void transform_rhombus();
void randColor();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int VBOs[3], VAOs[3], EBOs[3];
unsigned int speed = 130;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

float vertices1[] = {
    -0.1f, -0.1f, 0.0f,
     0.1f, -0.1f, 0.0f,
     0.0f,  0.1f, 0.0f, // top
};

float vertices2[] = {
     0.5f,  0.1f, 0.0f,  // top right
     0.5f, -0.1f, 0.0f,  // bottom right
     0.3f, -0.1f, 0.0f,  // bottom left
     0.3f,  0.1f, 0.0f   // top left 
};

float vertices3[] = {
     0.8f,  0.1f, 0.0f,  // top
     0.9f,  0.0f, 0.0f,  // right
     0.8f,  -0.1f, 0.0f,  // bottom
     0.7f,   0.0f, 0.0f   // left 

};

bool animation = true;
bool upTriangle = true;

float center[] = {
    0.4f, 0.0f, 0.0f
};

bool verticalRhombus = true;
bool upRhombus = true;
bool leftRhombus = true;

float rand1;
float rand2;
float rand3;

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
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    unsigned int indices1[] = {
        0, 1, 2,  // first Triangle
    };

    unsigned int indices2[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int indices3[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    glGenBuffers(3, EBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // draw our first triangle
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        randColor();
        glUniform4f(vertexColorLocation, rand1, rand2, rand3, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        randColor();
        glUniform4f(vertexColorLocation, rand2, rand3, rand1, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        randColor();
        glUniform4f(vertexColorLocation, rand3, rand1, rand2, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        if (animation)
        {
            transform_triangle();
            transform_square();
            transform_rhombus();

            _sleep(speed);
        }
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glDeleteBuffers(3, EBOs);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void transform_triangle()
{
    if (upTriangle) {
        if (vertices1[7] < 0.21) {
            for (int i = 0; i < 3; i++) {
                scale(vertices1 + (i * 3), 1.05);
            }
        }
        else {
            upTriangle = false;
        }
    }
    else {
        if (vertices1[7] > 0.1) {
            for (int i = 0; i < 3; i++) {
                scale(vertices1 + (i * 3), 0.95);
            }
        }
        else
        {
            upTriangle = true;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);
}

void transform_square()
{
    rotateZ(center, 5.29);
 
    float vertices2[] = {
     0.5f,  0.1f, 0.0f,  // top right
     0.5f, -0.1f, 0.0f,  // bottom right
     0.3f, -0.1f, 0.0f,  // bottom left
     0.3f,  0.1f, 0.0f   // top left 
    };

    vertices2[0] = center[0] + 0.1f;
    vertices2[1] = center[1] + 0.1f;
    vertices2[3] = vertices2[0];
    vertices2[4] = vertices2[1] - 0.2;
    vertices2[6] = vertices2[0] - 0.2;
    vertices2[7] = vertices2[1] - 0.2;
    vertices2[9] = vertices2[0] - 0.2;
    vertices2[10] = vertices2[1];

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
}

void transform_rhombus()
{
    if (verticalRhombus) {
        if (upRhombus) {
            if (vertices3[1] < 0.9) {
                for (int i = 0; i < 4; i++) {
                    translationUp(vertices3 + (i * 3), 0.1f);
                }
            }
            else {
                verticalRhombus = false;
                upRhombus = false;
            }
        }
        else {
            if (vertices3[7] > -0.9) {
                for (int i = 0; i < 4; i++) {
                    translationDown(vertices3 + (i * 3), 0.1f);
                }
            }
            else {
                verticalRhombus = false;
                upRhombus = true;
            }
        }
    }
    else {
        if (leftRhombus) {
            if (vertices3[9] > -0.9) {
                for (int i = 0; i < 4; i++) {
                    translationLeft(vertices3 + (i * 3), 0.1f);
                }
            }
            else {
                verticalRhombus = true;
                upRhombus = false;
                leftRhombus = false;
            }
        }
        else { 
            if (vertices3[3] < 0.8) {
                for (int i = 0; i < 4; i++) {
                    translationRight(vertices3 + (i * 3), 0.1f);
                }
            }
            else {
                verticalRhombus = true;
                upRhombus = true;
                leftRhombus = true;
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_DYNAMIC_DRAW);
}

void randColor()
{
    srand(time(NULL));
    rand1 = (rand() % 11) / 10.0;
    rand2 = (rand() % 11) / 10.0;
    rand3 = (rand() % 11) / 10.0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        animation = !animation;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && speed > 50) {
        speed -= 10;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && speed < 200) {
        speed += 10;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}