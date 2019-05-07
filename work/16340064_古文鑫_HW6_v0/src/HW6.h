#pragma once
#include "global.h"
#include "Camera.h"
#include "Shader.h"

Shader PhongShading();
Shader GouraudShading();
void lightRotate(glm::vec3 &lightPos, float deltaTime);
