//
//  main.cpp
//  EX1
//
//  Created by 刘东旭 on 2018/5/21.
//  Copyright © 2018年 刘东旭. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setTexture(unsigned int*, const char* filename);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//位移 旋转
glm::vec3 move(0.0f,0.0f,0.0f);
float radian = 0.0f;
glm::vec3 rot(0.0f,0.0f,1.0f);
glm::vec3 scl(1.0f, 1.0f, 1.0f);

//裁剪
bool isCut = false;

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
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
	Shader frontShader("shader.v", "shader.f");
	Shader backShader("shader_box.v", "shader_box.f");

	float triangle[] = {
		//三角形
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // 右下
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,    // 顶部
	};

	unsigned int tri_indices[] = {
		0, 1, 2,
	};


	//boat verteices
	float boat_vertices[] = {
		 //位置					//颜色		
		 0.0f,  0.44f, 0.0f,    1.0f, 0.4f, 0.2f,
		 0.0f, -0.5f, 0.0f,		1.0f, 0.4f, 0.2f,
		 0.4f, -0.5f, 0.0f,		1.0f, 0.4f, 0.2f,

		-0.08f,  0.2f, -0.1f,	0.8f, 0.4f, 0.2f,
		-0.08f, -0.5f, -0.1f,	1.0f, 0.4f, 0.2f,
		-0.38f, -0.5f, -0.1f,	1.0f, 0.4f, 0.2f,
		 
		-0.5f, -0.55f, -0.1f,	1.0f, 0.4f, 0.2f,
		 0.5f, -0.55f, -0.1f,	1.0f, 0.4f, 0.2f,
		 0.0f, -1.0f,  -0.1f,	0.8f, 0.4f, 0.2f,

	};

	//boat indices
	unsigned int boat_indices[] = {
		3, 4, 5,
		6, 7, 8,
		0, 1, 2,
	};

	// cut verteices
	float cut_vertices[] = {
		//边界区域
		-0.4f,   0.3f, 0.0f,	0.0f, 0.5f, 0.0f,	//左上
		 0.4f,   0.3f, 0.0f,	0.0f, 0.5f, 0.0f,	//右上
		-0.4f,  -0.6f, 0.0f,	0.0f, 0.5f, 0.0f,	//左下
		 0.4f,  -0.6f, 0.0f,	0.0f, 0.5f, 0.0f,	//右下
	};
	
	//boat indices
	unsigned int cut_indices[] = {
		0, 1, 2,
		1, 2, 3
	};


    
	//设置顶点、颜色及纹理坐标
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle), triangle,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_indices), tri_indices, GL_DYNAMIC_DRAW);

	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//设置裁剪区域
	unsigned int VAO1, VBO1, EBO1;

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cut_vertices), cut_vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cut_indices), cut_indices, GL_DYNAMIC_DRAW);

	//顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // render
        // ------
		
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		if (isCut) {
			backShader.use();
			glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0); // no need to unbind it every time
		}
		// active the shaderPorgram
		frontShader.use();
		
		glm::mat4 trans;
		trans = glm::translate(trans, move);
		trans = glm::rotate(trans, radian, rot);
		trans = glm::scale(trans, scl);
		frontShader.setMat4("transform", trans);
		frontShader.setBool("isCut",isCut); //vec4(-0.4f, 0.4f, -0.6f, 0.3f
		frontShader.setVec4("cut", glm::vec4(-0.4f, 0.4f, -0.6f, 0.3f));
		//frontShader.setMat4("cut", glm::ortho(-0.4f, 0.4f, -0.6f, 0.3f));
		//frontShader.setVec4("cut", glm::vec4(-0.4f, 0.4f, -0.6f, 0.3f));
        
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L)==GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F)==GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
		move.x += 0.02;
    }
    if (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS){
		move.x -= 0.02;
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move.y += 0.02;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move.y -= 0.02;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		radian += 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		radian -= 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		scl.x += 0.02f;
		scl.y += 0.02f;
		scl.z += 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		scl.x -= 0.02f;
		scl.y -= 0.02f;
		scl.z -= 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		isCut = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		isCut = false;
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

void setTexture(unsigned int* texture, const char* filename) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载并生成纹理
	stbi_set_flip_vertically_on_load(true);
	int t_width, t_height, t_nrChannels;
	unsigned char* t_data = stbi_load(filename, &t_width, &t_height, &t_nrChannels, 0);
	if (t_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, t_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(t_data);
}
