#pragma once
#include "global.h"

class Shader {

public:
  Shader(
    const char* vertexShaderSource =
      "#version 330 core\n"
      "void main() {\n"
      "  gl_Position = vec4(0.0);\n"
      "}\0",
    const char* fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "  FragColor = vec4(1.0);\n"
      "}\0"
  );
  ~Shader();

  void use();
  void setInt(const char* name, int value);
  void setFloat(const char* name, float value);
  void setVec3(const char* name, glm::vec3 value);
  void setMat4(const char* name, glm::mat4 value);

private:
  int shaderProgram;

};

