#include "Shader.h"

Shader::Shader(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath) {
  int success;
  char infoLog[512];

  // read
  string vertexShaderSourceCode;
  string fragmentShaderSourceCode;

  ifstream vifstm;
  ifstream fifstm;

  stringstream vsstm;
  stringstream fsstm;

  vifstm.open(vertexShaderSourcePath);
  fifstm.open(fragmentShaderSourcePath);

  vsstm << vifstm.rdbuf();
  fsstm << fifstm.rdbuf();

  vertexShaderSourceCode = vsstm.str();
  fragmentShaderSourceCode = fsstm.str();

  // vertexShader
  const char* vertexShaderSource = vertexShaderSourceCode.c_str();
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
    cout << "Failed to compile vertex shader\n" << infoLog << endl;
  }

  // fragmentShader
  const char* fragmentShaderSource = fragmentShaderSourceCode.c_str();
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
    cout << "Failed to compile fragment shader\n" << infoLog << endl;
  }

  // shaderProgram
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
    cout << "Failed to link to shader program\n" << infoLog << endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader() {
  glDeleteProgram(shaderProgram);
}


void Shader::use() {
  glUseProgram(shaderProgram);
}

void Shader::setInt(const char* name, int value) {
  glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::setFloat(const char* name, float value) {
  glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::setVec3(const char* name, glm::vec3 value) {
  glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4 value) {
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(value));
}
