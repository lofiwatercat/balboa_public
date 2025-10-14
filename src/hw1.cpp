#include "hw1.h"
#include "hw1_scenes.h"
#include "vector.h"

using namespace hw1;

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]); center = Vector2{x, y}; } else if (params[i] == "-radius") { radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }
    // silence warnings, feel free to remove it
    UNUSED(radius);
    UNUSED(color);


    // Loop through a square where the circle is
    // for (int y = 0; y < radius * 2; y++) {
    //     for (int x = 0; x < radius * 2; x++) {
    //         // isInCircle()            
    //     }
    // }


    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }
    renderCircle(img, center, radius, std::optional<Vector3>{color});
    
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render polylines
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Image3 img(640 /* width */, 480 /* height */);
    std::vector<Vector2> polyline;
    // is_closed = true indicates that the last point and
    // the first point of the polyline are connected
    bool is_closed = false;
    std::optional<Vector3> fill_color;
    std::optional<Vector3> stroke_color;
    Real stroke_width = 1;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-points") {
            while (params.size() > i+1 &&
                    params[i+1].length() > 0 &&
                    params[i+1][0] != '-') {
                Real x = std::stof(params[++i]);
                Real y = std::stof(params[++i]);
                polyline.push_back(Vector2{x, y});
            }
        } else if (params[i] == "--closed") {
            is_closed = true;
        } else if (params[i] == "-fill_color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            fill_color = Vector3{r, g, b};
        } else if (params[i] == "-stroke_color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            stroke_color = Vector3{r, g, b};
        } else if (params[i] == "-stroke_width") {
            stroke_width = std::stof(params[++i]);
        }
    }
    // silence warnings, feel free to remove it
    UNUSED(stroke_width);

    if (fill_color && !is_closed) {
        std::cout << "Error: can't have a non-closed shape with fill color." << std::endl;
        return Image3(0, 0);
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }

    if (is_closed && fill_color) renderSimpleShape(img, polyline, *fill_color);

    if (stroke_color) drawLines(img, *stroke_color, stroke_width, polyline, is_closed);

    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Draw the shapes
    for (auto it = scene.shapes.rbegin(); it != scene.shapes.rend(); it++) {
        auto& shape = *it;
        Vector3 default_color{0.5, 0.5, 0.5};
        if (auto *circle = std::get_if<Circle>(&shape)) {
            // do something with circle
            renderCircle(img, circle->center, circle->radius, circle->fill_color, circle->stroke_width, circle->stroke_color);
        } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
            // do something with polyline
            if (polyline->is_closed && polyline->fill_color) renderSimpleShape(img, polyline->points, polyline->fill_color.value_or(default_color));

            if (polyline->stroke_color) drawLines(img, polyline->stroke_color.value(), polyline->stroke_width, polyline->points, polyline->is_closed);
        }
    }



    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Draw the shapes
    for (auto it = scene.shapes.rbegin(); it != scene.shapes.rend(); it++) {
        auto& shape = *it;
        if (auto *circle = std::get_if<Circle>(&shape)) {
            // do something with circle
            
            renderTransformedCircle(
                img,
                circle->transform,
                circle->radius,
                circle->fill_color,
                circle->stroke_width,
                circle->stroke_color,
                scene.background,
                1);
        } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
            // do something with polyline
            // transform polyline
            std::vector<Vector2> transformed_points = transformPoints(polyline->points, polyline->transform);
            // transform stroke_width
            Real scaleX = std::sqrt(polyline->transform(0,0) * polyline->transform(0,0) + polyline->transform(1,0) * polyline->transform(1,0));
            Real scaleY = std::sqrt(polyline->transform(0,1) * polyline->transform(0,1) + polyline->transform(1,1) * polyline->transform(1,1));
            Real avgScale = (scaleX + scaleY) / 2;
            Real scaledStroke = polyline->stroke_width * avgScale;

            
            if (polyline->is_closed && polyline->fill_color) renderSimpleShape(img, transformed_points, polyline->fill_color.value());

            if (polyline->stroke_color) drawLines(img, polyline->stroke_color.value(), scaledStroke, transformed_points, polyline->is_closed);
        }
    }



    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    // for (int y = 0; y < img.height; y++) {
    //     for (int x = 0; x < img.width; x++) {
    //         img(x, y) = Vector3{1, 1, 1};
    //     }
    // }
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }


    int super_sample = 4;
    // Draw the shapes
    for (auto it = scene.shapes.rbegin(); it != scene.shapes.rend(); it++) {
        auto& shape = *it;
        Vector3 default_color{0.5, 0.5, 0.5};
        if (auto *circle = std::get_if<Circle>(&shape)) {
            // do something with circle
            
            renderTransformedCircle(
                img,
                circle->transform,
                circle->radius,
                circle->fill_color,
                circle->stroke_width,
                circle->stroke_color,
                scene.background,
                super_sample);
        } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
            // do something with polyline
            // transform polyline
            std::vector<Vector2> transformed_points = transformPoints(polyline->points, polyline->transform);
            // transform stroke_width
            Real scaleX = std::sqrt(polyline->transform(0,0) * polyline->transform(0,0) + polyline->transform(1,0) * polyline->transform(1,0));
            Real scaleY = std::sqrt(polyline->transform(0,1) * polyline->transform(0,1) + polyline->transform(1,1) * polyline->transform(1,1));
            Real avgScale = (scaleX + scaleY) / 2;
            Real scaledStroke = polyline->stroke_width * avgScale;

            
            if (polyline->is_closed && polyline->fill_color) renderSimpleShapeSuper(img, transformed_points, polyline->fill_color.value(), scene.background, super_sample);

            if (polyline->stroke_color) drawLinesSuper(img, polyline->stroke_color.value(), scaledStroke, transformed_points, polyline->is_closed, scene.background, super_sample);
        }
    }

    
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

