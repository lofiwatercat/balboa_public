#include "hw1.h"
#include "hw1_scenes.h"

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
    renderCircle(img, center, radius, color);
    
    // test patch
    // for (int i = 0; i < 100; i++) {
    //     for (int j = 0; j < 100; j++) {
    //         int x = 320 + i;
    //         int y = 240 + j;
    //         img(x, y) = Vector3{0.3, 0.7, 0.5};
    //     }
    // }
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

    // Need to render the polyline
    // Cast a ray through the lines, if the pixel hits even, then its out, odd = in
    // Draw a line first

    // For drawing a shape and filling it
    // Loop through the bounding area
    // for (int i = 0; i < img.width; i++) {
    //     for (int j = 0; j < img.height; j++) {
    //         Vector2 point = Vector2(i, j);
    //         if (isInPoly(point, polyline)) {
    //             img(i, img.height - 1 - j) = *fill_color;
    //         }
    //     }
    // }

    if (is_closed) renderSimpleShape(img, polyline, *fill_color);


    
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

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
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
void renderCircle(Image3& canvas, Vector2 center, Real radius, Vector3 color) {
    // Need to find the x and y inside the circle
    for (Real i = center[0] - radius; i < center[0] + radius; i++) {
        for (Real j = center[1] - radius; j < center[1] + radius; j++) {
            if (isInCircle(i, j, center, radius)) {
                // Flip Y since canvas starts from bottom left
                Real flippedY = canvas.height - 1 - j;

                // Make sure that we are within the canvas
                if (i < canvas.width && i > 0 && flippedY < canvas.height && flippedY > 0) {
                    canvas(i, flippedY) = color;
                }
            }
        }
    }
}

// Function to see if a pixel is within the circle
bool isInCircle(Real x, Real y, Vector2 center, Real radius) {
    // Distance from radius = sqrt(|x - center.x|^2 + |y - center.y|^2)
    Real distance = sqrt(pow(center[0] - x, 2) + pow(center[1] - y, 2));
    if (distance < radius) {
        return true;
    } else {
        return false;
    }
}

void drawLine(Image3& canvas, Vector3 color, Vector2 point_one, Vector2 point_two) {
    Real dx = point_two.x - point_one.x;
    Real dy = point_two.y - point_one.y;
    double length = sqrt(dx * dx + dy * dy);

    for (int i = 0; i <= length; i++) {
        Real t = i / length;
        int x = point_one.x + t * dx;
        int y = point_one.y + t * dy;

        canvas(x, canvas.height - 1 - y) = color;
    }

}

// Checks if a pixel is inside a polyline
// point: the pixel location
// polyline: the list of points
bool isInPoly(Vector2 point, std::vector<Vector2> polyline) {
    bool inside = false;
    for (int i = 0; i < polyline.size(); i++) {
        Vector2 point_one = polyline[i];
        Vector2 point_two = polyline[(i + 1) % polyline.size()];
        // Real dx = point_two.x - point_one.x;
        // Real dy = point_two.y - point_one.y;
        // double length = sqrt(dx * dx + dy * dy);
        if (rayIntersectLine(point, point_one, point_two)) {
            inside = !inside;   
        }
    }

    return inside;    
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
