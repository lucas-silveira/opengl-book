#include <glad/gl.h>
#include <GLFW/glfw3.h>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
    // Defining the vertex attributes as well as its local positions within Vertex Shader
    "layout (location = 0) in vec3 aPos;\n" // aPos attribute it's at location 0
    "layout (location = 1) in vec3 aColor;\n" // aColor attribute it's at location 1
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   color = aColor;\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(color, 1.0f);\n"
    "}\0";


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

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Triangle vertices
    float verticesPos[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f,   // top
    };
    // Triangle colors
    float colors[] = {
        1.0f, 0.0f, 0.0f, // left
        0.0f, 1.0f, 0.0f, // right
        0.0f, 0.0f, 1.0f  // top
    };

    unsigned int VBOVerticesPos;
    glGenBuffers(1, &VBOVerticesPos);

    unsigned int VBOColors;
    glGenBuffers(1, &VBOColors);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOVerticesPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPos), verticesPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Copying the colors array into the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Set the vertex attributes pointers (this tells to OpenGL how to handle vertex data in vertex shader)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // here we specify the color attribute (which is at location 1)
    glEnableVertexAttribArray(1); // this is the color layout location in the vertexShaderSource

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawing
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOVerticesPos);
    glDeleteBuffers(1, &VBOColors);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}