// Takes the canvas, center, radius, and color
void renderCircle(Image3& canvas, Vector2 center, Real radius, std::optional<Vector3> color, Real stroke_width, std::optional<Vector3> stroke_color) {
    Real half_stroke = stroke_width / 2;
    // Make sure we are within the canvas, and include the extra area for strokes
    int x0 = std::max(int(center[0] - radius - half_stroke), 0);
    int x1 = std::min(int(center[0] + radius + half_stroke), canvas.width - 1);
    int y0 = std::max(int(center[1] - radius - half_stroke), 0);
    int y1 = std::min(int(center[1] + radius + half_stroke), canvas.height - 1);
    // Need to find the x and y inside the circle
    for (int i = x0; i <= x1; i++) {
        for (int j = y0; j <= y1; j++) {
            // Flip Y since canvas starts from bottom left
            Real flippedY = canvas.height - 1 - j;
            if (color && isInCircle(i, j, center, radius)) {
                canvas(i, flippedY) = *color;
            }
            if (stroke_color && stroke_width > 0 && isInCircle(i, j, center, radius, stroke_width)) {
                canvas(i, flippedY) = *stroke_color;
            }

        }
    }
}

// Function to see if a pixel is within the circle
bool isInCircle(Real x, Real y, Vector2 center, Real radius, Real stroke_width) {
    // Distance from radius = sqrt(|x - center.x|^2 + |y - center.y|^2)
    Real distance = sqrt(pow(center[0] - x, 2) + pow(center[1] - y, 2));
    if (stroke_width > 0) {
        return distance >= radius - stroke_width / 2 && distance <= radius + stroke_width / 2;
    } else {
        return distance <= radius;
    }
}


// Checks if a pixel is inside a polyline
// point: the pixel location
// polyline: the list of points
bool isInPoly(Vector2 point, std::vector<Vector2> polyline) {
    // For direction
    int count = 0;
    for (int i = 0; i < polyline.size(); i++) {
        Vector2 point_one = polyline[i];
        Vector2 point_two = polyline[(i + 1) % polyline.size()];
        bool intersect = rayIntersectLine(point, point_one, point_two);

        if (point_two.y > point_one.y && intersect) {
            count++;   
        } else if (point_two.y < point_one.y && intersect) {
            count--;
        }
    }

    if (count != 0) {
        return true;
    } else {
        return false;
    }

}

