#include <iostream>
#include "memory_resource.h"
#include "queue.h"

int main() {
    FixedBlockMemoryResource resource(1024);
    Queue<int> queue(&resource);

    queue.push(10);
    queue.push(20);
    queue.push(30);

    std::cout << "Очередь: ";
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Первый элемент: " << queue.front() << std::endl;
    queue.pop();
    std::cout << "После pop: " << queue.front() << std::endl;

    return 0;
}