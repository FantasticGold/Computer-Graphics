#pragma once
#include "global.h"

const glm::vec3 initCameraPos(0.0f, 0.0f, 20.0f);

class Camera {

public:
  Camera(glm::vec3 pos = initCameraPos, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
    float pitch = 0.0f, float yaw = 0.0f, float sensitivity = 0.1f, float speed = 5.0f);
  ~Camera();

  glm::mat4 getView();
  glm::vec3 getViewPos();
  void move(const glm::vec3 direction, float deltaTime);
  void rotate(double offsetX, double offsetY);

private:
  glm::vec3 pos, worldUp;
  glm::vec3 front, right, up;
  float pitch, yaw;
  float speed, sensitivity;
  void update();

};

