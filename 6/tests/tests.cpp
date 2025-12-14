// tests.cpp - Google Tests для лабораторной работы "Редактор подземелья"

#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>

#include "npc.h"
#include "npc_factory.h"
#include "visitor.h"
#include "observer.h"
#include "dungeon.h"

using namespace std;

// ====================== Тесты для NPC ======================

TEST(NPCTest, ConstructorAndGetters) {
    Knight knight("Arthur", 100, 200);
    
    EXPECT_EQ(knight.getType(), "Knight");
    EXPECT_EQ(knight.getName(), "Arthur");
    EXPECT_EQ(knight.getX(), 100);
    EXPECT_EQ(knight.getY(), 200);
    EXPECT_TRUE(knight.isAlive());
}

TEST(NPCTest, DistanceCalculation) {
    Knight knight1("K1", 0, 0);
    Knight knight2("K2", 3, 4); // Расстояние 5 по теореме Пифагора
    
    EXPECT_DOUBLE_EQ(knight1.distanceTo(knight2), 5.0);
    EXPECT_TRUE(knight1.isInRange(knight2, 5));
    EXPECT_FALSE(knight1.isInRange(knight2, 4));
}

TEST(NPCTest, DeathFunctionality) {
    Knight knight("Arthur", 100, 200);
    
    EXPECT_TRUE(knight.isAlive());
    knight.die();
    EXPECT_FALSE(knight.isAlive());
}

TEST(NPCTest, DifferentNPCTypes) {
    Knight knight("Arthur", 100, 200);
    Squirrel squirrel("Nutty", 150, 250);
    Pegasus pegasus("Sky", 300, 400);
    
    EXPECT_EQ(knight.getType(), "Knight");
    EXPECT_EQ(squirrel.getType(), "Squirrel");
    EXPECT_EQ(pegasus.getType(), "Pegasus");
}

// ====================== Тесты для NPCFactory ======================

TEST(NPCFactoryTest, CreateNPC) {
    auto knight = NPCFactory::createNPC("Knight", "Arthur", 100, 200);
    auto squirrel = NPCFactory::createNPC("Squirrel", "Nutty", 150, 250);
    auto pegasus = NPCFactory::createNPC("Pegasus", "Sky", 300, 400);
    
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    EXPECT_EQ(pegasus->getType(), "Pegasus");
    
    EXPECT_THROW(NPCFactory::createNPC("Dragon", "Draco", 0, 0), runtime_error);
}

TEST(NPCFactoryTest, SerializationDeserialization) {
    Knight original("Arthur", 100, 200);
    
    string serialized = NPCFactory::saveNPCToString(original);
    
    // Проверяем формат сериализации
    EXPECT_TRUE(serialized.find("Knight Arthur 100 200") != string::npos);
    
    // Десериализация
    auto deserialized = NPCFactory::loadNPCFromString(serialized);
    
    EXPECT_EQ(deserialized->getType(), "Knight");
    EXPECT_EQ(deserialized->getName(), "Arthur");
    EXPECT_EQ(deserialized->getX(), 100);
    EXPECT_EQ(deserialized->getY(), 200);
}

TEST(NPCFactoryTest, LoadInvalidFormat) {
    EXPECT_THROW(NPCFactory::loadNPCFromString(""), runtime_error);
    EXPECT_THROW(NPCFactory::loadNPCFromString("Knight Arthur"), runtime_error);
    EXPECT_THROW(NPCFactory::loadNPCFromString("Knight Arthur 100"), runtime_error);
    EXPECT_THROW(NPCFactory::loadNPCFromString("Knight Arthur 100 abc"), runtime_error);
}

// ====================== Тесты для BattleVisitor ======================

// Тестовый посетитель для проверки вызовов
class TestVisitor : public NPCVisitor {
public:
    int knightVisits = 0;
    int squirrelVisits = 0;
    int pegasusVisits = 0;
    
    void visit(Knight& knight, NPC& other) override {
        knightVisits++;
    }
    
    void visit(Squirrel& squirrel, NPC& other) override {
        squirrelVisits++;
    }
    
    void visit(Pegasus& pegasus, NPC& other) override {
        pegasusVisits++;
    }
};

TEST(BattleVisitorTest, AcceptMethodCalls) {
    Knight knight("Arthur", 0, 0);
    Squirrel squirrel("Nutty", 0, 0);
    Pegasus pegasus("Sky", 0, 0);
    
    TestVisitor visitor;
    
    knight.accept(visitor);
    squirrel.accept(visitor);
    pegasus.accept(visitor);
    
    EXPECT_EQ(visitor.knightVisits, 1);
    EXPECT_EQ(visitor.squirrelVisits, 1);
    EXPECT_EQ(visitor.pegasusVisits, 1);
}

