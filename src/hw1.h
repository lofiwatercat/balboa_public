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