// point is the pixel we cast the ray from, point one and two are the beginning
// and end of the line
bool rayIntersectLine(Vector2 point, Vector2 point_one, Vector2 point_two) {
    Real dx = point_two.x - point_one.x;
    Real dy = point_two.y - point_one.y;


    Real y_min = std::min(point_one.y, point_two.y);
    Real y_max = std::max(point_one.y, point_two.y);

    if (dy == 0) return false;
    if (point.y < y_min || point.y >= y_max) return false;

    // qx + s = p0x + t * dx
    // qy     = p0y + t * dy
    // t = (qy - p0y) / dy
    // s = p0x + t * dx - qx

    Real t = (point.y - point_one.y) / dy;
    Real s = point_one.x + t * dx - point.x;

    if (t >= 0 && t <= 1 && s >= 0) {
        return true;
    }

    return false;
}

void renderSimpleShape(Image3& canvas, std::vector<Vector2> polyline, Vector3 fill_color) {
    for (int i = 0; i < canvas.width; i++) {
        for (int j = 0; j < canvas.height; j++) {
            Vector2 point = Vector2(i, j);
            if (isInPoly(point, polyline)) {
                canvas(i, canvas.height - 1 - j) = fill_color;
            }
        }
    }

}

// Draw lines, takes in
// the canavs, color, width, and the polylines
void drawLines(Image3& canvas, Vector3 color, Real width, std::vector<Vector2> polylines, bool closed) {
    for (int i = 0; i < polylines.size() - 1; i++) {
        Vector2 p0 = polylines[i];
        Vector2 p1 = polylines[i + 1];

        drawLine(canvas, color, p0, p1, width);
    }

    if (closed) {
        drawLine(canvas, color, polylines.back(), polylines.front(), width);
    }
}

// Draws the actual lines given two points
void drawLine(Image3& canvas, Vector3 color, Vector2 point_one, Vector2 point_two, Real width) {
    Real dx = point_two.x - point_one.x;
    Real dy = point_two.y - point_one.y;

    for (int i = 0; i < canvas.width; i++) {
        for (int j = 0; j < canvas.height; j++) {
            Vector2 q;
            q.x = i;
            q.y = j;
            if (isInLine(q, point_one, point_two, width)) {
                canvas(i, canvas.height - 1 -j) = color;
            }
        }
    }

}

// Checks if a pixel at q is in a line or not
bool isInLine(Vector2 q0, Vector2 p0, Vector2 p1, Real width) {
    Vector2 v = p1 - p0;    
    Vector2 wq = q0 - p0;

    Real v_length = std::sqrt(v.x * v.x + v.y * v.y);
    Vector2 v_normalized = v / v_length;

    // Real l = (v.x * wq.x + v.y * wq.y) / v_length;
    Real l = dot(wq, v_normalized);
    l = std::max(0.0, std::min(v_length, l));

    Vector2 q1 = p0 + l * v_normalized;

    Vector2 q_diff = q0 - q1;
    Real q_length = std::sqrt(q_diff.x * q_diff.x + q_diff.y * q_diff.y);

    // Flat caps
    // if (l > 0 && l < v_length && q_length < width/2) {
    //     return true;
    // }

    // Rounded caps
    return (q_length <= width/2);
}

void renderTransformedCircle(
    Image3& canvas,
    Matrix3x3 transform,
    Real radius,
    std::optional<Vector3> fill_color,
    Real stroke_width,
    std::optional<Vector3> stroke_color,
    Vector3 background,
    int super_sample
) {
    // Check if the pixel in the canvas space fits in the object space after transforming
    Matrix3x3 invT = inverse(transform);

    Real pixel_step = 1.0 / super_sample;

    for (int i = 0; i < canvas.width; i++) {
        for (int j = 0; j < canvas.height; j++) {
            // For super sampling
            Vector3 accum_color{0, 0, 0};

            for (int sx = 0; sx < super_sample; sx++) {
                for (int sy = 0; sy < super_sample; sy++) {
                    Real sample_x = i + (sx + 0.5) * pixel_step;
                    Real sample_y = j + (sy + 0.5) * pixel_step;
                    Vector3 c_pixel = Vector3{sample_x, canvas.height - 1 - sample_y, 1.0};
                    Vector3 o_pixel = invT * c_pixel;

                    Real dist = std::sqrt(o_pixel.x * o_pixel.x + o_pixel.y * o_pixel.y);
                    Vector3 sample_color = background;

                    if (fill_color && dist <= radius) {
                        sample_color = *fill_color;
                    } else if (stroke_color && stroke_width > 0 && dist >= radius - stroke_width / 2 && dist <= radius + stroke_width / 2) {
                        sample_color = *stroke_color;
                    }

                    accum_color += sample_color;
                }
            }

            int avg_factor = super_sample * super_sample;
            canvas(i, j) = Vector3{accum_color.x / avg_factor, accum_color.y / avg_factor, accum_color.z / avg_factor};

        }
    }
}

