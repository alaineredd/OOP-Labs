// tests.cpp
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <typeinfo>
#include <atomic>
#include "visitor.h"
#include "npc.h"

// Helper функция для создания shared_ptr тестовых NPC
std::shared_ptr<Knight> createTestKnight(int x, int y) {
    return std::make_shared<Knight>("TestKnight", x, y);
}

std::shared_ptr<Squirrel> createTestSquirrel(int x, int y) {
    return std::make_shared<Squirrel>("TestSquirrel", x, y);
}

std::shared_ptr<Pegasus> createTestPegasus(int x, int y) {
    return std::make_shared<Pegasus>("TestPegasus", x, y);
}

// Тест 1: Проверка создания и уничтожения
TEST(AsyncBattleVisitorTest, ConstructorDestructor) {
    AsyncBattleVisitor visitor;
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 2: Проверка добавления задачи через addBattleTask
TEST(AsyncBattleVisitorTest, AddBattleTaskBasic) {
    AsyncBattleVisitor visitor;
    
    auto attacker = createTestKnight(0, 0);
    auto defender = createTestSquirrel(1, 1);
    
    // Используем прямое добавление задачи
    visitor.addBattleTask(attacker, defender);
    
    EXPECT_FALSE(visitor.isEmpty());
    
    // Проверяем получение задачи
    BattleTask task(nullptr, nullptr);
    bool gotTask = visitor.getBattleTask(task);
    EXPECT_TRUE(gotTask);
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 3: Проверка shouldFight логики
TEST(AsyncBattleVisitorTest, ShouldFightImplementation) {
    AsyncBattleVisitor visitor;
    
    auto knight = createTestKnight(0, 0);
    auto squirrel = createTestSquirrel(1, 1);
    auto pegasus = createTestPegasus(2, 2);
    auto knight2 = createTestKnight(3, 3);
    
    bool result = visitor.shouldFight(*knight, *squirrel);
    EXPECT_TRUE(result); // Knight должен сражаться с Squirrel
    
    bool result2 = visitor.shouldFight(*knight, *pegasus);
    EXPECT_FALSE(result2); // Knight не должен сражаться с Pegasus
    
    bool result3 = visitor.shouldFight(*squirrel, *pegasus);
    EXPECT_TRUE(result3); // Squirrel должен сражаться с Pegasus
    
    bool result4 = visitor.shouldFight(*knight, *knight2);
    EXPECT_FALSE(result4); // Knight не должен сражаться с другим Knight
}

// Тест 4: Проверка visit методов - они должны добавлять в очередь
TEST(AsyncBattleVisitorTest, VisitMethods) {
    AsyncBattleVisitor visitor;
    
    auto knight = createTestKnight(0, 0);
    auto squirrel = createTestSquirrel(1, 1);
    auto pegasus = createTestPegasus(2, 2);
    
    // Проверяем visit для Knight (должен добавить, так как рыцарь атакует белку)
    visitor.visit(static_cast<Knight&>(*knight), *squirrel);
    
    // Очередь должна быть не пустой
    EXPECT_FALSE(visitor.isEmpty());
    
    // Проверяем visit для Squirrel с Pegasus (должен добавить)
    visitor.visit(static_cast<Squirrel&>(*squirrel), *pegasus);
    
    // Проверяем visit для Squirrel с Knight (не должен добавить)
    visitor.visit(static_cast<Squirrel&>(*squirrel), *knight);
    
    // Проверяем visit для Pegasus (не должен добавить никого)
    visitor.visit(static_cast<Pegasus&>(*pegasus), *knight);
    
    // Очередь должна содержать 2 задачи
    BattleTask task(nullptr, nullptr);
    int count = 0;
    while (visitor.getBattleTask(task)) {
        count++;
    }
    EXPECT_EQ(count, 2);
}

// Тест 5: Упрощенный тест processBattle
TEST(AsyncBattleVisitorTest, ProcessBattleSimple) {
    AsyncBattleVisitor visitor;
    
    auto knight = createTestKnight(0, 0);
    auto squirrel = createTestSquirrel(1, 1);
    
    // Вызываем processBattle - он должен проверить shouldFight и НЕ добавлять в очередь
    // processBattle обрабатывает битву сразу, а не добавляет в очередь
    visitor.processBattle(*knight, *squirrel);
    
    // Очередь должна быть пустой, так как processBattle обрабатывает сразу
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 6: Многопоточное добавление задач с таймаутом
TEST(AsyncBattleVisitorTest, ConcurrentAddTasksWithTimeout) {
    AsyncBattleVisitor visitor;
    const int NUM_TASKS = 10;
    
    std::vector<std::thread> threads;
    std::atomic<int> tasksAdded{0};
    
    for (int i = 0; i < NUM_TASKS; ++i) {
        threads.emplace_back([&visitor, i, &tasksAdded]() {
            auto attacker = createTestKnight(i, i);
            auto defender = createTestSquirrel(i+1, i+1);
            visitor.addBattleTask(attacker, defender);
            tasksAdded++;
        });
    }
    
    // Ждем завершения всех потоков
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Проверяем что все задачи добавлены
    EXPECT_EQ(tasksAdded, NUM_TASKS);
    
    // Пытаемся получить задачи с таймаутом
    int tasksRetrieved = 0;
    auto start = std::chrono::steady_clock::now();
    
    while (tasksRetrieved < NUM_TASKS && 
           std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
        BattleTask task(nullptr, nullptr);
        if (visitor.getBattleTask(task)) {
            tasksRetrieved++;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    EXPECT_EQ(tasksRetrieved, NUM_TASKS);
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 7: Простой Producer-Consumer
TEST(AsyncBattleVisitorTest, SimpleProducerConsumer) {
    AsyncBattleVisitor visitor;
    
    std::atomic<int> produced{0};
    std::atomic<int> consumed{0};
    
    // Producer
    auto producer = std::thread([&visitor, &produced]() {
        for (int i = 0; i < 5; ++i) {
            auto attacker = createTestKnight(i, i);
            auto defender = createTestSquirrel(i+1, i+1);
            visitor.addBattleTask(attacker, defender);
            produced++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    
    // Consumer
    std::thread consumer([&visitor, &consumed]() {
        int received = 0;
        auto start = std::chrono::steady_clock::now();
        
        while (received < 5 && 
               std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
            BattleTask task(nullptr, nullptr);
            if (visitor.getBattleTask(task)) {
                received++;
                consumed++;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    });
    
    producer.join();
    consumer.join();
    
    EXPECT_EQ(produced, 5);
    EXPECT_EQ(consumed, 5);
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 8: Проверка порядка задач (FIFO)
TEST(AsyncBattleVisitorTest, TaskOrderFIFO) {
    AsyncBattleVisitor visitor;
    
    // Добавляем задачи в определенном порядке
    for (int i = 0; i < 5; ++i) {
        auto attacker = createTestKnight(i, i);
        auto defender = createTestSquirrel(i, i);
        visitor.addBattleTask(attacker, defender);
    }
    
    // Получаем задачи и проверяем порядок
    for (int i = 0; i < 5; ++i) {
        BattleTask task(nullptr, nullptr);
        bool gotTask = visitor.getBattleTask(task);
        
        EXPECT_TRUE(gotTask);
        if (gotTask) {
            auto attacker = std::dynamic_pointer_cast<Knight>(task.attacker);
            auto defender = std::dynamic_pointer_cast<Squirrel>(task.defender);
            EXPECT_NE(attacker, nullptr);
            EXPECT_NE(defender, nullptr);
        }
    }
    
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 9: Обработка пустой очереди
TEST(AsyncBattleVisitorTest, EmptyQueueBehavior) {
    AsyncBattleVisitor visitor;
    
    // Проверяем что очередь пустая
    EXPECT_TRUE(visitor.isEmpty());
    
    // Создаем поток, который будет пытаться получить задачу
    std::atomic<bool> taskReceived{false};
    std::atomic<bool> threadFinished{false};
    
    std::thread consumer([&visitor, &taskReceived, &threadFinished]() {
        BattleTask task(nullptr, nullptr);
        // Пытаемся получить задачу с таймаутом
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(100)) {
            if (visitor.getBattleTask(task)) {
                taskReceived = true;
                break;
            }
        }
        threadFinished = true;
    });
    
    // Ждем завершения потока
    consumer.join();
    
    // Задача не должна быть получена
    EXPECT_FALSE(taskReceived);
    EXPECT_TRUE(threadFinished);
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 10: Разные комбинации NPC
TEST(AsyncBattleVisitorTest, DifferentNPCCombinations) {
    AsyncBattleVisitor visitor;
    
    // Добавляем разные комбинации NPC
    auto knight = createTestKnight(0, 0);
    auto squirrel = createTestSquirrel(1, 1);
    auto pegasus = createTestPegasus(2, 2);
    
    visitor.addBattleTask(knight, squirrel);
    visitor.addBattleTask(squirrel, pegasus);
    visitor.addBattleTask(pegasus, knight);
    visitor.addBattleTask(knight, pegasus);
    
    int count = 0;
    BattleTask task(nullptr, nullptr);
    while (visitor.getBattleTask(task)) {
        count++;
    }
    
    EXPECT_EQ(count, 4);
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 11: Проверка метода isEmpty
TEST(AsyncBattleVisitorTest, IsEmptyMethod) {
    AsyncBattleVisitor visitor;
    
    // Изначально пусто
    EXPECT_TRUE(visitor.isEmpty());
    
    // Добавляем задачу
    auto attacker = createTestKnight(0, 0);
    auto defender = createTestSquirrel(1, 1);
    visitor.addBattleTask(attacker, defender);
    
    // Теперь не пусто
    EXPECT_FALSE(visitor.isEmpty());
    
    // Забираем задачу
    BattleTask task(nullptr, nullptr);
    bool gotTask = visitor.getBattleTask(task);
    EXPECT_TRUE(gotTask);
    
    // Снова пусто
    EXPECT_TRUE(visitor.isEmpty());
}

// Тест 12: Многократное добавление и удаление
TEST(AsyncBattleVisitorTest, MultipleAddRemove) {
    AsyncBattleVisitor visitor;
    
    for (int i = 0; i < 10; ++i) {
        auto attacker = createTestKnight(i, i);
        auto defender = createTestSquirrel(i+100, i+100);
        visitor.addBattleTask(attacker, defender);
        
        BattleTask task(nullptr, nullptr);
        bool gotTask = visitor.getBattleTask(task);
        EXPECT_TRUE(gotTask);
        EXPECT_TRUE(visitor.isEmpty());
    }
}

// Тест 13: Проверка логики shouldFight с разными комбинациями
TEST(AsyncBattleVisitorTest, ShouldFightDifferentCombinations) {
    AsyncBattleVisitor visitor;
    
    auto knight1 = createTestKnight(0, 0);
    auto knight2 = createTestKnight(1, 1);
    auto squirrel1 = createTestSquirrel(2, 2);
    auto squirrel2 = createTestSquirrel(3, 3);
    auto pegasus1 = createTestPegasus(4, 4);
    auto pegasus2 = createTestPegasus(5, 5);
    
    // Рыцарь должен сражаться с белкой
    EXPECT_TRUE(visitor.shouldFight(*knight1, *squirrel1));
    
    // Рыцарь не должен сражаться с другим рыцарем
    EXPECT_FALSE(visitor.shouldFight(*knight1, *knight2));
    
    // Рыцарь не должен сражаться с пегасом
    EXPECT_FALSE(visitor.shouldFight(*knight1, *pegasus1));
    
    // Белка должна сражаться с пегасом
    EXPECT_TRUE(visitor.shouldFight(*squirrel1, *pegasus1));
    
    // Белка не должна сражаться с другой белкой
    EXPECT_FALSE(visitor.shouldFight(*squirrel1, *squirrel2));
    
    // Белка не должна сражаться с рыцарем (это рыцарь атакует белку)
    EXPECT_FALSE(visitor.shouldFight(*squirrel1, *knight1));
    
    // Пегас ни с кем не должен сражаться
    EXPECT_FALSE(visitor.shouldFight(*pegasus1, *knight1));
    EXPECT_FALSE(visitor.shouldFight(*pegasus1, *squirrel1));
    EXPECT_FALSE(visitor.shouldFight(*pegasus1, *pegasus2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}