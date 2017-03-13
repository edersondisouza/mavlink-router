#include <unistd.h>

#include "ringbuffer.h"

#include "log.h"

template<typename T>
Ringbuffer<T>::Ringbuffer(size_t num_elem)
{
    _buffer = (uint8_t *)calloc(num_elem, sizeof(T));
    _num_elem = num_elem;
}

template<typename T>
Ringbuffer<T>::~Ringbuffer()
{
    free(_buffer);
}

template<typename T>
enum Ringbuffer<T>::add_result Ringbuffer<T>::add(T *item, uint32_t seqno)
{
    uint32_t pos;

    if (_full) {
        return Ringbuffer<T>::overflow;
    }

    if (seqno > _last_seqno + 1) {
        Ringbuffer<T>::add_result result = _add_spaces(seqno - _last_seqno);
        if (result != Ringbuffer<T>::ok) {
            return result;
        }
        _last_seqno = seqno - 1; // So it enters next if first case
    }

    if (seqno == 0 || seqno == _last_seqno + 1) {
        pos = _head;
        _head = _inc(_head);
        _last_seqno = seqno;
    } else {
        uint32_t offset = _last_seqno - seqno;
        pos = _dec_by(_dec(_head), offset);

        // if pos is not in buffer anymore, drop it. We don't consideer tail, as it may
        // have been partially written
        if ((offset > _num_elem) || !_in_between(pos, _inc(_tail), _head)) {
            return Ringbuffer<T>::dropped;
        }
    }

    memcpy(static_cast<uint8_t *>(&_buffer[pos * sizeof(T)]), item, sizeof(T));

    if (_head == _tail) {
        _full = true;
        return Ringbuffer<T>::full;
    }

    return Ringbuffer<T>::ok;
}

template<typename T>
ssize_t Ringbuffer<T>::_write(int fd, uint32_t to)
{
    ssize_t r;
    size_t len;

    if (_tail == to && !_full) {
        log_debug("nothing to write");
        return 0;
    }

    if (_tail < to) {
        len = (to * sizeof(T)) - (_tail * sizeof(T)) - _tail_offset;
        r = ::write(fd, static_cast<void *>(&_buffer[(_tail * sizeof(T)) + _tail_offset]), len);
        if (r > 0) {
            _inc_tail_by_bytes(r);
        }
        return r;
    }

    // Write twice: from tail to the end, and from the beginning to the head
    len = (_num_elem * sizeof(T)) - (_tail * sizeof(T)) - _tail_offset;
    r = ::write(fd, static_cast<void *>(&_buffer[(_tail * sizeof(T)) + _tail_offset]), len);
    if (r > 0) {
        _inc_tail_by_bytes(r);
    }
    if (r <= 0 || (size_t)r < len) {
        // Fail or partial write, no need to try to write from beginning to head
        return r;
    }

    len = to * sizeof(T);
    r = ::write(fd, _buffer, len);
    if (r > 0) {
        _inc_tail_by_bytes(r);
    }
    return r;
}

template<typename T>
enum Ringbuffer<T>::add_result Ringbuffer<T>::_add_spaces(size_t n) {
    uint32_t vacancies;

    if (_full) {
        return Ringbuffer<T>::overflow;
    }

    if (_head < _tail) {
        vacancies = _tail - _head;
    } else {
        vacancies = (_num_elem - _head) + _tail;
    }

    if (n > vacancies) {
        return Ringbuffer<T>::overflow;
    }

    _head = (_head + n) % _num_elem;
    if (_head == _dec(_tail)) {
        _full = true;
        return Ringbuffer<T>::full;
    }

    return Ringbuffer<T>::ok;
}

template<typename T>
void Ringbuffer<T>::_inc_tail_by_bytes(size_t num_bytes)
{
    uint32_t inc;

    if (num_bytes < (sizeof(T) - _tail_offset)) {
        _tail_offset += num_bytes;
        return; // We couldn't even complete a pending tail
    }

    if (_tail_offset) {
        num_bytes -= sizeof(T) - _tail_offset;
        _tail_offset = 0;
        _tail = _inc(_tail);
    }

    inc = num_bytes / sizeof(T);
    _tail_offset = num_bytes % sizeof(T);

    _tail = (_tail + inc) % _num_elem;

    _full = _tail == _head;
}

template class Ringbuffer<uint8_t[200]>;
