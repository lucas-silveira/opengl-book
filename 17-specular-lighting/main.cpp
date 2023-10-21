#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/shader.hpp"
#include "../include/camera.hpp"

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Time
float deltaTime = 0.f;
float lastFrame = 0.f;

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool firstMouse = true;
float lastMouseX = SCREEN_WIDTH/2.f;
float lastMouseY = SCREEN_HEIGHT/2.f;

// Lighting
glm::vec3 lightPos(1.2f, 1.f, 2.f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Avoid camera jump to the mouse position from the center
    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed: y ranges from bottom to top
    lastMouseX = xpos;
    lastMouseY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main(void)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // Capture mouse input and hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("shader.vert", "shader.frag");
    Shader lightingCubeShader("light-cube.vert", "light-cube.frag");

    float vertices[] = {
        // Positions          // Normals
        // Back
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,
         0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,
        -0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,
        // Front
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,
         0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,
        -0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,
        // Left
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,
        -0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,
        -0.5f, -0.5f,  0.5f, -1.f,  0.f,  0.f,
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,
        // Right
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,
         0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,
         0.5f, -0.5f,  0.5f,  1.f,  0.f,  0.f,
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,
        // Bottom
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,
         0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,
        -0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,
        // Top
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,
         0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,
        -0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,
    };
    // Cube
    unsigned int VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Light source cube (we only create this object in order to visualize the position of the source light)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // Use can use the existing VBO since the cube vertices is the same
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    lightingShader.use();
    lightingShader.setVec3("lightPos", lightPos);

    // Uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers

        // Use object related shader
        lightingShader.use();
        lightingShader.setVec3("objColor", 1.f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.f, 1.f, 1.f);
        lightingShader.setVec3("viewPos", camera.Position);

        // Projection transformation
        glm::mat4 projection;
        float aspectRatio = (float)SCREEN_WIDTH/SCREEN_HEIGHT;
        float nearPlane = 0.1f;
        float farPlane = 100.f;
        projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, nearPlane, farPlane);
        lightingShader.setMat4("projection", projection);
        // View transformation
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);
        // Model transformation
        glm::mat4 model = glm::mat4(1.f);
        lightingShader.setMat4("model", model);
        // Normal matrix
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model)); // inversing matrices is an expensive operation for shaders, so it's better to do it once from CPU and send it as an uniform
        lightingShader.setMat4("normalMatrix", normalMatrix);
        // Drawing object
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing source light cube
        lightingCubeShader.use();
        lightingCubeShader.setMat4("projection", projection);
        lightingCubeShader.setMat4("view", view);
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightingCubeShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}