#ifndef QWE_RING_BUFFER_HH
#define QWE_RING_BUFFER_HH

#include <vector>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class c_ring_buffer {
    std::vector<T> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t max_size{};
    bool full = false;

    std::mutex mutex;
    std::condition_variable cv;

public:
    explicit c_ring_buffer(size_t size) : max_size(size) { buffer.resize(size); }

    void push_back(const T& item) {
        std::unique_lock<std::mutex> lock(mutex);

        buffer[head] = item;
        head = (head + 1) % buffer.size();

        if (full)
            tail = (tail + 1) % max_size;

        full = (head == tail);

        lock.unlock();
        cv.notify_all();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex);

        cv.wait(lock, [this] { return !empty(); });

        auto item = buffer[tail];
        full = false;
        tail = (tail + 1) % max_size;

        return item;
    }

    [[nodiscard]] bool empty() const {
        return (!full && (head == tail));
    }
};

#endif //QWE_RING_BUFFER_HH
