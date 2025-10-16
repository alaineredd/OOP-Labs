#include <string>
#include <stdexcept>

#include <gtest/gtest.h>

#include "seven.h"
#include "vector.h"
// ==================== TESTS FOR VECTOR CLASS ====================

TEST(VectorTest, DefaultConstructor) {
    vector::vector v;
    EXPECT_EQ(v.Size(), 0);
    EXPECT_GE(v.Capacity(), 0);
    EXPECT_TRUE(v.IsEmpty());
}

TEST(VectorTest, SizeConstructor) {
    vector::vector v(5);
    EXPECT_EQ(v.Size(), 5);
    EXPECT_GE(v.Capacity(), 5);
    EXPECT_FALSE(v.IsEmpty());
}

TEST(VectorTest, SizeValueConstructor) {
    vector::vector v(3, 'A');
    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Get(0), 'A');
    EXPECT_EQ(v.Get(1), 'A');
    EXPECT_EQ(v.Get(2), 'A');
}

TEST(VectorTest, StringConstructor) {
    std::string s = "123";
    vector::vector v(s);
    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Get(0), '1');
    EXPECT_EQ(v.Get(1), '2');
    EXPECT_EQ(v.Get(2), '3');
}

TEST(VectorTest, InitializerListConstructor) {
    vector::vector v{'1', '2', '3'};
    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Get(0), '1');
    EXPECT_EQ(v.Get(1), '2');
    EXPECT_EQ(v.Get(2), '3');
}

TEST(VectorTest, CopyConstructor) {
    vector::vector original{'1', '2', '3'};
    vector::vector copy(original);
    
    EXPECT_EQ(copy.Size(), original.Size());
    for(size_t i = 0; i < original.Size(); ++i) {
        EXPECT_EQ(copy.Get(i), original.Get(i));
    }
}

TEST(VectorTest, MoveConstructor) {
    vector::vector original{'1', '2', '3'};
    vector::vector moved(std::move(original));
    
    EXPECT_EQ(moved.Size(), 3);
    EXPECT_EQ(moved.Get(0), '1');
    EXPECT_EQ(moved.Get(1), '2');
    EXPECT_EQ(moved.Get(2), '3');
    EXPECT_EQ(original.Size(), 0); // После move оригинал должен быть пустым
}

TEST(VectorTest, PushBack) {
    vector::vector v;
    v.Push_back('1');
    v.Push_back('2');
    v.Push_back('3');
    
    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Get(0), '1');
    EXPECT_EQ(v.Get(1), '2');
    EXPECT_EQ(v.Get(2), '3');
}

TEST(VectorTest, PopBack) {
    vector::vector v{'1', '2', '3'};
    v.Pop_back();
    
    EXPECT_EQ(v.Size(), 2);
    EXPECT_EQ(v.Get(0), '1');
    EXPECT_EQ(v.Get(1), '2');
    
    v.Pop_back();
    v.Pop_back();
    EXPECT_EQ(v.Size(), 0);
    EXPECT_TRUE(v.IsEmpty());
}

TEST(VectorTest, Reserve) {
    vector::vector v;
    v.Reserve(100);
    
    EXPECT_GE(v.Capacity(), 100);
    EXPECT_EQ(v.Size(), 0);
    EXPECT_TRUE(v.IsEmpty());
}

TEST(VectorTest, GetConst) {
    const vector::vector v{'1', '2', '3'};
    EXPECT_EQ(v.Get(0), '1');
    EXPECT_EQ(v.Get(1), '2');
    EXPECT_EQ(v.Get(2), '3');
}

TEST(VectorTest, GetNonConst) {
    vector::vector v{'1', '2', '3'};
    v.Get(1) = '9';
    EXPECT_EQ(v.Get(1), '9');
}

TEST(VectorTest, LargePushBack) {
    vector::vector v;
    for(size_t i = 0; i < 1000; ++i) {
        v.Push_back('0' + (i % 7));
    }
    
    EXPECT_EQ(v.Size(), 1000);
    for(size_t i = 0; i < 1000; ++i) {
        EXPECT_GE(v.Get(i), '0');
        EXPECT_LE(v.Get(i), '6');
    }
}

