#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::deque<int> buffer;
unsigned const int maxSize{100};

void producer(int val)
{
    while(val) {
        std::unique_lock<std::mutex> locker(m);
        cv.wait(locker, [] () { return buffer.size() < maxSize; });
        buffer.push_back(val);
        std::cout << "Value Produced: " << val <<"\n";
        val--;
        locker.unlock();
        cv.notify_one();
    }
}

void consumer()
{
    while(true) {
        std::unique_lock<std::mutex> locker(m);
        cv.wait(locker, [] () { return buffer.size() > 0; });
        std::cout << "Value Consumed: " << buffer.back() <<"\n";
        buffer.pop_back();
        locker.unlock();
        cv.notify_one();
    }
}

int main()
{
    std::jthread pro{producer, 100};
    std::jthread cons{consumer};
    return 0;
}
