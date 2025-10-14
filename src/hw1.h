#pragma once

#include "image.h"
#include "matrix.h"
#include <optional>
#include <vector>
#include <string>

Image3 hw_1_1(const std::vector<std::string> &params);
Image3 hw_1_2(const std::vector<std::string> &params);
Image3 hw_1_3(const std::vector<std::string> &params);
Image3 hw_1_4(const std::vector<std::string> &params);
Image3 hw_1_5(const std::vector<std::string> &params);
Image3 hw_1_6(const std::vector<std::string> &params);

bool isInCircle(Real, Real, Vector2, Real, Real stroke_width = 0);
void renderCircle(Image3&, Vector2, Real, std::optional<Vector3> = std::nullopt, Real stroke_width = 0, std::optional<Vector3> = std::nullopt);

bool isInPoly(Vector2, std::vector<Vector2>);
bool rayIntersectLine(Vector2, Vector2, Vector2);
void renderSimpleShape(Image3&, std::vector<Vector2>, Vector3);

void drawLines(Image3&, Vector3, Real, std::vector<Vector2>, bool);
void drawLine(Image3&, Vector3, Vector2, Vector2, Real);
bool isInLine(Vector2, Vector2, Vector2, Real);

void renderTransformedCircle(Image3& canvas, Matrix3x3 transform, Real radius, std::optional<Vector3> fill_color, Real stroke_width, std::optional<Vector3> stroke_color, Vector3 background, int super_sample);

std::vector<Vector2> transformPoints(const std::vector<Vector2>& points, const Matrix3x3 transform);

void drawLineSuper(
    Image3& canvas,
    Vector3 color,
    Vector2 point_one,
    Vector2 point_two,
    Real width,
    Vector3 background,
    int super_sample
);

void drawLinesSuper(
    Image3& canvas,
    Vector3 color,
    Real width,
    const std::vector<Vector2>& polylines,
    bool closed,
    Vector3 background,
    int super_sample
);

void renderSimpleShapeSuper(
    Image3& canvas,
    const std::vector<Vector2>& polyline,
    Vector3 fill_color,
    Vector3 background,
    int super_sample = 4
);