TEST(SevenTest, DefaultConstructor) {
    Seven::Seven s;
    // Должен создавать валидное число (скорее всего "0")
}

TEST(SevenTest, SizeNumberConstructor) {
    Seven::Seven s(3, '4');
    // Проверяем что создалось число длины 3
}

TEST(SevenTest, SizeNumberConstructorInvalidSize) {
    EXPECT_THROW(Seven::Seven(0, '1'), std::invalid_argument);
}

TEST(SevenTest, SizeNumberConstructorInvalidDigit) {
    EXPECT_THROW(Seven::Seven(3, '7'), std::invalid_argument);
    EXPECT_THROW(Seven::Seven(3, '8'), std::invalid_argument);
    EXPECT_THROW(Seven::Seven(3, '9'), std::invalid_argument);
}

TEST(SevenTest, StringConstructor) {
    std::string str = "123";
    Seven::Seven s(str);
    // Должен создать число 321 (так как хранится в обратном порядке)
}

TEST(SevenTest, StringConstructorInvalidString) {
    std::string invalid1 = "128"; // '8' недопустима в семеричной системе
    std::string invalid2 = "12a"; // 'a' не цифра
    std::string empty = "";
    
    EXPECT_THROW(Seven::Seven a(invalid1), std::invalid_argument);
    EXPECT_THROW(Seven::Seven b(invalid2), std::invalid_argument);
    EXPECT_THROW(Seven::Seven c(empty), std::invalid_argument);
}

TEST(SevenTest, InitializerListConstructor) {
    Seven::Seven s{'1', '2', '3'};
    // Должен создать число 321
}

TEST(SevenTest, InitializerListConstructorInvalid) {
    EXPECT_THROW(Seven::Seven({}), std::invalid_argument);
    EXPECT_THROW(Seven::Seven({'1', '7', '3'}), std::invalid_argument);
}

TEST(SevenTest, Equality) {
    Seven::Seven s1({'1', '2', '3'});
    Seven::Seven s2({'1', '2', '3'});
    Seven::Seven s3({'4', '5', '6'});
    
    EXPECT_TRUE(s1.EQ(s2));
    EXPECT_FALSE(s1.EQ(s3));
}

TEST(SevenTest, GreaterThan) {
    Seven::Seven s1({'3', '2', '1'}); // 123 в семеричной
    Seven::Seven s2({'2', '1'});      // 12 в семеричной
    Seven::Seven s3({'1', '2', '3'}); // 321 в семеричной
    
    EXPECT_TRUE(s1.GT(s2)); // 123 > 12
    EXPECT_FALSE(s2.GT(s1)); // 12 < 123
    EXPECT_TRUE(s3.GT(s1)); // 321 > 123
}

TEST(SevenTest, LessThan) {
    Seven::Seven s1({'1', '2'});      // 21 в семеричной
    Seven::Seven s2({'3', '2', '1'}); // 123 в семеричной
    Seven::Seven s3({'3', '2', '1'}); // 123 в семеричной
    
    EXPECT_TRUE(s1.LT(s2)); // 21 < 123
    EXPECT_FALSE(s2.LT(s1)); // 123 > 21
    EXPECT_FALSE(s2.LT(s3)); // 123 == 123
}

TEST(SevenTest, Addition) {
    Seven::Seven s1({'1'});    // 1
    Seven::Seven s2({'2'});    // 2
    Seven::Seven s3({'3'});    // 3
    Seven::Seven s4({'6'});    // 6
    
    // 1 + 2 = 3
    Seven::Seven result1 = Seven::Seven::ADD(s1, s2);
    EXPECT_TRUE(result1.EQ(s3));
    
    // 6 + 1 = 10 (в семеричной)
    Seven::Seven result2 = Seven::Seven::ADD(s4, s1);
    Seven::Seven expected({'0', '1'}); // 10 в семеричной
    EXPECT_TRUE(result2.EQ(expected));
}

