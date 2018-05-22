//
//  Shader.h
//  EX1
//
//  Created by 刘东旭 on 2018/5/21.
//  Copyright © 2018年 刘东旭. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    unsigned int ID; //shaderProgram ID
    Shader(const char* vertexPath, const char* fragmentPath){
        //1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
        try{
            //open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //read file's contents into stream
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //close handlers
            vShaderFile.close();
            fShaderFile.close();
            //convert strem to string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        //2.compile shaders
        unsigned int vertex, fragment;
        //vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        //fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        //shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        //delete shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    //active the shader
    void use(){
        glUseProgram(ID);
    }
    
    void setUniform(const std::string &name,bool value)const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setUniform(const std::string &name,int value)const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setUniform(const std::string &name,float value)const
    {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }
    
private:
    void checkCompileErrors(unsigned int shader, std::string type){
        int sucess;
        char infoLog[1024];
        if(type!="PROGRAM"){
            glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
            if(!sucess){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else{
            glGetProgramiv(shader, GL_LINK_STATUS, &sucess);
            if(!sucess){
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};



#endif /* Shader_h */
