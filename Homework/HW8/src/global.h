#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
using namespace std;
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initWindow(GLFWwindow* &window, int width, int height, const char* title);
void initGLAD();
void initImGui(GLFWwindow* &window);
void initTexture(unsigned int &texture, const char* image);
