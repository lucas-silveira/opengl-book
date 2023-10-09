#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
  public:
    // Program ID
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    // Activate the program
    void use();

    // Delete the program
    void remove();

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif