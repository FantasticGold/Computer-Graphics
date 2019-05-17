#pragma once
#include "global.h"

class Shader {

public:
  Shader(const char* vectexShaderSourcePath, const char* fragmentShaderSourcePath);
  ~Shader();

  void use();
  void setInt(const char* name, int value);
  void setFloat(const char* name, float value);
  void setVec3(const char* name, glm::vec3 value);
  void setMat4(const char* name, glm::mat4 value);

private:
  int shaderProgram;

};