std::vector<Vector2> transformPoints(const std::vector<Vector2>& points, const Matrix3x3 transform) {
   std::vector<Vector2> result;
   result.reserve(points.size());
   for (const auto& p : points) {
       Vector3 tp = transform * Vector3{p.x, p.y, 1.0};
       result.push_back(Vector2{tp.x, tp.y});
   }
   return result;
}

void drawLineSuper(
        Image3& canvas,
        Vector3 color,
        Vector2 point_one,
        Vector2 point_two,
        Real width,
        Vector3 background,
        int super_sample
) {
    Real pixel_step = 1.0 / super_sample;

    for (int i = 0; i < canvas.width; i++) {
        for (int j = 0; j < canvas.height; j++) {
            Vector3 accum_color{0,0,0};

            for (int sx = 0; sx < super_sample; sx++) {
                for (int sy = 0; sy < super_sample; sy++) {
                    Real sample_x = i + (sx + 0.5) * pixel_step;
                    Real sample_y = j + (sy + 0.5) * pixel_step;
                    Real sample_flippedY = canvas.height - 1 - sample_y;
                    Vector2 q{sample_x, sample_flippedY};
                    Vector3 sample_color = background;

                    if (isInLine(q, point_one, point_two, width)) {
                        sample_color = color;
                    }
                    accum_color += sample_color;
                }
            }

            Real avg_factor = super_sample * super_sample;
            Vector3 final_color = accum_color / avg_factor;

            // Blend with existing pixel color to accumulate multiple lines
            canvas(i, canvas.height - 1 - j) = final_color;
        }
    }
}

void drawLinesSuper(
    Image3& canvas,
    Vector3 color,
    Real width,
    const std::vector<Vector2>& polylines,
    bool closed,
    Vector3 background,
    int super_sample
) {
    for (int i = 0; i < polylines.size() - 1; i++) {
        drawLineSuper(canvas, color, polylines[i], polylines[i+1], width, background, super_sample);
    }

    if (closed) {
        drawLineSuper(canvas, color, polylines.back(), polylines.front(), width, background, super_sample);
    }
}

void renderSimpleShapeSuper(
    Image3& canvas,
    const std::vector<Vector2>& polyline,
    Vector3 fill_color,
    Vector3 background,
    int super_sample
) {
    Real pixel_step = 1.0 / super_sample;

    for (int i = 0; i < canvas.width; i++) {
        for (int j = 0; j < canvas.height; j++) {
            Real flippedY = canvas.height - 1 - j;

            Vector3 accum_color{0, 0, 0};

            for (int sx = 0; sx < super_sample; sx++) {
                for (int sy = 0; sy < super_sample; sy++) {
                    Real sample_x = i + (sx + 0.5) * pixel_step;
                    Real sample_y = j + (sy + 0.5) * pixel_step;

                    Vector2 sample_point{sample_x, sample_y};
                    Vector3 sample_color = background; 

                    if (isInPoly(sample_point, polyline)) {
                        sample_color = fill_color; 
                    }

                    accum_color += sample_color;
                }
            }

            int avg_factor = super_sample * super_sample;
            canvas(i, flippedY) = Vector3{
                accum_color.x / avg_factor,
                accum_color.y / avg_factor,
                accum_color.z / avg_factor
            };
        }
    }
}

