#include "global.h"
#include "Shader.h"

const int N = 50;

const int SIZE_POINT = 10;
const int SIZE_LINE = 1;
const int SIZE_BEZIER = 5;

struct Point {
  double x;
  double y;
  Point(double _x = 0, double _y = 0) : x(_x), y(_y) {

  }
  Point operator+(const Point p2) const {
    return Point(this->x + p2.x, this->y + p2.y);
  }
  Point operator*(double d) const {
    return Point(this->x * d, this->y * d);
  }
};

void initBezier();
void renderBezier(unsigned int VAO, unsigned int VBO, vector<Point>& points);
void renderAnimation(unsigned int VAO, unsigned int VBO, vector<Point>& points, float time);
