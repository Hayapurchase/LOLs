#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};



int main(void)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    
    if (!window)
    {
        std::cout << "failed to make glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shaderprogram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.linkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        /* Render here */
        glad_glClear(GL_COLOR_BUFFER_BIT);

        shaderprogram.Activate();

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_INT,0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderprogram.Delete();
    glfwTerminate();
    return 0;
}