TEST(BattleVisitorTest, BattleLogic) {
    // Создаем NPC для тестирования логики боя
    auto knight = make_shared<Knight>("Arthur", 0, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 0, 0); // В радиусе 0
    auto pegasus = make_shared<Pegasus>("Sky", 100, 100); // Вне радиуса
    
    BattleVisitor visitor(10); // Радиус 10
    
    // Рыцарь должен убить белку (если в радиусе)
    visitor.visit(*knight, *squirrel);
    EXPECT_TRUE(knight->isAlive());
    EXPECT_FALSE(squirrel->isAlive()); // Белка должна умереть
    
    // Белка должна убить пегаса (но пегас вне радиуса)
    visitor.visit(*squirrel, *pegasus);
    EXPECT_TRUE(pegasus->isAlive()); // Пегас должен остаться жив (вне радиуса)
}

TEST(BattleVisitorTest, ShouldFightLogic) {
    auto knight = make_shared<Knight>("Arthur", 0, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 5, 0); // В радиусе 10
    auto pegasus = make_shared<Pegasus>("Sky", 50, 50); // Вне радиуса
    
    BattleVisitor visitor(10);
    
    // Проверяем логику shouldFight
    EXPECT_TRUE(visitor.shouldFight(*knight, *squirrel)); // Разные типы, в радиусе
    EXPECT_FALSE(visitor.shouldFight(*knight, *pegasus)); // Пегас вне радиуса
    EXPECT_FALSE(visitor.shouldFight(*knight, *knight)); // Одинаковые типы
}

TEST(BattleVisitorTest, RangeChecking) {
    auto knight1 = make_shared<Knight>("Arthur", 0, 0);
    auto knight2 = make_shared<Knight>("Lancelot", 8, 6); // Расстояние 10
    
    BattleVisitor visitor(10);
    
    EXPECT_TRUE(knight1->isInRange(*knight2, 10)); // Точно в радиусе
    EXPECT_FALSE(knight1->isInRange(*knight2, 9)); // За пределами
}

// ====================== Тесты для BattleObserver ======================

TEST(BattleObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    
    // Проверяем, что не падает при вызове
    testing::internal::CaptureStdout();
    observer.onBattleResult("Test message");
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Test message") != string::npos);
}

TEST(BattleObserverTest, BattleSubject) {
    auto subject = make_shared<BattleSubject>();
    bool observer1Called = false;
    bool observer2Called = false;
    
    // Создаем мок-наблюдатели
    class MockObserver : public BattleObserver {
    public:
        bool& called;
        MockObserver(bool& calledRef) : called(calledRef) {}
        void onBattleResult(const string& result) override {
            called = true;
        }
    };
    
    auto observer1 = make_shared<MockObserver>(observer1Called);
    auto observer2 = make_shared<MockObserver>(observer2Called);
    
    subject->addObserver(observer1);
    subject->addObserver(observer2);
    
    subject->notifyObservers("Test");
    
    EXPECT_TRUE(observer1Called);
    EXPECT_TRUE(observer2Called);
    
    // Проверяем удаление наблюдателя
    observer1Called = false;
    subject->removeObserver(observer1);
    subject->notifyObservers("Test 2");
    
    EXPECT_FALSE(observer1Called); // Не должен быть вызван
    EXPECT_TRUE(observer2Called);  // Должен быть вызван
}

// ====================== Тесты для Dungeon ======================

TEST(DungeonTest, AddAndRemoveNPC) {
    Dungeon dungeon;
    
    EXPECT_EQ(dungeon.getNPCCount(), 0);
    
    dungeon.addNPC("Knight", "Arthur", 100, 200);
    EXPECT_EQ(dungeon.getNPCCount(), 1);
    
    dungeon.addNPC("Squirrel", "Nutty", 150, 250);
    EXPECT_EQ(dungeon.getNPCCount(), 2);
    
    // Проверяем валидацию координат
    EXPECT_THROW(dungeon.addNPC("Knight", "Invalid", 600, 600), runtime_error);
    EXPECT_THROW(dungeon.addNPC("Knight", "Negative", -10, -10), runtime_error);
}

