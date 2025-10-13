#pragma once

#include "image.h"
#include <vector>
#include <string>

Image3 hw_1_1(const std::vector<std::string> &params);
Image3 hw_1_2(const std::vector<std::string> &params);
Image3 hw_1_3(const std::vector<std::string> &params);
Image3 hw_1_4(const std::vector<std::string> &params);
Image3 hw_1_5(const std::vector<std::string> &params);
Image3 hw_1_6(const std::vector<std::string> &params);

bool isInCircle(Real, Real, Vector2, Real);
void renderCircle(Image3&, Vector2, Real, Vector3);

void drawLine(Image3&, Vector3, Vector2, Vector2);
bool isInPoly(Vector2, std::vector<Vector2>);
bool rayIntersectLine(Vector2, Vector2, Vector2);
void renderSimpleShape(Image3&, std::vector<Vector2>, Vector3);
