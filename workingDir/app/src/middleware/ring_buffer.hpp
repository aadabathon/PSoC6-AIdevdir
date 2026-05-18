/**
 * @file    ring_buffer.hpp
 * @brief   Single-producer / single-consumer ring buffer.
 *
 * Lock-free for the SPSC case if you're careful about memory ordering across
 * ISR/task boundaries. For multi-producer use, wrap with a mutex at the
 * caller. Statically sized — no allocation.
 *
 * Usage:
 *   RingBuffer<ImuSample, 64> q;
 *   q.push(sample);     // returns false if full
 *   ImuSample s;
 *   if (q.pop(s)) { ... }
 */
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace app {

template <typename T, size_t N>
class RingBuffer {
    static_assert(N > 0, "RingBuffer size must be > 0");
    // Power-of-two N enables a fast modulo. Not strictly required.
    static_assert((N & (N - 1)) == 0, "RingBuffer size must be a power of 2");

public:
    RingBuffer() = default;

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    bool push(const T& item) {
        const size_t head = head_.load(std::memory_order_relaxed);
        const size_t next = (head + 1) & (N - 1);
        if (next == tail_.load(std::memory_order_acquire)) {
            return false;  // full
        }
        buf_[head] = item;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) {
            return false;  // empty
        }
        out = buf_[tail];
        tail_.store((tail + 1) & (N - 1), std::memory_order_release);
        return true;
    }

    size_t size() const {
        const size_t h = head_.load(std::memory_order_acquire);
        const size_t t = tail_.load(std::memory_order_acquire);
        return (h - t) & (N - 1);
    }

    constexpr size_t capacity() const { return N - 1; }

private:
    T                   buf_[N]{};
    std::atomic<size_t> head_{0};
    std::atomic<size_t> tail_{0};
};

}  // namespace app