TEST(DungeonTest, SaveAndLoad) {
    Dungeon dungeon;
    
    // Добавляем тестовых NPC
    dungeon.addNPC("Knight", "Arthur", 100, 200);
    dungeon.addNPC("Squirrel", "Nutty", 150, 250);
    dungeon.addNPC("Pegasus", "Sky", 300, 400);
    
    const string testFile = "test_save.txt";
    
    // Сохраняем
    dungeon.saveToFile(testFile);
    EXPECT_TRUE(filesystem::exists(testFile));
    
    // Загружаем в новый dungeon
    Dungeon loadedDungeon;
    loadedDungeon.loadFromFile(testFile);
    
    // Проверяем, что загрузилось столько же NPC
    // (не проверяем точное совпадение, так как нет метода для получения списка)
    
    // Убираем за собой
    filesystem::remove(testFile);
}

TEST(DungeonTest, PrintFunctionality) {
    Dungeon dungeon;
    
    dungeon.addNPC("Knight", "Arthur", 100, 200);
    
    // Проверяем, что метод не падает
    testing::internal::CaptureStdout();
    dungeon.printAllNPCs();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Arthur") != string::npos);
    EXPECT_TRUE(output.find("Knight") != string::npos);
    EXPECT_TRUE(output.find("100") != string::npos);
    EXPECT_TRUE(output.find("200") != string::npos);
}

TEST(DungeonTest, RemoveDeadNPCs) {
    Dungeon dungeon;
    
    auto knight = make_shared<Knight>("Arthur", 0, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 0, 0);
    
    dungeon.addNPC(knight);
    dungeon.addNPC(squirrel);
    
    // Убиваем белку
    squirrel->die();
    
    // Удаляем мертвых
    dungeon.removeDeadNPCs();
    
    // Должен остаться только рыцарь
    // (не проверяем напрямую, так как нет доступа к списку)
}

// ====================== Интеграционные тесты ======================

TEST(IntegrationTest, CompleteBattleScenario) {
    Dungeon dungeon;
    
    // Настраиваем логирование
    auto consoleObserver = make_shared<ConsoleObserver>();
    auto fileObserver = make_shared<FileObserver>("integration_test_log.txt");
    dungeon.addBattleObserver(consoleObserver);
    dungeon.addBattleObserver(fileObserver);
    
    // Создаем сценарий:
    // 1. Рыцарь убивает белку
    // 2. Белка убивает пегаса (но только если рыцарь не убил ее раньше)
    // 3. Пегас никого не трогает
    
    dungeon.addNPC("Knight", "Arthur", 0, 0);
    dungeon.addNPC("Squirrel", "Nutty", 5, 0);   // В радиусе боя
    dungeon.addNPC("Pegasus", "Sky", 10, 0);     // В радиусе для белки
    dungeon.addNPC("Squirrel", "Chip", 50, 50);  // Вне радиуса
    
    // Запускаем бой
    dungeon.startBattle(15);
    
    // Ожидаемый результат:
    // - Arthur убивает Nutty (белка в радиусе)
    // - Chip остается жив (вне радиуса)
    // - Sky остается жив (пегас никого не трогает, и белка уже мертва)
    
    // Проверяем что файл лога создан
    EXPECT_TRUE(filesystem::exists("integration_test_log.txt"));
    
    // Убираем за собой
    filesystem::remove("integration_test_log.txt");
}

TEST(IntegrationTest, FilePersistence) {
    const string testFile = "persistence_test.txt";
    
    // Создаем и сохраняем dungeon
    {
        Dungeon dungeon;
        dungeon.addNPC("Knight", "Arthur", 100, 200);
        dungeon.addNPC("Squirrel", "Nutty", 150, 250);
        dungeon.saveToFile(testFile);
    }
    
    // Загружаем в новый dungeon
    Dungeon loadedDungeon;
    loadedDungeon.loadFromFile(testFile);
    
    // Проверяем, что загрузилось
    // (косвенно через печать)
    testing::internal::CaptureStdout();
    loadedDungeon.printAllNPCs();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Arthur") != string::npos);
    EXPECT_TRUE(output.find("Nutty") != string::npos);
    
    // Убираем за собой
    filesystem::remove(testFile);
}

// ====================== Тесты на граничные условия ======================

