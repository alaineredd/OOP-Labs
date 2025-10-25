#include <gtest/gtest.h>
#include "figure.h"
#include "trapezoid.h"
#include "rhombus.h"
#include "pentagon.h"
#include "vector.h"

// Тесты для Point
TEST(PointTest, DefaultConstructor) {
    figure::Point p;
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}

TEST(PointTest, ValueConstructor) {
    figure::Point p{1.5, 2.5};
    EXPECT_DOUBLE_EQ(p.x, 1.5);
    EXPECT_DOUBLE_EQ(p.y, 2.5);
}

// Тесты для Trapezoid
TEST(TrapezoidTest, DefaultConstructor) {
    figure::Trapezoid t;
    
    // Проверяем конкретные значения вершин, установленные в конструкторе
    EXPECT_DOUBLE_EQ(t.GetVertex(0).x, 0.0);
    EXPECT_DOUBLE_EQ(t.GetVertex(0).y, 0.0);
    
    EXPECT_DOUBLE_EQ(t.GetVertex(1).x, 1.0);
    EXPECT_DOUBLE_EQ(t.GetVertex(1).y, 0.0);
    
    EXPECT_DOUBLE_EQ(t.GetVertex(2).x, 1.0);
    EXPECT_DOUBLE_EQ(t.GetVertex(2).y, 1.0);
    
    EXPECT_DOUBLE_EQ(t.GetVertex(3).x, 0.0);
    EXPECT_DOUBLE_EQ(t.GetVertex(3).y, 1.0);
    
    EXPECT_GT(t.Area(), 0.0);
    
    auto center = t.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.5);
    EXPECT_DOUBLE_EQ(center.y, 0.5); 
}

TEST(TrapezoidTest, ValueConstructor) {
    figure::Point p1{0, 0}, p2{4, 0}, p3{3, 3}, p4{1, 3};
    figure::Trapezoid t(p1, p2, p3, p4);
    
    EXPECT_DOUBLE_EQ(t.GetVertex(0).x, 0);
    EXPECT_DOUBLE_EQ(t.GetVertex(1).x, 4);
}

TEST(TrapezoidTest, CenterCalculation) {
    figure::Point p1{0, 0}, p2{4, 0}, p3{3, 3}, p4{1, 3};
    figure::Trapezoid t(p1, p2, p3, p4);
    
    auto center = t.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 1.5);
}

TEST(TrapezoidTest, AreaCalculation) {
    figure::Point p1{0, 0}, p2{4, 0}, p3{3, 3}, p4{1, 3};
    figure::Trapezoid t(p1, p2, p3, p4);
    
    double area = t.Area();
    EXPECT_NEAR(area, 9.0, 1e-9);
}

TEST(TrapezoidTest, CopyConstructor) {
    figure::Point p1{0, 0}, p2{4, 0}, p3{3, 3}, p4{1, 3};
    figure::Trapezoid t1(p1, p2, p3, p4);
    figure::Trapezoid t2(t1);
    
    EXPECT_TRUE(t1 == t2);
}

TEST(TrapezoidTest, MoveConstructor) {
    figure::Point p1{0, 0}, p2{4, 0}, p3{3, 3}, p4{1, 3};
    figure::Trapezoid t1(p1, p2, p3, p4);
    figure::Trapezoid t2(std::move(t1));
    
    EXPECT_NE(t2.Area(), 0);
}

// Тесты для Rhombus
TEST(RhombusTest, DefaultConstructor) {
    figure::Rhombus r;
    auto center = r.Center();
    EXPECT_NE(center.x, 0);
    EXPECT_NE(center.y, 0);
}

TEST(RhombusTest, AreaCalculation) {
    figure::Point p1{0, 0}, p2{2, 1}, p3{0, 2}, p4{-2, 1};
    figure::Rhombus r(p1, p2, p3, p4);
    
    double area = r.Area();
    EXPECT_NEAR(area, 4.0, 1e-9);
}

TEST(RhombusTest, CenterCalculation) {
    figure::Point p1{0, 0}, p2{2, 1}, p3{0, 2}, p4{-2, 1};
    figure::Rhombus r(p1, p2, p3, p4);
    
    auto center = r.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.0);
    EXPECT_DOUBLE_EQ(center.y, 1.0);
}