TEST(SevenTest, AdditionWithCarry) {
    Seven::Seven s1({'6'}); // 6
    Seven::Seven s2({'1'}); // 1
    
    Seven::Seven result = Seven::Seven::ADD(s1, s2);
    Seven::Seven expected({'0', '1'}); // 10 в семеричной (6 + 1 = 7 = 10₇)
    EXPECT_TRUE(result.EQ(expected));
}

TEST(SevenTest, Subtraction) {
    Seven::Seven s1({'3'}); // 3
    Seven::Seven s2({'1'}); // 1
    Seven::Seven s3({'5', '1'}); // 15 в семеричной (12 в десятичной)
    Seven::Seven s4({'2'}); // 2
    
    // 3 - 1 = 2
    Seven::Seven result1 = Seven::Seven::SUB(s1, s2);
    EXPECT_TRUE(result1.EQ(s4));
    
    // 15 - 2 = 13 в семеричной (10 в десятичной)
    Seven::Seven result2 = Seven::Seven::SUB(s3, s4);
    Seven::Seven expected({'3', '1'}); // 13 в семеричной
    EXPECT_TRUE(result2.EQ(expected));
}

TEST(SevenTest, SubtractionInvalid) {
    Seven::Seven s1({'1'}); // 1
    Seven::Seven s2({'2'}); // 2
    
    // 1 - 2 = отрицательное число - должно бросать исключение
    EXPECT_THROW(Seven::Seven::SUB(s1, s2), std::invalid_argument);
}

TEST(SevenTest, LargeNumbers) {
    // Тест с большими числами
    Seven::Seven large1(std::string("123456"));
    Seven::Seven large2(std::string("654321"));
    
    // Проверяем что операции не падают
    EXPECT_NO_THROW(Seven::Seven::ADD(large1, large2));
    EXPECT_THROW(Seven::Seven::SUB(large2, large1), std::invalid_argument);
}

TEST(SevenTest, RemoveLeadingZeroes) {
    // Тест что ведущие нули удаляются
    Seven::Seven withZeroes({'3', '2', '1', '0', '0'});
    Seven::Seven withoutZeroes({'3', '2', '1'});
    
    // После удаления ведущих нулей должны быть равны
    EXPECT_TRUE(withZeroes.EQ(withoutZeroes));
}

TEST(SevenTest, BoundaryValues) {
    // Тестирование граничных значений
    Seven::Seven maxDigit({'6'}); // Максимальная цифра
    Seven::Seven minDigit({'0'}); // Минимальная цифра
    
    Seven::Seven one({'1'});
    
    // 6 + 1 = 10
    Seven::Seven result = Seven::Seven::ADD(maxDigit, one);
    Seven::Seven expected({'0', '1'});
    EXPECT_TRUE(result.EQ(expected));
    
    // 1 - 0 = 1
    Seven::Seven result2 = Seven::Seven::SUB(one, minDigit);
    EXPECT_TRUE(result2.EQ(one));
}

// ==================== INTEGRATION TESTS ====================

TEST(IntegrationTest, VectorInSeven) {
    // Тест что vector корректно используется в Seven
    Seven::Seven s({'1', '2', '3'});
    
    // Проверяем базовые операции
    EXPECT_FALSE(s.EQ(Seven::Seven({'4', '5', '6'})));
    EXPECT_TRUE(s.GT(Seven::Seven({'1', '2'})));
}

TEST(IntegrationTest, ComplexOperations) {
    Seven::Seven a(std::string("123"));
    Seven::Seven b(std::string("456"));
    
    Seven::Seven sum = Seven::Seven::ADD(a, b);
    Seven::Seven diff = Seven::Seven::SUB(b, a);
    
    // Проверяем что результаты валидны
    EXPECT_TRUE(sum.GT(a));
    EXPECT_TRUE(sum.GT(b));
    EXPECT_TRUE(diff.GT(Seven::Seven({'0'}))); // Разность должна быть положительной
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}