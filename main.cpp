#include <iostream>
#include <thread>
#include <vector>

int shared_counter = 0; // Общая переменная без мьютекса!

void increment() {
    for (int i = 0; i < 100000; ++i) {
        shared_counter++;
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Final counter: " << shared_counter << std::endl;
    return 0;
}