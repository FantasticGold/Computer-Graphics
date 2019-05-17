#include "HW7.h"

// window
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

// camera
Camera camera;

float deltaTime = 0.0f;
float lastTime = (float)glfwGetTime();

// move
glm::vec3 direction(0.0f, 0.0f, 0.0f);

bool firstMouse = true;
double lastX = WINDOW_WIDTH / 2.0f;
double lastY = WINDOW_HEIGHT / 2.0f;

float fov = 45.0f;
const float MAX_FOV = 45.0f;
const float MIN_FOV = 1.0f;

// radio button
const int VIEWRB_FREEZE = 1;
const int VIEWRB_MOVE = 2;

int viewRB = VIEWRB_FREEZE;

const int PROJECTION_ORTHOGRAPHIC = 3;
const int PROJECTION_PERSPECTIVE = 4;

int projRB = PROJECTION_ORTHOGRAPHIC;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // switch
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    viewRB = VIEWRB_FREEZE;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    viewRB = VIEWRB_MOVE;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    projRB = PROJECTION_ORTHOGRAPHIC;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    projRB = PROJECTION_PERSPECTIVE;
  }

  // move
  direction = glm::vec3(0.0f, 0.0f, 0.0f);
  if (viewRB == VIEWRB_FREEZE) {
    return;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP)) {
    direction.z += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN)) {
    direction.z -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT)) {
    direction.x -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT)) {
    direction.x += 1.0f;
  }
}
void mouse_callback(GLFWwindow* window, double posX, double posY) {
  if (firstMouse) {
    firstMouse = false;
    lastX = posX;
    lastY = posY;
  }

  double offsetX = posX - lastX;
  double offsetY = -(posY - lastY);

  lastX = posX;
  lastY = posY;

  camera.rotate(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  fov -= yoffset;
  fov = min(MAX_FOV, max(MIN_FOV, fov));
}

int main() {
  // initialize window
  GLFWwindow* window;
  const char* title = "HW7";

  initWindow(window, WINDOW_WIDTH, WINDOW_HEIGHT, title);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // initialize GLAD
  initGLAD();

  // intialize shaders
  Shader objectShader("./object.vs", "./object.fs");
  Shader lightShader("./light.vs", "./light.fs");
  Shader shadowShader("./shadow.vs", "./shadow.fs");

  // initialize ImGui
  initImGui(window);

  // texture
  unsigned int texture;
  initTexture(texture, "./container.jpg");

  objectShader.use();
  objectShader.setInt("diffuseTexture", 0);
  objectShader.setInt("shadowMap", 1);

  // shadow
  GLuint depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);
  const GLuint SHADOW_WIDTH = 1024;
  const GLuint SHADOW_HEIGHT = 1024;

  GLuint depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 
    0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  GLfloat borderColor[] = {
    1.0f, 1.0f, 1.0f, 1.0f
  };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  // light
  const float near_plane = 0.1f;
  const float far_plane = 100.0f;

  glm::vec3 lightPos = glm::vec3(0.0f, 8.0f, 1.0f);
  glm::mat4 lightOrthoProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  glm::mat4 lightPersProjection = glm::perspective(glm::radians(45.0f),
    (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);

  // objects
  glEnable(GL_DEPTH_TEST);

  float cube[] = {
    /*
    vec3                  vec2         vec3               vec3
    position              texture      color              normal
    */

    // behind
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f, -2.0f, -2.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,

    // front
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
    -2.0f,  2.0f,  2.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,

    // left
    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

    // right
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,

     // bottom
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,

    // top
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    -2.0f,  2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f
  };

  unsigned int cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  unsigned int cubeVBO;
  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  float plane[] = {
    /*
    vec3                    vec2           vec3               vec3
    position                texture        color              normal
    */
     20.0f, -6.0f,  20.0f,  20.0f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -20.0f, -6.0f, -20.0f,   0.0f, 20.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -20.0f, -6.0f,  20.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

     20.0f, -6.0f,  20.0f,  20.0f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     20.0f, -6.0f, -20.0f,  20.0f, 20.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -20.0f, -6.0f, -20.0f,   0.0f, 20.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f
  };

  unsigned int planeVAO;
  glGenVertexArrays(1, &planeVAO);
  glBindVertexArray(planeVAO);

  unsigned int planeVBO;
  glGenBuffers(1, &planeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glm::mat4 currentView = camera.getView();
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // move
    float currTime = (float)glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
    camera.move(direction, deltaTime);

    // clear
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // transform
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
    projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    if (viewRB == VIEWRB_FREEZE) {
      view = currentView;
    }
    else if (viewRB == VIEWRB_MOVE) {
      view = camera.getView();
      currentView = view;
    }

    glm::mat4 lightModel = glm::mat4(1.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    lightShader.use();
    lightShader.setMat4("model", lightModel);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);

    shadowShader.use();
    shadowShader.setMat4("model", model);
    if (projRB == PROJECTION_ORTHOGRAPHIC) {
      shadowShader.setMat4("lightSpace", lightOrthoProjection * lightView);
    }
    else if (projRB == PROJECTION_PERSPECTIVE) {
      shadowShader.setMat4("lightSpace", lightPersProjection * lightView);
    }

    objectShader.use();
    objectShader.setMat4("model", model);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);
    objectShader.setVec3("lightPos", lightPos);
    if (projRB == PROJECTION_ORTHOGRAPHIC) {
      objectShader.setMat4("lightSpace", lightOrthoProjection * lightView);
    }
    else if (projRB == PROJECTION_PERSPECTIVE) {
      objectShader.setMat4("lightSpace", lightPersProjection * lightView);
    }
    objectShader.setVec3("viewPos", camera.getViewPos());

    // shadow
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    shadowShader.use();
    glCullFace(GL_FRONT);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    objectShader.use();
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    lightShader.use();
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Shadow");

    ImGui::Text("View");
    ImGui::RadioButton("1.Freeze", &viewRB, VIEWRB_FREEZE); ImGui::SameLine();
    ImGui::RadioButton("2.Move", &viewRB, VIEWRB_MOVE);

    ImGui::Text("Projection");
    ImGui::RadioButton("3.Orthographic", &projRB, PROJECTION_ORTHOGRAPHIC); ImGui::SameLine();
    ImGui::RadioButton("4.Perspective", &projRB, PROJECTION_PERSPECTIVE);

    ImGui::Text("Light");
    ImGui::SliderFloat("x", &lightPos.x, -10.0, 10.0);
    ImGui::SliderFloat("y", &lightPos.y, -10.0, 10.0);
    ImGui::SliderFloat("z", &lightPos.z, -10.0, 10.0);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (viewRB == VIEWRB_FREEZE) {
      firstMouse = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (viewRB == VIEWRB_MOVE) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // display
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clear
  glDeleteBuffers(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVAO);
  glDeleteBuffers(1, &planeVBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

