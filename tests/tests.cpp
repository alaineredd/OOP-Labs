#include <gtest/gtest.h>
#include <sstream>
#include "point.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "trapezoid.hpp"
#include "vector.hpp"

using namespace figure;
using namespace vector;

// ==================== Point Tests ====================

TEST(PointTest, DefaultConstructor) {
    Point<int> p;
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<double> p(3.5, 2.7);
    EXPECT_DOUBLE_EQ(p.x, 3.5);
    EXPECT_DOUBLE_EQ(p.y, 2.7);
}

TEST(PointTest, EqualityOperator) {
    Point<int> p1(1, 2);
    Point<int> p2(1, 2);
    Point<int> p3(3, 4);
    
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(PointTest, FloatingPointEquality) {
    Point<double> p1(1.0, 2.0);
    Point<double> p2(1.0 + 1e-10, 2.0 + 1e-10);
    Point<double> p3(1.1, 2.1);
    
    EXPECT_TRUE(p1 == p2);  // Within epsilon
    EXPECT_FALSE(p1 == p3); // Outside epsilon
}

TEST(PointTest, OutputOperator) {
    Point<int> p(5, 10);
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "(5, 10)");
}

TEST(PointTest, InputOperator) {
    Point<int> p;
    std::istringstream iss("15 25");
    iss >> p;
    EXPECT_EQ(p.x, 15);
    EXPECT_EQ(p.y, 25);
}

// ==================== Rectangle Tests ====================

TEST(RectangleTest, DefaultConstructor) {
    Rectangle<double> rect;
    auto center = rect.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.5);
    EXPECT_DOUBLE_EQ(center.y, 0.5);
    EXPECT_DOUBLE_EQ(rect.Area(), 1.0);
}

TEST(RectangleTest, AreaCalculation) {
    Point<double> p1(0, 0), p2(4, 0), p3(4, 3), p4(0, 3);
    Rectangle<double> rect(p1, p2, p3, p4);
    EXPECT_DOUBLE_EQ(rect.Area(), 12.0);
}

TEST(RectangleTest, CenterCalculation) {
    Point<double> p1(1, 1), p2(5, 1), p3(5, 4), p4(1, 4);
    Rectangle<double> rect(p1, p2, p3, p4);
    auto center = rect.Center();
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 2.5);
}

TEST(RectangleTest, EqualityOperator) {
    Point<double> p1(0, 0), p2(2, 0), p3(2, 2), p4(0, 2);
    Rectangle<double> rect1(p1, p2, p3, p4);
    Rectangle<double> rect2(p1, p2, p3, p4);
    
    EXPECT_TRUE(rect1 == rect2);
}

TEST(RectangleTest, CopyConstructor) {
    Rectangle<double> rect1;
    Rectangle<double> rect2(rect1);
    EXPECT_TRUE(rect1 == rect2);
}

TEST(RectangleTest, MoveConstructor) {
    Rectangle<double> rect1;
    Rectangle<double> rect2(std::move(rect1));
    // Should not crash and should be valid
    EXPECT_NO_THROW(rect2.Area());
}

TEST(RectangleTest, OutputOperator) {
    Rectangle<double> rect;
    std::ostringstream oss;
    rect.Print(oss);
    EXPECT_FALSE(oss.str().empty());
    EXPECT_NE(oss.str().find("rectangle:"), std::string::npos);
}

// ==================== Rhombus Tests ====================

TEST(RhombusTest, DefaultConstructor) {
    Rhombus<double> rhombus;
    auto center = rhombus.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.5);
    EXPECT_DOUBLE_EQ(center.y, 0.5);
}

TEST(RhombusTest, AreaCalculation) {
    // Rhombus with diagonals 4 and 6
    Point<double> p1(0, 0);
    Point<double> p2(2, 3);
    Point<double> p3(4, 0);
    Point<double> p4(2, -3);
    Rhombus<double> rhombus(p1, p2, p3, p4);
    EXPECT_DOUBLE_EQ(rhombus.Area(), 12.0); // 0.5 * 4 * 6
}

TEST(RhombusTest, CenterCalculation) {
    Point<double> p1(0, 0);
    Point<double> p2(3, 4);
    Point<double> p3(6, 0);
    Point<double> p4(3, -4);
    Rhombus<double> rhombus(p1, p2, p3, p4);
    auto center = rhombus.Center();
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 0.0);
}

TEST(RhombusTest, CloneMethod) {
    Rhombus<double> rhombus;
    auto clone = rhombus.clone();
    EXPECT_NE(clone, nullptr);
    EXPECT_DOUBLE_EQ(clone->Area(), rhombus.Area());
}

// ==================== Trapezoid Tests ====================

TEST(TrapezoidTest, DefaultConstructor) {
    Trapezoid<double> trapezoid;
    auto center = trapezoid.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.5);
    EXPECT_DOUBLE_EQ(center.y, 0.5);
}

TEST(TrapezoidTest, AreaCalculation) {
    // Trapezoid with bases 4 and 6, height 3
    Point<double> p1(0, 0), p2(4, 0), p3(3, 3), p4(1, 3);
    Trapezoid<double> trapezoid(p1, p2, p3, p4);
    EXPECT_DOUBLE_EQ(trapezoid.Area(), 9.0); // 0.5 * (4 + 6) * 3
}

TEST(TrapezoidTest, InputOutput) {
    Trapezoid<double> trapezoid1;
    std::ostringstream oss;
    trapezoid1.Print(oss);
    
    std::istringstream iss("0 0 4 0 3 3 1 3");
    Trapezoid<double> trapezoid2;
    trapezoid2.Read(iss);
    
    EXPECT_NO_THROW(trapezoid2.Area());
}

