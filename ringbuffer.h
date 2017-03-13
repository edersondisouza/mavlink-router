#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

template<typename T>
class Ringbuffer {
public:
    Ringbuffer(size_t num_elem);
    ~Ringbuffer();

    enum add_result {ok, full, overflow, dropped};

    enum add_result add(T *item, uint32_t seqno);

    inline void reset() {
        _head = 0;
        _tail = 0;
        _tail_offset = 0;
        _last_seqno = 0;
        _full = false;
    };

    inline int write_full(int fd) {
        return _write(fd, _head);
    };

    inline int write_partial(int fd) {
        return _write(fd, _dec_by(_head, _num_elem / 2));
    };

private:
    uint8_t *_buffer;
    size_t _num_elem;
    uint32_t _head = 0;
    uint32_t _tail = 0;
    uint32_t _tail_offset = 0;
    uint32_t _last_seqno = 0;
    bool _full = false;

    inline uint32_t _inc(uint32_t i) {
        return ++i % _num_elem;
    };

    inline uint32_t _dec(uint32_t i) {
        if (i == 0) {
            return _num_elem - 1;
        }

        return --i;
    };

    // dec must be <= _num_elem
    inline uint32_t _dec_by(uint32_t i, uint32_t dec) {
        int32_t sub = i - dec;
        if (sub < 0) {
            return (uint32_t)((int32_t)_num_elem) + sub;
        }

        return sub;
    };

    inline bool _in_between(int pos, int tail, int head) {
        return (pos >= tail && pos < head) ||
               (pos < head && tail > head) ||
               (pos >= tail && head < tail);
    };

    void _inc_tail_by_bytes(size_t num_bytes);
    enum add_result _add_spaces(size_t n);
    ssize_t _write(int fd, uint32_t to);

    // Disable copy constructor and assignment
    Ringbuffer(const Ringbuffer&) = delete;
    Ringbuffer& operator=(const Ringbuffer&) = delete;
};
