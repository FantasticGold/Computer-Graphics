#include "HW8.h"

const float T = 8.0f;
const float K = 2.0f;

void renderAnimation(unsigned int VAO, unsigned int VBO, vector<Point>& points, float t) {
  if (points.size() <= 2) return;

  t /= (T + K);
  t -= floor(t);
  t *= (T + K);
  t = min(t / T, 1.0f);

  int n = points.size() - 1;
  vector<Point> bp(points);
  vector<float> dp(3 * n);
  vector<float> dl(6 * (n - 1));
  for (int i = n; i >= 1; --i) {
    for (int j = 0; j < i; ++j) {
      bp[j] = bp[j] * (1 - t) + bp[j + 1] * t;
      dp[3 * j    ] = bp[j].x;
      dp[3 * j + 1] = bp[j].y;
      dp[3 * j + 2] = 0.0f;
    }

    glPointSize(SIZE_BEZIER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * i * 3, &dp[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, i);

    for (int j = 0; j < i - 1; ++j) {
      dl[6 * j    ] = bp[j].x;
      dl[6 * j + 1] = bp[j].y;
      dl[6 * j + 2] = 0.0f;
      dl[6 * j + 3] = bp[j + 1].x;
      dl[6 * j + 4] = bp[j + 1].y;
      dl[6 * j + 5] = 0.0f;
    }

    glPointSize(SIZE_LINE);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (i-1) * 6, &dl[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, (i - 1) * 2);
  }
}