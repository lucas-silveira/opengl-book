#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/shader.hpp"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    gladLoadGL(glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    Shader shaderProgram("shader.vert", "shader.frag");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.f,  0.f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.f,  0.f,

        -0.5f, -0.5f,  0.5f,  0.f,  0.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.f,  0.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.f,

        -0.5f, -0.5f, -0.5f,  0.f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,
        -0.5f, -0.5f, -0.5f,  0.f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.f,
        -0.5f,  0.5f,  0.5f,  0.f,  0.f,
        -0.5f,  0.5f, -0.5f,  0.f,  1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture 1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("../assets/wood.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Texture 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true); // invert y-axis of the image while loading it
    data = stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // Free the image memory
    stbi_image_free(data);

    shaderProgram.use();
    shaderProgram.setInt("tex1", 0);
    shaderProgram.setInt("tex2", 1);

    // Uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers

        shaderProgram.use();
        /* Transformation matrices */
        // World transformation
        glm::mat4 model = glm::mat4(1.f);
        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(50.f), glm::vec3(0.5f, 1.f, 0.f));
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // View transformation
        glm::mat4 view = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f)); // we translate the scene in the reverse direction
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Projection transformation
        glm::mat4 projection;
        float fov = glm::radians(45.f);
        float aspectRatio = (float)SCREEN_WIDTH/SCREEN_HEIGHT;
        float nearPlane = 0.1f;
        float farPlane = 100.f;
        projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
        int projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Drawing
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.remove();

    glfwTerminate();
    return 0;
}