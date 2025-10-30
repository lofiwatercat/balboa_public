#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

bool point_in_polyline_fill(const Vector2 &pt,
                            const std::vector<Vector2> polyline) {
    // the code implicitly assumes that the polyline is closed
    // shoot a ray from "pt" towards right
    // intersect the ray with all lines
    // we count the "winding number" for each intersection: 
    // if the line is going up, winding number +1
    // otherwise winding number -1
    // we then use the "non-zero rule" to determine
    // the result
    int winding_number = 0;
    for (int i = 0; i < (int)polyline.size(); i++) {
        Vector2 p0 = polyline[i];
        Vector2 p1 = polyline[(i + 1) % int(polyline.size())];
        // the ray is pt + t * (1, 0)
        // the line is p0 + s * (p1 - p0)
        // expanding, we have two equations:
        // pt.x + t = p0.x + s * (p1.x - p0.x)
        // pt.y = p0.y + s * (p1.y - p0.y)
        // from the second equation, we know that
        Real s = (pt.y - p0.y) / (p1.y - p0.y);
        // from the first equation, we know that
        Real t = p0.x + s * (p1.x - p0.x) - pt.x;
        if (s > 0 && s < 1 && t > 0) {
            if (p1.y - p0.y > 0) {
                winding_number += 1;
            } else {
                winding_number -= 1;
            }
        }
    }
    // return abs(winding_number) % 2 == 1 // even-odd rule
    return winding_number != 0; // non-zero rule
}

Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single 3D triangle

    Image3 img(640 /* width */, 480 /* height */);

    Vector3 p0{0, 0, -1};
    Vector3 p1{1, 0, -1};
    Vector3 p2{0, 1, -1};
    Real s = 1; // scaling factor of the view frustrum
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    Real z_near = 1e-6; // distance of the near clipping plane
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-p0") {
            p0.x = std::stof(params[++i]);
            p0.y = std::stof(params[++i]);
            p0.z = std::stof(params[++i]);
        } else if (params[i] == "-p1") {
            p1.x = std::stof(params[++i]);
            p1.y = std::stof(params[++i]);
            p1.z = std::stof(params[++i]);
        } else if (params[i] == "-p2") {
            p2.x = std::stof(params[++i]);
            p2.y = std::stof(params[++i]);
            p2.z = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }

    // Check if any if of the z-coordinates are behind the camera
    bool rejected = false;
    std::vector<Vector3> zLine;
    zLine.push_back(p0);
    zLine.push_back(p1);
    zLine.push_back(p2);

    for (Vector3 point : zLine) {
        if (-point.z < z_near) {
            rejected = true;
        }
    }

    

    


    // Calculate the projected points first
    Vector3 p0_1{
        - p0.x / p0.z,
        - p0.y / p0.z,
        -1.0
    };
    Vector3 p1_1{
        - p1.x / p1.z,
        - p1.y / p1.z,
        -1.0
    };
    Vector3 p2_1{
        - p2.x / p2.z,
        - p2.y / p2.z,
        -1.0
    };

    Real aspectRatio = (Real) img.width / img.height;

    // Convert the projected points to the screen space
    Vector2 p0_2{
        (img.width * (p0_1.x + aspectRatio)) / (2 * aspectRatio),
        (img.height * (-p0_1.y + aspectRatio)) / (2 * aspectRatio),
    };
    Vector2 p1_2{
        (img.width * (p1_1.x + aspectRatio)) / (2 * aspectRatio),
        (img.height * (-p1_1.y + aspectRatio)) / (2 * aspectRatio),
    };
    Vector2 p2_2{
        (img.width * (p2_1.x + aspectRatio)) / (2 * aspectRatio),
        (img.height * (-p2_1.y + aspectRatio)) / (2 * aspectRatio),
    };
    std::vector<Vector2> polyline;
    polyline.push_back(p0_2);
    polyline.push_back(p1_2);
    polyline.push_back(p2_2);

    // Draw the img with the new coordinates
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
            Vector2 pixelCenter{x + 0.5, y + 0.5};
            if (!rejected && point_in_polyline_fill(pixelCenter, polyline)) {
                img(x, y) = color;
            }
        }
    }

    return img;
}

// Converts the given point to the projected point 
Vector3 toProjectedPoint(Vector3 point) {
    return Vector3{
        - point.x / point.z,
        - point.y / point.z,
        -1.0
    };
}

// Converts the projected points to the screen space
Vector2 projectedToScreenSpace(Vector3 point, int width, int height) {
    Real aspectRatio = (Real) width / height;
    return Vector2 {
        (width * (point.x + aspectRatio)) / (2 * aspectRatio),
        (height * (-point.y + aspectRatio)) / (2 * aspectRatio)
    };
}

Image3 hw_2_2(const std::vector<std::string> &params) {
    // Homework 2.2: render a triangle mesh

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }
    // silence warnings, feel free to remove these
    UNUSED(s); 
    UNUSED(z_near);
    UNUSED(scene_id);

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }

    // Summary of steps to do
    // 1. Project triangle vertices to screen space
    // 2. If the image plane point is in the triangle
    // 3. Convert current image plane point from screen to projected camera space
    // 4. Compute barycentric coordinates b0', b1', b2'
    // 5. Convet the barycentric coords to the original barycentric coords
    // 6. Obtain depth of the ipp and use the depth to pick the triangle closest to the pixel sample but in front of the clipping plane.
    
    // Check if any if of the z-coordinates are behind the camera
    bool rejected = false;

    // Project triangle vertices
    std::vector<Vector3> projectedVertices;
    for (Vector3 vertex: mesh.vertices) {
        if (-vertex.z < z_near) {
            rejected = true;
        }
        Vector3 projectedVertex = toProjectedPoint(vertex);
        projectedVertices.push_back(projectedVertex);
    }

    // Convert projected points to screen space
    std::vector<Vector2> screenVertices;
    for (Vector3 vertex: projectedVertices) {
        Vector2 screenVertex = projectedToScreenSpace(vertex, img.width, img.height);
        screenVertices.push_back(screenVertex);
    }

    // Create the screen triangles
    std::vector<std::tuple<std::vector<Vector2>, Vector3>> polylines;
    for (int i = 0; i < mesh.faces.size(); i++) {
        std::vector<Vector2> polyline;
        polyline.push_back(screenVertices[mesh.faces[i].x]);
        polyline.push_back(screenVertices[mesh.faces[i].y]);
        polyline.push_back(screenVertices[mesh.faces[i].z]);
        polylines.push_back(std::make_tuple(polyline, mesh.face_colors[i]));
        
    }
    // Check if the image plane point is in the triangle for every triangle
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
            Vector2 pixelCenter{x + 0.5, y + 0.5};
            for (auto &[polyline, faceColor] : polylines) {
                if (!rejected && point_in_polyline_fill(pixelCenter, polyline)) {
                    img(x, y) = faceColor;
                }
            } 
        }
    }






    
    return img;
}  

Image3 hw_2_3(const std::vector<std::string> &params) {
    // Homework 2.3: render a triangle mesh with vertex colors

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }
    // silence warnings, feel free to remove these
    UNUSED(s); 
    UNUSED(z_near);
    UNUSED(scene_id);

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_2_4(const std::vector<std::string> &params) {
    // Homework 2.4: render a scene with transformation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.camera.resolution.x,
               scene.camera.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

