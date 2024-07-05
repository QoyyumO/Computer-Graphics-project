#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1400;

// Octagonal prism vertices and texture coordinates
float vertices[] = {
    // Positions         // Texture Coords
    // Bottom face
     0.25f, 0.0f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.0f,  0.25f, 1.0f, 0.0f,
     0.5f,  0.0f, -0.25f, 0.0f, 1.0f,
     0.25f, 0.0f, -0.5f,  1.0f, 1.0f,
    -0.25f, 0.0f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.0f, -0.25f, 1.0f, 0.0f,
    -0.5f,  0.0f,  0.25f, 0.0f, 1.0f,
    -0.25f, 0.0f,  0.5f,  1.0f, 1.0f,
    // Top face
     0.25f, 0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.25f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.25f, 0.0f, 1.0f,
     0.25f, 0.5f, -0.5f,  1.0f, 1.0f,
    -0.25f, 0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.25f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.25f, 0.0f, 1.0f,
    -0.25f, 0.5f,  0.5f,  1.0f, 1.0f
};
// Indices for the vertices order
unsigned int indices[] = {
    // Bottom face
    0, 1, 2,
    2, 3, 0,
    3, 4, 5,
    5, 6, 3,
    6, 7, 0,
    0, 3, 6,
    // Top face
    8, 9, 10,
    10, 11, 8,
    11, 12, 13,
    13, 14, 11,
    14, 15, 8,
    8, 11, 14,
    // Side faces
    0, 1, 8,
    1, 9, 8,
    1, 2, 9,
    2, 10, 9,
    2, 3, 10,
    3, 11, 10,
    3, 4, 11,
    4, 12, 11,
    4, 5, 12,
    5, 13, 12,
    5, 6, 13,
    6, 14, 13,
    6, 7, 14,
    7, 15, 14,
    7, 0, 15,
    0, 8, 15
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Octagonal Prism", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("default.vert", "default.frag");

    VAO VAO1;
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.Bind();
    VBO1.Bind();
    EBO1.Bind();

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    Texture texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    texture.texUnit(shader, "tex0", 0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.7f, 0.8f, 0.8f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Activate();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        texture.Bind();

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    texture.Delete();
    shader.Delete();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
