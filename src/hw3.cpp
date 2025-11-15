#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"
#include "matrix.h"
#include <cmath>
#include "3rdparty/glm/glm.hpp"
#include "3rdparty/glm/gtc/matrix_transform.hpp"
#include "3rdparty/glm/gtc/type_ptr.hpp"


using namespace hw3;

const char *vertexShaderSource2 = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 transform;\n"
    "void main() {\n"
    "    gl_Position = transform * vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *vertexShaderSource3 = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 transform;\n"
    "void main() {\n"
    "    gl_Position = transform * vec4(aPos, 1.0);\n"
    "    vertexColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource3 =
    "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(vertexColor, 1.0);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void hw_3_1(const std::vector<std::string> &params) {
    // HW 3.1: Open a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For mac os
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

    // Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to crate GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void hw_3_2(const std::vector<std::string> &params) {
    // HW 3.2: Render a single 2D triangle
    // Window, copied from HW 3.1
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For mac os
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

    // Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to crate GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader);

    

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Triangle rotation
    // Construct the rotation matrix around the Z-axis f
    float angleDegrees = 10;
    float angleRadians = angleDegrees * (M_PI / 180.0f);

    int vertexRotationLocation = glGetUniformLocation(shaderProgram, "transform");

    float start = glfwGetTime();
    

    while (!glfwWindowShouldClose(window)) {
        // Identity matrix
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, float(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(vertexRotationLocation, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    // Window, copied from HW 3.1
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For mac os
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

    // Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to crate GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Build and compile shader program
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource3, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource3, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // For vertices, we have scenes.meshes which is a std::vectro<TriangleMeshes>
    // Create VAO for each mesh
    // then create VBO's for the vertices and colors
    // then create EBO's for the indices
    std::vector<TriangleMesh> meshes = scene.meshes;
    int numMeshes = meshes.size();
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOsVertex;
    std::vector<unsigned int> VBOsColor;
    std::vector<unsigned int> EBOs;
    VAOs.resize(numMeshes);
    VBOsVertex.resize(numMeshes);
    VBOsColor.resize(numMeshes);
    EBOs.resize(numMeshes);

    glGenVertexArrays(numMeshes, VAOs.data());
    glGenBuffers(numMeshes, VBOsVertex.data());
    glGenBuffers(numMeshes, VBOsColor.data());
    glGenBuffers(numMeshes, EBOs.data());

    for (int i = 0; i < numMeshes; i++) {
        // Bind VAO
        glBindVertexArray(VAOs[i]);

        // Bind VBOvertex and set it
        glBindBuffer(GL_ARRAY_BUFFER, VBOsVertex[i]);
        glBufferData(GL_ARRAY_BUFFER, meshes[i].vertices.size() * sizeof(Vector3f), meshes[i].vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
        glEnableVertexAttribArray(0);

        // Bind VBOcolor and set it
        glBindBuffer(GL_ARRAY_BUFFER, VBOsColor[i]);
        glBufferData(GL_ARRAY_BUFFER, meshes[i].vertex_colors.size() * sizeof(Vector3f), meshes[i].vertex_colors.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
        glEnableVertexAttribArray(1);

        // Bind EBO and set it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i].faces.size() * sizeof(Vector3i), meshes[i].faces.data(), GL_DYNAMIC_DRAW);
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Iterate through our VAOs and draw them
        for (int i = 0; i < numMeshes; i++) {
            glUseProgram(shaderProgram);
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, 3 * meshes[i].faces.size(), GL_UNSIGNED_INT, 0);
            
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


}

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}
