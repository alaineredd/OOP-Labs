#include <cmath>
#include <algorithm>

#include "rhombus.h"

namespace figure {

Rhombus::Rhombus() : vertices{{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}} {}

Rhombus::Rhombus(const Point& p1, const Point& p2, const Point& p3, const Point& p4) : vertices{p1, p2, p3, p4} {}

Rhombus::~Rhombus() = default;

Rhombus& Rhombus::operator=(const Rhombus& other) {
    if (this != &other) {
        for (int i = 0; i < TETRA_VERTICES; ++i) {
            vertices[i] = other.vertices[i];
        }
    }
    return *this;
}

Rhombus& Rhombus::operator=(Rhombus&& other) {
    if (this != &other) {
        for (int i = 0; i < TETRA_VERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
            other.vertices[i] = {0.0, 0.0};
        }
    }
    return *this;
}

Rhombus::operator double() {
    return Area();
}

Point Rhombus::Center() const {
    Point center = {0.0, 0.0};
    for (int i = 0; i < TETRA_VERTICES; ++i) {
        center.x += vertices[i].x;
        center.y += vertices[i].y;
    }
    center.x /= TETRA_VERTICES;
    center.y /= TETRA_VERTICES;
    return center;
}

double Rhombus::Area() const {
    double d1 = std::sqrt(std::pow(vertices[2].x - vertices[0].x, 2) + std::pow(vertices[2].y - vertices[0].y, 2));
    double d2 = std::sqrt(std::pow(vertices[3].x - vertices[1].x, 2) + std::pow(vertices[3].y - vertices[1].y, 2));
    return 0.5 * d1 * d2;
}

Point Rhombus::GetVertex(int index) const {
    if (index < 0 || index >= TETRA_VERTICES) {
        throw std::out_of_range("Vertex index out of range");
    }
    return vertices[index];
}

void Rhombus::Print(std::ostream& out) const {
    out << "rhombus\n";
    for (int i = 0; i < TETRA_VERTICES; ++i) {
        out << vertices[i];
        if (i < TETRA_VERTICES - 1) {
            out << ", ";
        }
    }
}

bool Rhombus::IsValid() const {
    for (int i = 0; i < TETRA_VERTICES; ++i) {
        for (int j = i + 1; j < TETRA_VERTICES; ++j) {
            if (vertices[i] == vertices[j]) {
                return false;
            }
        }
    }
    
    double side1 = std::sqrt(std::pow(vertices[1].x - vertices[0].x, 2) + std::pow(vertices[1].y - vertices[0].y, 2));
    double side2 = std::sqrt(std::pow(vertices[2].x - vertices[1].x, 2) + std::pow(vertices[2].y - vertices[1].y, 2));
    double side3 = std::sqrt(std::pow(vertices[3].x - vertices[2].x, 2) + std::pow(vertices[3].y - vertices[2].y, 2));
    double side4 = std::sqrt(std::pow(vertices[0].x - vertices[3].x, 2) + std::pow(vertices[0].y - vertices[3].y, 2));
    
    return std::abs(side1 - side2) < EPSILON && std::abs(side2 - side3) < EPSILON && std::abs(side3 - side4) < EPSILON;
}

bool operator==(const Rhombus& first, const Rhombus& second) {
    for (int i = 0; i < TETRA_VERTICES; ++i) {
        if (std::abs(first.vertices[i].x - second.vertices[i].x) > EPSILON ||
            std::abs(first.vertices[i].y - second.vertices[i].y) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Rhombus& first, const Rhombus& second) {
    return !(first == second);
}

std::istream &operator>>(std::istream &in, Rhombus& rhombus) {
    for (int i = 0; i < TETRA_VERTICES; ++i) {
        in >> rhombus.vertices[i];
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Rhombus& rhombus) {
    rhombus.Print(out);
}

} // namespace figure