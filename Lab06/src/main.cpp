#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void generateTriangles(float x1, float y1, float x2, float y2, float x3, float y3, int& i, int p, float* triangles);
void drawTriangles(int& i, int& iTriangles, int maxTriangles, int currentDepth, int _maxDepth);
int calculationNumberTriangles(int _depth);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int shaderProgram2;
unsigned int VBOs[4], VAOs[4];

int depth = 6;
int temDepth = depth;
int numTriangles = calculationNumberTriangles(depth);
int temNumTriangles = numTriangles;
int numVertices = 9 * numTriangles;
int dir = 0;
float* triangles1 = new float[numVertices];
float* triangles2 = new float[numVertices];
float* triangles3 = new float[numVertices];
int* numDepth = new int[numTriangles];

float colors[18] = {
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 0.0f,
     0.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f };

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
"   FragColor = vec4(0.7f, 0.2f, 0.4f, 1.0f);\n"
"}\n\0";
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

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
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int shaderProgram1 = glCreateProgram();
    shaderProgram2 = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);

    // link
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float triangleM[] = {
        -0.9f, -0.9f, 0.0f,
         0.9f, -0.9f, 0.0f,
         0.0f,  0.9f, 0.0f
    };
    int temIGenerate = 0;
    generateTriangles(triangleM[0], triangleM[1], triangleM[3], triangleM[4], triangleM[6], triangleM[7], temIGenerate, 0, triangles1);
    temIGenerate = 0;
    generateTriangles(triangleM[3], triangleM[4], triangleM[6], triangleM[7], triangleM[0], triangleM[1], temIGenerate, 0, triangles2);
    temIGenerate = 0;
    generateTriangles(triangleM[6], triangleM[7], triangleM[0], triangleM[1], triangleM[3], triangleM[4], temIGenerate, 0, triangles3);


    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleM), triangleM, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // triangles setup
    // ---------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), triangles1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[2]);	
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), triangles2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), triangles3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int tempT = 1;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        if(dir == 0) {
            glBindVertexArray(VAOs[1]);
        }
        else if (dir == 1) {
            glBindVertexArray(VAOs[2]);
        }
        else if (dir == 2) {
            glBindVertexArray(VAOs[3]);
        }
        glUseProgram(shaderProgram2);
        int temI = 0;
        int temTriangles = 0;
        drawTriangles(temI, temTriangles, tempT, 0, temDepth);

        if (tempT < temNumTriangles)
        {
            tempT++;
        }
        else
        {
            tempT = 0;
        }
        _sleep(500/(temDepth+1));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (temDepth < depth) {
            temDepth++;
            temNumTriangles = calculationNumberTriangles(temDepth);
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (temDepth > 0) {
            temDepth--;
            temNumTriangles = calculationNumberTriangles(temDepth);
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && dir != 0)
    {
        dir = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && dir != 2)
    {
        dir = 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && dir != 1)
    {
        dir = 1;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void generateTriangles(float x1, float y1, float x2, float y2, float x3, float y3, int& i, int p, float* triangles)
{
    if (p >= depth)
    {
        return;
    }
    numDepth[i / 9] = p;
    int tempI = i;
    i = i + 9;
    //4
    triangles[tempI] = (x1 + x3) / 2;
    triangles[tempI + 1] = (y1 + y3) / 2;
    triangles[tempI + 2] = 0.0;
    //5
    triangles[tempI + 3] = (x1 + x2) / 2;
    triangles[tempI + 4] = (y1 + y2) / 2;
    triangles[tempI + 5] = 0.0;
    //6
    triangles[tempI + 6] = (x3 + x2) / 2;
    triangles[tempI + 7] = (y3 + y2) / 2;
    triangles[tempI + 8] = 0.0;

    generateTriangles(x1, y1, triangles[tempI + 3], triangles[tempI + 4], triangles[tempI], triangles[tempI + 1], i, p + 1, triangles);
    generateTriangles(triangles[tempI], triangles[tempI + 1], triangles[tempI + 6], triangles[tempI + 7], x3, y3, i, p + 1, triangles);
    generateTriangles(triangles[tempI + 3], triangles[tempI + 4], x2, y2, triangles[tempI + 6], triangles[tempI + 7], i, p + 1, triangles);
}

void drawTriangles(int& i, int& iTriangles, int maxTriangles, int currentDepth, int _maxDepth)
{
    if (currentDepth >= _maxDepth)
    {
        return;
    }
    for (int j = 0; j < 3; j++)
    {
        if (iTriangles > maxTriangles)
        {
            return;
        }
        while (numDepth[i] != currentDepth) {
            i++;
        }

        int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "ourColor");
        glUniform4f(vertexColorLocation2, colors[(currentDepth*3)], colors[(currentDepth * 3) + 1], colors[(currentDepth * 3) + 2], 1.0f);        

        glDrawArrays(GL_TRIANGLES, 3 * (i), 3);
        i++;
        iTriangles++;
        drawTriangles(i, iTriangles, maxTriangles, currentDepth + 1, _maxDepth);
    }
}

int calculationNumberTriangles(int _depth)
{
    int res = 0;
    for (int i = 0; i < _depth; i++)
    {
        res += pow(3, i);
    }
    return res;
}