#include <iostream>

#include "./include/pentagon.h"
#include "./include/rhombus.h"
#include "./include/trapezoid.h"
#include "./include/vector.h"

int main() {
    vector::Vector figures;
    auto* trap = new figure::Trapezoid();
    auto* rhomb = new figure::Rhombus();
    auto* penta = new figure::Pentagon();

    figures.PushBack(trap);
    figures.PushBack(rhomb);
    figures.PushBack(penta);
    figures.PrintAllAreas();
}
