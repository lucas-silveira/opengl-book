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
const unsigned int kScreenWidth = 800;
const unsigned int kScreenHeight = 600;

// Time
float delta_time = 0.f;
float last_frame = 0.f;

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool first_mouse = true;
float last_mouse_x = kScreenWidth/2.f;
float last_mouse_y = kScreenHeight/2.f;

// Lighting
glm::vec3 light_pos(1.2f, 1.f, 2.f);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Avoid camera jump to the mouse position from the center
    if (first_mouse)
    {
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos; // reversed: y ranges from bottom to top
    last_mouse_x = xpos;
    last_mouse_y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
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

    GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    // Capture mouse input and hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("shader.vert", "shader.frag");
    Shader lightingCubeShader("light-cube.vert", "light-cube.frag");

    float vertices[] = {
        // Positions          // Normals        // Texture
        // Back
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.0f,  0.0f,
        // Front
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  0.0f,  0.0f,
        // Left
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.f,  0.f,  0.f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,  1.0f,  0.0f,
        // Right
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.f,  0.f,  0.f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,  1.0f,  0.0f,
        // Bottom
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.0f,  1.0f,
        // Top
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.0f,  1.0f
    };
    // Cube
    unsigned int VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Light source cube (we only create this object in order to visualize the position of the source light)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // Use can use the existing VBO since the cube vertices is the same
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Texture 1
    unsigned int diffuse_map;
    glGenTextures(1, &diffuse_map);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("../assets/container.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);

    // Uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        ProcessInput(window);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers

        // Rotating the light source over z axis
        light_pos.x = std::cos(glfwGetTime()) * 2.f;
        light_pos.y = std::sin(glfwGetTime()) * 2.f;

        lightingShader.use();
        // Lighting
        lightingShader.setVec3("view_pos", camera.Position);
        lightingShader.setVec3("light.position", light_pos);
        lightingShader.setVec3("light.ambient", .2f, .2f, .2f);
        lightingShader.setVec3("light.diffuse", .5f, .5f, .5f);
        lightingShader.setVec3("light.specular", 1.f, 1.f, 1.f);
        // Material
        // Each struct member needs to be individually set. The struct name act as a namespace
        lightingShader.setVec3("material.specular", .5f, .5f, .5f); // Specular receives a medium white color
        lightingShader.setFloat("material.shininess", 32.f);

        // Projection transformation
        glm::mat4 projection;
        float aspect_ratio = (float)kScreenWidth/kScreenHeight;
        float near_plane = 0.1f;
        float far_plane = 100.f;
        projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, near_plane, far_plane);
        lightingShader.setMat4("projection", projection);
        // View transformation
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);
        // Model transformation
        glm::mat4 model = glm::mat4(1.f);
        lightingShader.setMat4("model", model);
        // Normal matrix
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model)); // inversing matrices is an expensive operation for shaders, so it's better to do it once from CPU and send it as an uniform
        lightingShader.setMat4("normal_matrix", normal_matrix);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);

        // Drawing object
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing source light cube
        lightingCubeShader.use();
        lightingCubeShader.setMat4("projection", projection);
        lightingCubeShader.setMat4("view", view);
        model = glm::mat4(1.f);
        model = glm::translate(model, light_pos);
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