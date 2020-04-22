#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float r = 1.0;
float g = 1.0;
float b = 1.0;
float aValue = 1.0;
float inte = 1.0;
bool br = false;
bool bg = false;
bool bb = false;

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

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
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

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        // render        
        if (br && bg && bb) {
            glClearColor(r * inte, g * inte, b * inte, aValue);
        }
        else if (br && !bg && !bb) {
            glClearColor(r * inte, 0.0f, 0.0f, aValue);
        }
        else if (!br && bg && !bb) {
            glClearColor(0.0f, g * inte, 0.0f, aValue);
        }
        else if (!br && !bg && bb) {
            glClearColor(0.0f, 0.0f, b * inte, aValue);
        }
        else if (br && bg && !bb) {
            glClearColor(r * inte, g * inte, 0.0f, aValue);
        }
        else if (br && !bg && bb) {
            glClearColor(r * inte, 0.0f, b * inte, aValue);
        }
        else if (!br && bg && bb) {
            glClearColor(0.0f, g * inte, b * inte, aValue);
        }
        else if (!br && !bg && !bb) {
            glClearColor(0.0f, 0.0f, 0.0f, aValue);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        br = !br;
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        bg = !bg;
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        bb = !bb;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS && aValue < 1.0) {
        
        aValue += 0.1;
        std::cout << aValue << std::endl;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && aValue > 0.0) {
        aValue -= 0.1;
        std::cout << aValue << std::endl;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && inte < 1.0) {

        inte += 0.1;
        std::cout << inte << std::endl;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && inte > 0.0) {
        inte -= 0.1;
        std::cout << inte << std::endl;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}