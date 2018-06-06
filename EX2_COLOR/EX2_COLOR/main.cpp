//
//  main.cpp
//  EX2_COLOR
//
//  Created by 刘东旭 on 2018/6/6.
//  Copyright © 2018年 刘东旭. All rights reserved.
//

#include <GLFW/glfw3.h>
#include <unistd.h>
#include <vector>
#include <stack>
#include <iostream>
struct point{
    float x;
    float y;
};

void drawPoint(float x, float y);
void drawLine(float x1,float y1, float x2, float y2);

std::vector<point> new_points;
std::stack<point> old_points;
int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
    return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "颜色填充", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    float vertices[] = {
        //边界区域
        -0.5f,   0.5f,
         0.5f,   0.5f,
        -0.5f,  -0.5f,
         0.5f,  -0.5f,
    };
    
    //boat indices
    unsigned int indices[] = {
        0, 2,
        2, 6,
        4, 6,
        4, 0
    };
    float x=0.0f,y=0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        x+=0.01f;
        y+=0.01f;
        //背景色设置
        glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /*your draw*/
        drawPoint(x,y);
        
        for(int i=0;i<8;i+=2){
            drawLine(vertices[indices[i]], vertices[indices[i]+1], vertices[indices[i+1]], vertices[indices[i+1]+1]);
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
        sleep(1);
    }
    
    glfwTerminate();
    return 0;
}

void drawPoint(float x, float y)
{
    /* Draw a point */
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);    // Red
    glVertex2f(x,y);
    glEnd();
}
void drawLine(float x1,float y1, float x2, float y2)
{
    glLineWidth(1);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(x1,y1); //定点坐标范围
    glVertex2f(x2,y2);
    glEnd();
}
