#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int width = 800;
int height = 800;


float vertices[] = {
    // positions          // colors           // texture coords
    -0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // top right
    -0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   5.0f, 0.0f,   // bottom right
     0.5f,  0.0f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
     0.5f,  0.0f,  0.5f,   1.0f, 1.0f, 0.0f,   5.0f, 0.0f,    // top left 
     0.0f,  0.8f,  0.0f,   0.0f, 0.0f, 1.0f,   2.5f, 5.0f      //tip
};

GLuint indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
    0, 3, 2,    // second triangle
    1, 2, 4,
    0, 1, 4,
    0, 3, 4,
    3, 2, 4
};



int main(void)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    
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

    glViewport(0, 0, width, height);

    Shader shaderprogram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.linkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.linkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    //glEnableVertexAttribArray(2);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderprogram.ID, "scale");
    
    //Textures

    int widthImg, heightImg, numcolCh;

    unsigned char* bytes;
    stbi_set_flip_vertically_on_load(true);
    bytes = stbi_load("pop_cat.png", &widthImg, &heightImg, &numcolCh, 0);
    
    if (!bytes) {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << std::endl;
        return -1; // Handle the error appropriately
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    

    GLuint tex0Uni = glGetUniformLocation(shaderprogram.ID, "tex0");
    shaderprogram.Activate();
    glUniform1i(tex0Uni, 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        /* Render here */
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderprogram.Activate();

        double crntTime = glfwGetTime();

        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        view = glm::translate(view, glm::vec3(0.05f, -0.5f, -2.0f));
        //                    //FOV           //screen aspect ratio  //clip distance 
        proj = glm::perspective(glm::radians(45.0f),(float)(width/height), 0.1f, 100.0f);

        //sending uniform data to shader\

        int modelLoc = glGetUniformLocation(shaderprogram.ID, "model");
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderprogram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shaderprogram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(uniID, 0.5f);
        glBindTexture(GL_TEXTURE_2D,texture);

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteTextures(1, &texture);
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderprogram.Delete();
    glfwTerminate();
    return 0;
}