// Тесты для Pentagon
TEST(PentagonTest, DefaultConstructor) {
    figure::Pentagon p;
    auto center = p.Center();
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

TEST(PentagonTest, AreaCalculation) {
    figure::Pentagon p;
    double area = p.Area();
    EXPECT_GT(area, 0);
}

TEST(PentagonTest, CenterCalculation) {
    figure::Pentagon p;
    auto center = p.Center();
    EXPECT_NEAR(center.x, 0.0, 1e-9);
    EXPECT_NEAR(center.y, 0.0, 1e-9);
}

// Тесты для Vector
TEST(VectorTest, DefaultConstructor) {
    vector::Vector v;
    EXPECT_TRUE(v.IsEmpty());
    EXPECT_EQ(v.Size(), 0);
    EXPECT_GE(v.Capacity(), vector::DEFAULT_CAPACITY);
}

TEST(VectorTest, InitializerListConstructor) {
    auto* t1 = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    auto* t2 = new figure::Rhombus({0,0}, {2,1}, {0,2}, {-2,1});
    
    vector::Vector v = {t1, t2};
    EXPECT_EQ(v.Size(), 2);
    EXPECT_FALSE(v.IsEmpty());
}

TEST(VectorTest, PushBackAndSize) {
    vector::Vector v;
    auto* t = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    
    v.PushBack(t);
    EXPECT_EQ(v.Size(), 1);
    EXPECT_FALSE(v.IsEmpty());
}

TEST(VectorTest, TotalAreaCalculation) {
    vector::Vector v;
    auto* t = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    auto* r = new figure::Rhombus({0,0}, {2,1}, {0,2}, {-2,1});
    
    v.PushBack(t);
    v.PushBack(r);
    
    double totalArea = v.TotalArea();
    EXPECT_NEAR(totalArea, t->Area() + r->Area(), 1e-9);
}

TEST(VectorTest, ElementAccess) {
    auto* t = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    vector::Vector v = {t};
    
    EXPECT_NO_THROW(v[0]);
    EXPECT_THROW(v[1], std::out_of_range);
}

TEST(VectorTest, FrontAndBack) {
    auto* t1 = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    auto* t2 = new figure::Rhombus({0,0}, {2,1}, {0,2}, {-2,1});
    
    vector::Vector v = {t1, t2};
    
    EXPECT_EQ(&v.Front(), &v[0]);
    EXPECT_EQ(&v.Back(), &v[1]);
}

TEST(VectorTest, InsertAndErase) {
    vector::Vector v;
    auto* t1 = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    auto* t2 = new figure::Rhombus({0,0}, {2,1}, {0,2}, {-2,1});
    
    v.PushBack(t1);
    v.Insert(0, t2);
    
    EXPECT_EQ(v.Size(), 2);
    EXPECT_EQ(&v[0], t2);
    
    v.Erase(0);
    EXPECT_EQ(v.Size(), 1);
    EXPECT_EQ(&v[0], t1);
}

TEST(VectorTest, ReserveAndCapacity) {
    vector::Vector v;
    size_t initial_cap = v.Capacity();
    
    v.Reserve(initial_cap * 2);
    EXPECT_GE(v.Capacity(), initial_cap * 2);
}

TEST(VectorTest, Clear) {
    vector::Vector v;
    auto* t = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    
    v.PushBack(t);
    EXPECT_EQ(v.Size(), 1);
    
    v.Clear();
    EXPECT_EQ(v.Size(), 0);
    EXPECT_TRUE(v.IsEmpty());
}

TEST(VectorTest, PopBack) {
    vector::Vector v;
    auto* t = new figure::Trapezoid({0,0}, {4,0}, {3,3}, {1,3});
    
    v.PushBack(t);
    EXPECT_EQ(v.Size(), 1);
    
    v.PopBack();
    EXPECT_EQ(v.Size(), 0);
}

// Тесты операторов сравнения
TEST(ComparisonTest, TrapezoidEquality) {
    figure::Point p1{0,0}, p2{4,0}, p3{3,3}, p4{1,3};
    figure::Trapezoid t1(p1, p2, p3, p4);
    figure::Trapezoid t2(p1, p2, p3, p4);
    figure::Trapezoid t3({0,0}, {5,0}, {4,3}, {1,3});
    
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
    EXPECT_TRUE(t1 != t3);
}

// Тесты преобразования в double
TEST(ConversionTest, ToDouble) {
    figure::Trapezoid t({0,0}, {4,0}, {3,3}, {1,3});
    figure::Rhombus r({0,0}, {2,1}, {0,2}, {-2,1});
    
    double t_area = static_cast<double>(t);
    double r_area = static_cast<double>(r);
    
    EXPECT_NEAR(t_area, t.Area(), 1e-9);
    EXPECT_NEAR(r_area, r.Area(), 1e-9);
}

// Тесты валидации
TEST(ValidationTest, TrapezoidValidation) {
    figure::Point p1{0,0}, p2{4,0}, p3{3,3}, p4{1,3};
    figure::Trapezoid valid_trapezoid(p1, p2, p3, p4);
    
    figure::Point invalid_p1{0,0}, invalid_p2{0,0}, invalid_p3{3,3}, invalid_p4{1,3};
    figure::Trapezoid invalid_trapezoid(invalid_p1, invalid_p2, invalid_p3, invalid_p4);
    
    EXPECT_TRUE(valid_trapezoid.IsValid());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}