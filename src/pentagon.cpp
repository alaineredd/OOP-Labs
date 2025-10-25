#include <cmath>
#include <algorithm>

#include "../include/pentagon.h"

namespace figure {

Pentagon::Pentagon() {
    vertices[0] = Point(1.0, 0.0);
    vertices[1] = Point(0.309, 0.951);
    vertices[2] = Point(-0.809, 0.588);
    vertices[3] = Point(-0.809, -0.588);
    vertices[4] = Point(0.309, -0.951);
}

Pentagon::Pentagon(const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Point& p5) {
    vertices[0] = p1;
    vertices[1] = p2;
    vertices[2] = p3;
    vertices[3] = p4;
    vertices[4] = p5;
}

Pentagon::Pentagon(const Pentagon& other) {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        vertices[i] = other.vertices[i];
    }
}

Pentagon::Pentagon(Pentagon&& other) {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        vertices[i] = std::move(other.vertices[i]);
        other.vertices[i] = {0.0, 0.0};
    }
}

Pentagon::~Pentagon() = default;

Pentagon& Pentagon::operator=(const Pentagon& other) {
    if (this != &other) {
        for (int i = 0; i < PENTAVERTICES; ++i) {
            vertices[i] = other.vertices[i];
        }
    }
    return *this;
}

Pentagon& Pentagon::operator=(Pentagon&& other) {
    if (this != &other) {
        for (int i = 0; i < PENTAVERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
            other.vertices[i] = {0.0, 0.0};
        }
    }
    return *this;
}

Point Pentagon::Center() const {
    Point center = {0.0, 0.0};
    for (int i = 0; i < PENTAVERTICES; ++i) {
        center.x += vertices[i].x;
        center.y += vertices[i].y;
    }
    center.x /= PENTAVERTICES;
    center.y /= PENTAVERTICES;
    return center;
}

double Pentagon::Area() const {
    double area = 0.0;
    for (int i = 0; i < 5; ++i) { // какая то формула Гаусса хз нашёл в инете 
        int j = (i + 1) % 5;
        area += vertices[i].x * vertices[j].y;
        area -= vertices[j].x * vertices[i].y;
    }
    return std::abs(area) / 2.0;
}

Pentagon::operator double() {
    return Area();
}

void Pentagon::Print(std::ostream& out) const {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        out << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i < PENTAVERTICES - 1) out << ", ";
    }
}

Point Pentagon::GetVertex(int index) const {
    if (index < 0 || index >= PENTAVERTICES) {
        throw std::out_of_range("vertex index out of range");
    }
    return vertices[index];
}

bool Pentagon::IsValid() const {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        for (int j = i + 1; j < PENTAVERTICES; ++j) {
            if (vertices[i].x == vertices[j].x && vertices[i].y == vertices[j].y) {
                return false;
            }
        }
    }
    
    double slope1 = (vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x);
    double slope2 = (vertices[3].y - vertices[2].y) / (vertices[3].x - vertices[2].x);
    
    return std::abs(slope1 - slope2) < EPSILON;
}

bool operator==(const Pentagon& first, const Pentagon& second) {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        if (std::abs(first.vertices[i].x - second.vertices[i].x) > EPSILON ||
            std::abs(first.vertices[i].y - second.vertices[i].y) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Pentagon& first, const Pentagon& second) {
    return !(first == second);
}

std::istream &operator>>(std::istream &in, Pentagon& Pentagon) {
    for (int i = 0; i < PENTAVERTICES; ++i) {
        in >> Pentagon.vertices[i].x >> Pentagon.vertices[i].y;
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Pentagon& Pentagon) {
    out << "Pentagon vertices: ";
    for (int i = 0; i < PENTAVERTICES; ++i) {
        out << "(" << Pentagon.vertices[i].x << ", " << Pentagon.vertices[i].y << ")";
        if (i < PENTAVERTICES - 1) out << " ";
    }
    return out;
}

} // namespace figure