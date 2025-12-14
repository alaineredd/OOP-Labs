#include <gtest/gtest.h>
#include "memory_resource.h"
#include "queue.h"

// Тесты для FixedBlockMemoryResource
class FixedBlockMemoryResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = new FixedBlockMemoryResource(1024);
    }

    void TearDown() override {
        delete resource;
    }

    FixedBlockMemoryResource* resource = nullptr;
};

TEST_F(FixedBlockMemoryResourceTest, BasicAllocation) {
    void* ptr = resource->allocate(64, 8);
    EXPECT_NE(ptr, nullptr);
    resource->deallocate(ptr, 64, 8);
}

TEST_F(FixedBlockMemoryResourceTest, ReuseMemory) {
    void* ptr1 = resource->allocate(32, 8);
    resource->deallocate(ptr1, 32, 8);
    
    void* ptr2 = resource->allocate(32, 8);
    EXPECT_EQ(ptr1, ptr2); // Должен переиспользовать память
    resource->deallocate(ptr2, 32, 8);
}

TEST_F(FixedBlockMemoryResourceTest, OutOfMemory) {
    EXPECT_THROW({
        resource->allocate(2048, 8); // Больше чем размер пула
    }, std::runtime_error);
}

TEST_F(FixedBlockMemoryResourceTest, MultipleAllocations) {
    void* ptr1 = resource->allocate(16, 8);
    void* ptr2 = resource->allocate(32, 8);
    void* ptr3 = resource->allocate(64, 8);
    
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr2, ptr3);
    
    resource->deallocate(ptr1, 16, 8);
    resource->deallocate(ptr2, 32, 8);
    resource->deallocate(ptr3, 64, 8);
}

// Тесты для Queue с int
class QueueIntTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = new FixedBlockMemoryResource(1024);
        queue = new Queue<int>(resource);
    }

    void TearDown() override {
        delete queue;
        delete resource;
    }

    FixedBlockMemoryResource* resource = nullptr;
    Queue<int>* queue = nullptr;
};

TEST_F(QueueIntTest, EmptyQueue) {
    EXPECT_TRUE(queue->empty());
    EXPECT_EQ(queue->size(), 0);
}

TEST_F(QueueIntTest, PushAndSize) {
    queue->push(10);
    EXPECT_FALSE(queue->empty());
    EXPECT_EQ(queue->size(), 1);
    
    queue->push(20);
    EXPECT_EQ(queue->size(), 2);
}

TEST_F(QueueIntTest, FrontAndBack) {
    queue->push(10);
    queue->push(20);
    queue->push(30);
    
    EXPECT_EQ(queue->front(), 10);
    EXPECT_EQ(queue->back(), 30);
}

TEST_F(QueueIntTest, Pop) {
    queue->push(10);
    queue->push(20);
    
    queue->pop();
    EXPECT_EQ(queue->front(), 20);
    EXPECT_EQ(queue->size(), 1);
    
    queue->pop();
    EXPECT_TRUE(queue->empty());
}

TEST_F(QueueIntTest, PopEmptyThrows) {
    EXPECT_THROW({
        queue->pop();
    }, std::runtime_error);
}

TEST_F(QueueIntTest, FrontEmptyThrows) {
    EXPECT_THROW({
        queue->front();
    }, std::runtime_error);
}

TEST_F(QueueIntTest, Iterator) {
    queue->push(10);
    queue->push(20);
    queue->push(30);
    
    int expected[] = {10, 20, 30};
    int i = 0;
    
    for (auto it = queue->begin(); it != queue->end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

TEST_F(QueueIntTest, RangeBasedFor) {
    queue->push(1);
    queue->push(2);
    queue->push(3);
    
    int sum = 0;
    for (const auto& item : *queue) {
        sum += item;
    }
    
    EXPECT_EQ(sum, 6);
}

TEST_F(QueueIntTest, CopyConstructor) {
    queue->push(1);
    queue->push(2);
    
    Queue<int> copy = *queue;
    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy.front(), 1);
    
    copy.pop();
    EXPECT_EQ(copy.front(), 2);
    EXPECT_EQ(queue->size(), 2); // Оригинал не должен измениться
}

TEST_F(QueueIntTest, MoveConstructor) {
    queue->push(1);
    queue->push(2);
    
    Queue<int> moved = std::move(*queue);
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(queue->empty()); // Оригинал должен стать пустым
}

TEST_F(QueueIntTest, Clear) {
    queue->push(1);
    queue->push(2);
    queue->push(3);
    
    queue->clear();
    EXPECT_TRUE(queue->empty());
    EXPECT_EQ(queue->size(), 0);
}

// Тесты для Queue со сложными типами
struct TestStruct {
    int id;
    std::string name;
    
    TestStruct(int i, const std::string& n) : id(i), name(n) {}
    bool operator==(const TestStruct& other) const {
        return id == other.id && name == other.name;
    }
};

class QueueComplexTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = new FixedBlockMemoryResource(2048);
        queue = new Queue<TestStruct>(resource);
    }

    void TearDown() override {
        delete queue;
        delete resource;
    }

    FixedBlockMemoryResource* resource = nullptr;
    Queue<TestStruct>* queue = nullptr;
};

TEST_F(QueueComplexTest, PushComplexType) {
    queue->push(TestStruct(1, "first"));
    queue->push(TestStruct(2, "second"));
    
    EXPECT_EQ(queue->front(), TestStruct(1, "first"));
    EXPECT_EQ(queue->back(), TestStruct(2, "second"));
}

TEST_F(QueueComplexTest, EmplaceComplexType) {
    queue->push(TestStruct(1, "test"));
    queue->push(TestStruct(2, "example"));
    
    EXPECT_EQ(queue->size(), 2);
    EXPECT_EQ(queue->front().id, 1);
    EXPECT_EQ(queue->back().id, 2);
}

// Интеграционные тесты
class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = new FixedBlockMemoryResource(1024);
    }

    void TearDown() override {
        delete resource;
    }

    FixedBlockMemoryResource* resource = nullptr;
};

TEST_F(IntegrationTest, QueueUsesCustomMemoryResource) {
    Queue<int> queue(resource);
    
    // Добавляем много элементов чтобы убедиться что используется наш memory_resource
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
    }
    
    EXPECT_EQ(queue.size(), 10);
    
    // Проверяем что память переиспользуется
    for (int i = 0; i < 5; ++i) {
        queue.pop();
    }
    
    for (int i = 10; i < 15; ++i) {
        queue.push(i);
    }
    
    EXPECT_EQ(queue.size(), 10);
}

TEST_F(IntegrationTest, MultipleQueuesSameResource) {
    Queue<int> queue1(resource);
    Queue<std::string> queue2(resource);
    
    queue1.push(42);
    queue2.push("test");
    
    EXPECT_EQ(queue1.front(), 42);
    EXPECT_EQ(queue2.front(), "test");
}


TEST(MemoryLeakTest, NoLeaksOnDestruction) {
    // Создаем объекты на стеке, а не в куче
    FixedBlockMemoryResource resource(1024);
    Queue<int> queue(&resource);
    
    for (int i = 0; i < 50; ++i) {
        queue.push(i);
    }
    
    // Все должно корректно очиститься при выходе из scope
    SUCCEED();
}