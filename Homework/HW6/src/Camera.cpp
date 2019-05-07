#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 worldUp, float pitch, float yaw, float sensitivity, float speed) {
  this->pos = pos;
  this->worldUp = worldUp;
  this->pitch = pitch;
  this->yaw = yaw;
  this->sensitivity = sensitivity;
  this->speed = speed;
  update();
}

Camera::~Camera() {

}


glm::mat4 Camera::getView() {
  return glm::lookAt(pos, pos + front, up);
}

glm::vec3 Camera::getViewPos() {
  return pos;
}

void Camera::move(const glm::vec3 direction, float deltaTime) {
  glm::vec3 frontXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
  pos += (direction.z * frontXZ + direction.x * right) * deltaTime * speed;
}

void Camera::rotate(double offsetX, double offsetY) {
  yaw += (float) offsetX * sensitivity;
  pitch += (float) offsetY * sensitivity;

  pitch = min(pitch, 89.0f);
  pitch = max(pitch, -89.0f);

  update();
}


void Camera::update() {
  float pitchR = glm::radians(pitch);
  float yawR = glm::radians(-yaw);

  glm::quat quaternion(glm::vec3(pitchR, yawR, 0.0f));
  glm::vec4 front4 = glm::toMat4(quaternion) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

  front = glm::vec3(front4.x, front4.y, front4.z);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
