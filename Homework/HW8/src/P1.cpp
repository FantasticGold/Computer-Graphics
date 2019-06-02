#include "HW8.h"

// window
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

// mouse
double mouseX = WINDOW_WIDTH / 2.0f;
double mouseY = WINDOW_HEIGHT / 2.0f;

// points
vector<Point> points;

// radio button
const int POINT_ENABLE = 1;
const int POINT_DISABLE = 2;
const int BEZIER_INVISIBLE = 3;
const int BEZIER_VISIBLE = 4;
const int ANIMATION_FREEZE = 5;
const int ANIMATION_MOVE = 6;

int pointRB = POINT_DISABLE;
int bezierRB = BEZIER_INVISIBLE;
int animationRB = ANIMATION_FREEZE;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // switch
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    pointRB = POINT_DISABLE;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    pointRB = POINT_ENABLE;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    bezierRB = BEZIER_INVISIBLE;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    bezierRB = BEZIER_VISIBLE;
  }
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
    animationRB = ANIMATION_FREEZE;
  }
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
    animationRB = ANIMATION_MOVE;
  }
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
  mouseX = posX;
  mouseY = posY;
}

bool isValid(double x, double y) {
  return x >= 0 && x < WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (pointRB == POINT_ENABLE && action == GLFW_PRESS && isValid(mouseX, mouseY)) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      double viewportX =  mouseX / WINDOW_WIDTH * 2.0f - 1.0f;
      double viewportY = -mouseY / WINDOW_HEIGHT * 2.0f + 1.0f;
      if (points.size() < 50) {
        points.push_back(Point(viewportX, viewportY));
      }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (!points.empty()) {
        points.pop_back();
      }
    }
  }
}

void renderPoints(unsigned int VAO, unsigned int VBO, vector<Point>& points) {
  if (points.empty()) return;

  vector<float> data(3 * points.size());
  for (int i = 0; i < points.size(); ++i) {
    data[3 * i    ] = points[i].x;
    data[3 * i + 1] = points[i].y;
    data[3 * i + 2] = 0.0f;
  }

  glPointSize(SIZE_POINT);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);
  glDrawArrays(GL_POINTS, 0, points.size());
}

void renderLines(unsigned int VAO, unsigned int VBO, vector<Point>& points) {
  if (points.size() <= 1) return;

  vector<float> data(6 * (points.size() - 1));
  for (int i = 0; i < points.size() - 1; ++i) {
    data[6 * i    ] = points[i].x;
    data[6 * i + 1] = points[i].y;
    data[6 * i + 2] = 0.0f;
    data[6 * i + 3] = points[i+1].x;
    data[6 * i + 4] = points[i+1].y;
    data[6 * i + 5] = 0.0f;
  }

  glPointSize(SIZE_LINE);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);
  glDrawArrays(GL_LINES, 0, 2 * (points.size() - 1));
}

int main() {
  // initialize window
  GLFWwindow* window;
  const char* title = "HW8";

  initWindow(window, WINDOW_WIDTH, WINDOW_HEIGHT, title);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  // initialize GLAD
  initGLAD();

  // intialize shaders
  Shader shader("./point.vs", "./point.fs");

  // initialize ImGui
  initImGui(window);

  // points
  initBezier();

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // time
  float animationTime = 0.0f;
  float lastTime = (float)glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    float currTime = (float)glfwGetTime();
    if (animationRB == ANIMATION_MOVE) {
      animationTime += currTime - lastTime;
    }
    lastTime = currTime;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Bezier");

    ImGui::Text("Point");
    ImGui::RadioButton("1.Disable", &pointRB, POINT_DISABLE); ImGui::SameLine();
    ImGui::RadioButton("2.Enable", &pointRB, POINT_ENABLE);

    ImGui::Text("Bezier");
    ImGui::RadioButton("3.Invisible", &bezierRB, BEZIER_INVISIBLE); ImGui::SameLine();
    ImGui::RadioButton("4.Visible", &bezierRB, BEZIER_VISIBLE);

    ImGui::Text("Animation");
    ImGui::RadioButton("5.Freeze", &animationRB, ANIMATION_FREEZE); ImGui::SameLine();
    ImGui::RadioButton("6.Move", &animationRB, ANIMATION_MOVE);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // render
    shader.use();
    renderPoints(VAO, VBO, points);
    renderLines(VAO, VBO, points);

    if (bezierRB == BEZIER_VISIBLE) {
      renderBezier(VAO, VBO, points);
    }

    renderAnimation(VAO, VBO, points, animationTime);

    // display
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clear
  glDeleteBuffers(1, &VAO);
  glDeleteBuffers(1, &VBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

