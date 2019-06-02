#include "HW8.h"

const int M = 1000;

long long C[N][N];

void initBezier() {
  for (int i = 0; i < N; ++i) {
    C[0][i] = 1;
    C[i][i] = 1;
  }
  for (int i = 1; i < N; ++i) {
    for (int j = 1; j < i; ++j) {
      C[j][i] = C[j - 1][i - 1] + C[j][i - 1];
    }
  }
}

double B(int i, int n, double t) {
  return C[i][n] * pow(t, i) * pow(1 - t, n - i);
}

Point bezier(vector<Point>& points, double t) {
  Point point;

  int n = points.size() - 1;
  for (int i = 0; i <= n; ++i) {
    point = point + points[i] * B(i, n, t);
  }

  return point;
}

void renderBezier(unsigned int VAO, unsigned int VBO, vector<Point>& points) {
  if (points.size() <= 1) return;

  vector<float> data(3 * (M + 1));
  for (int i = 0; i <= M; ++i) {
    Point point = bezier(points, 1.0f * i / M);
    data[3 * i    ] = point.x;
    data[3 * i + 1] = point.y;
    data[3 * i + 2] = 0.0f;
  }

  glPointSize(SIZE_LINE);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);
  glDrawArrays(GL_POINTS, 0, M + 1);
}