// ==================== Figure Interface Tests ====================

TEST(FigureTest, DoubleConversion) {
    Rectangle<double> rect;
    double area = static_cast<double>(rect);
    EXPECT_DOUBLE_EQ(area, rect.Area());
}

TEST(FigureTest, Polymorphism) {
    std::shared_ptr<Figure<double>> rect = std::make_shared<Rectangle<double>>();
    std::shared_ptr<Figure<double>> rhombus = std::make_shared<Rhombus<double>>();
    std::shared_ptr<Figure<double>> trapezoid = std::make_shared<Trapezoid<double>>();
    
    EXPECT_NO_THROW(rect->Area());
    EXPECT_NO_THROW(rhombus->Area());
    EXPECT_NO_THROW(trapezoid->Area());
    
    EXPECT_NO_THROW(rect->Center());
    EXPECT_NO_THROW(rhombus->Center());
    EXPECT_NO_THROW(trapezoid->Center());
}

// ==================== Vector Tests ====================

TEST(VectorTest, DefaultConstructor) {
    Vector<int> vec;
    EXPECT_TRUE(vec.IsEmpty());
    EXPECT_EQ(vec.Size(), 0);
    EXPECT_EQ(vec.Capacity(), 0);
}

TEST(VectorTest, InitializerListConstructor) {
    Vector<int> vec{1, 2, 3, 4, 5};
    EXPECT_EQ(vec.Size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

TEST(VectorTest, PushBack) {
    Vector<int> vec;
    vec.PushBack(10);
    vec.PushBack(20);
    vec.PushBack(30);
    
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(VectorTest, ReserveAndCapacity) {
    Vector<int> vec;
    vec.Reserve(100);
    EXPECT_GE(vec.Capacity(), 100);
    EXPECT_EQ(vec.Size(), 0);
}

TEST(VectorTest, FrontAndBack) {
    Vector<int> vec{1, 2, 3};
    EXPECT_EQ(vec.Front(), 1);
    EXPECT_EQ(vec.Back(), 3);
}

TEST(VectorTest, InsertAndErase) {
    Vector<int> vec{1, 3, 4};
    vec.Insert(1, 2); // Insert at position 1
    EXPECT_EQ(vec.Size(), 4);
    EXPECT_EQ(vec[1], 2);
    
    vec.Erase(1); // Erase element at position 1
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_EQ(vec[1], 3);
}

TEST(VectorTest, Clear) {
    Vector<int> vec{1, 2, 3, 4, 5};
    vec.Clear();
    EXPECT_TRUE(vec.IsEmpty());
    EXPECT_EQ(vec.Size(), 0);
}

TEST(VectorTest, OutOfRangeAccess) {
    Vector<int> vec{1, 2, 3};
    EXPECT_THROW(vec[5], std::out_of_range);
    EXPECT_THROW(vec[10], std::out_of_range);
}

TEST(VectorTest, FigureVectorTotalArea) {
    Vector<std::shared_ptr<Figure<double>>> vec;
    
    auto rect = std::make_shared<Rectangle<double>>();
    auto rhombus = std::make_shared<Rhombus<double>>();
    
    vec.PushBack(rect);
    vec.PushBack(rhombus);
    
    double totalArea = vec.TotalArea<double>();
    double expected = rect->Area() + rhombus->Area();
    
    EXPECT_DOUBLE_EQ(totalArea, expected);
}

TEST(VectorTest, Swap) {
    Vector<int> vec1{1, 2, 3};
    Vector<int> vec2{4, 5, 6};
    
    vec1.swap(vec2);
    
    EXPECT_EQ(vec1[0], 4);
    EXPECT_EQ(vec2[0], 1);
}

TEST(VectorTest, PopBack) {
    Vector<int> vec{1, 2, 3};
    vec.PopBack();
    EXPECT_EQ(vec.Size(), 2);
    EXPECT_EQ(vec.Back(), 2);
    
    vec.PopBack();
    vec.PopBack();
    EXPECT_TRUE(vec.IsEmpty());
    
    // Pop from empty vector should not crash
    EXPECT_NO_THROW(vec.PopBack());
}

// ==================== Exception Safety Tests ====================

TEST(ExceptionTest, VectorEmptyAccess) {
    Vector<int> vec;
    EXPECT_THROW(vec.Front(), std::out_of_range);
    EXPECT_THROW(vec.Back(), std::out_of_range);
    EXPECT_THROW(vec[0], std::out_of_range);
}

// ==================== Integration Tests ====================

TEST(IntegrationTest, VectorOfFigures) {
    Vector<std::shared_ptr<Figure<double>>> figures;
    
    figures.PushBack(std::make_shared<Rectangle<double>>());
    figures.PushBack(std::make_shared<Rhombus<double>>());
    figures.PushBack(std::make_shared<Trapezoid<double>>());
    
    EXPECT_EQ(figures.Size(), 3);
    
    // Test polymorphism
    for (size_t i = 0; i < figures.Size(); ++i) {
        EXPECT_NO_THROW(figures[i]->Area());
        EXPECT_NO_THROW(figures[i]->Center());
    }
    
    // Test total area calculation
    double total = figures.TotalArea<double>();
    EXPECT_GT(total, 0.0);
}

TEST(IntegrationTest, FigureInputOutput) {
    std::stringstream ss;
    
    // Create and output a rectangle
    Rectangle<double> rect;
    rect.Print(ss);
    EXPECT_FALSE(ss.str().empty());
    
    // Test stream operators
    std::ostringstream oss;
    oss << rect;
    EXPECT_FALSE(oss.str().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}