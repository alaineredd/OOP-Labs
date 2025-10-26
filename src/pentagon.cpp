#include <cmath>
#include <algorithm>

#include "pentagon.h"

namespace figure {

Pentagon::Pentagon() 
    : vertices{{1.0, 0.0}, {0.309, 0.951}, {-0.809, 0.588}, {-0.809, -0.588}, {0.309, -0.951}}
{}

Pentagon::Pentagon(const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Point& p5)
 : vertices{p1, p2, p3, p4, p5} {
}

Pentagon::~Pentagon() = default;

Pentagon& Pentagon::operator=(const Pentagon& other) {
    Pentagon temp(other);
    std::swap(*this, temp);
    return *this;
}

Pentagon& Pentagon::operator=(Pentagon&& other) {
    if (this != &other) {
        for (int i = 0; i < PENTA_VERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
            other.vertices[i] = {0.0, 0.0};
        }
    }
    return *this;
}

Point Pentagon::Center() const {
    Point center = {0.0, 0.0};
    for (int i = 0; i < PENTA_VERTICES; ++i) {
        center.x += vertices[i].x;
        center.y += vertices[i].y;
    }
    center.x /= PENTA_VERTICES;
    center.y /= PENTA_VERTICES;
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
    out << "pentagon";
    for (int i = 0; i < PENTA_VERTICES; ++i) {
        out << vertices[i];
        if (i < PENTA_VERTICES - 1) {
            out << ", ";
        }
    }
}

Point Pentagon::GetVertex(int index) const {
    if (index < 0 || index >= PENTA_VERTICES) {
        throw std::out_of_range("vertex index out of range");
    }
    return vertices[index];
}

bool Pentagon::IsValid() const {
    for (int i = 0; i < PENTA_VERTICES; ++i) {
        for (int j = i + 1; j < PENTA_VERTICES; ++j) {
            if (vertices[i] == vertices[j]) {
                return false;
            }
        }
    }
    
    double slope1 = (vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x);
    double slope2 = (vertices[3].y - vertices[2].y) / (vertices[3].x - vertices[2].x);
    
    return std::abs(slope1 - slope2) < EPSILON;
}

bool operator==(const Pentagon& first, const Pentagon& second) {
    for (int i = 0; i < PENTA_VERTICES; ++i) {
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

std::istream &operator>>(std::istream &in, Pentagon& pentagon) {
    for (int i = 0; i < PENTA_VERTICES; ++i) {
        in >> pentagon.vertices[i];
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Pentagon& pentagon) {
    pentagon.Print(out);
}

} // namespace figure