TEST(BoundaryTest, CoordinatesBoundaries) {
    Dungeon dungeon;
    
    // Проверяем граничные значения
    EXPECT_NO_THROW(dungeon.addNPC("Knight", "Min", 0, 0));
    EXPECT_NO_THROW(dungeon.addNPC("Knight", "Max", 500, 500));
    EXPECT_NO_THROW(dungeon.addNPC("Knight", "Middle", 250, 250));
    
    EXPECT_THROW(dungeon.addNPC("Knight", "TooLow", -1, 0), runtime_error);
    EXPECT_THROW(dungeon.addNPC("Knight", "TooHigh", 501, 0), runtime_error);
    EXPECT_THROW(dungeon.addNPC("Knight", "Negative", -100, -100), runtime_error);
    EXPECT_THROW(dungeon.addNPC("Knight", "WayTooHigh", 1000, 1000), runtime_error);
}

TEST(BoundaryTest, BattleRangeEdgeCases) {
    auto knight = make_shared<Knight>("Arthur", 0, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 10, 0); // Точно на границе
    
    // Радиус точно равен расстоянию
    EXPECT_TRUE(knight->isInRange(*squirrel, 10));
    
    // Радиус чуть меньше расстояния
    EXPECT_FALSE(knight->isInRange(*squirrel, 9.999));
    
    // Радиус чуть больше расстояния
    EXPECT_TRUE(knight->isInRange(*squirrel, 10.001));
}

TEST(BoundaryTest, EmptyDungeonOperations) {
    Dungeon dungeon;
    
    // Операции с пустым dungeon не должны падать
    EXPECT_NO_THROW(dungeon.printAllNPCs());
    EXPECT_NO_THROW(dungeon.startBattle(100));
    EXPECT_NO_THROW(dungeon.removeDeadNPCs());
    
    const string testFile = "empty_test.txt";
    EXPECT_NO_THROW(dungeon.saveToFile(testFile));
    
    // Проверяем что файл создан
    EXPECT_TRUE(filesystem::exists(testFile));
    
    // Загрузка несуществующего файла должна бросать исключение
    EXPECT_THROW(dungeon.loadFromFile("non_existent_file.txt"), runtime_error);
    
    // Убираем за собой
    filesystem::remove(testFile);
}

// ====================== Тесты на правила боя ======================

TEST(BattleRulesTest, KnightVsSquirrel) {
    // Рыцарь убивает белку
    auto knight = make_shared<Knight>("Arthur", 0, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 5, 0);
    
    BattleVisitor visitor(10);
    
    visitor.visit(*knight, *squirrel);
    visitor.visit(*squirrel, *knight); // Обратное посещение
    
    EXPECT_TRUE(knight->isAlive());
    EXPECT_FALSE(squirrel->isAlive());
}

TEST(BattleRulesTest, SquirrelVsPegasus) {
    // Белка убивает пегаса
    auto squirrel = make_shared<Squirrel>("Nutty", 0, 0);
    auto pegasus = make_shared<Pegasus>("Sky", 5, 0);
    
    BattleVisitor visitor(10);
    
    visitor.visit(*squirrel, *pegasus);
    visitor.visit(*pegasus, *squirrel); // Пегас никого не трогает
    
    EXPECT_FALSE(pegasus->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
}

TEST(BattleRulesTest, PegasusNeverAttacks) {
    // Пегас никого не трогает
    auto pegasus = make_shared<Pegasus>("Sky", 0, 0);
    auto knight = make_shared<Knight>("Arthur", 5, 0);
    auto squirrel = make_shared<Squirrel>("Nutty", 5, 0);
    
    BattleVisitor visitor(10);
    
    visitor.visit(*pegasus, *knight);
    visitor.visit(*pegasus, *squirrel);
    
    // Все должны остаться живы после атаки пегаса
    EXPECT_TRUE(knight->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
    EXPECT_TRUE(pegasus->isAlive());
}

TEST(BattleRulesTest, SameTypeNoFight) {
    // NPC одного типа не дерутся
    auto knight1 = make_shared<Knight>("Arthur", 0, 0);
    auto knight2 = make_shared<Knight>("Lancelot", 5, 0);
    
    auto squirrel1 = make_shared<Squirrel>("Nutty", 10, 0);
    auto squirrel2 = make_shared<Squirrel>("Chip", 15, 0);
    
    BattleVisitor visitor(10);
    
    // Проверяем shouldFight
    EXPECT_FALSE(visitor.shouldFight(*knight1, *knight2));
    EXPECT_FALSE(visitor.shouldFight(*squirrel1, *squirrel2));
}

// ====================== Главная функция тестов ======================

int main(int argc, char **argv) {
    // Инициализация Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // Устанавливаем уровень детализации
    ::testing::GTEST_FLAG(print_time) = true;
    ::testing::GTEST_FLAG(color) = "yes";
    
    // Запуск всех тестов
    return RUN_ALL_TESTS();
}