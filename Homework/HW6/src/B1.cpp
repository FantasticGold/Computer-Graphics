#include "HW6.h"

const float T = 16.0f;

void lightRotate(glm::vec3 &lightPos, float deltaTime) {
  float angle = deltaTime / T * 360.0f;
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 matRotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), up);
  lightPos = glm::mat3(matRotate) * lightPos;
}


