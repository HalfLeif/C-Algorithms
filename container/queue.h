// Primitive dequeue implemented as circular array.
#ifndef QUEUE_H
#define QUEUE_H

#include "../base/logging.h"

namespace queue {

template <typename T>
class Queue {
 public:
  Queue();
  ~Queue();

  // Does not invalidate iterators.
  void push_back(T value);
  void push_front(T value);

  // Does not invalidate iterators.
  T&& pop_back();
  T&& pop_front();

  const T& front() const;
  const T& back() const;

  void debug() const;

  // Reserves memory for this many elements and moves the elements there.
  // Ignores call if is smaller than current size or equal to current capacity.
  // Does not invalidate iterators.
  void reserve(size_t capacity);
  void shrink_to_fit() { reserve(size_ + 1); }
  void clear() {
    size_ = 0;
    reset_ends();
  };

  size_t size() const { return size_; }
  size_t capacity() const { return capacity_; }
  bool empty() const { return size_ == 0; };

  class const_iterator {
   public:
    const_iterator& operator++();
    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const {
      return !this->operator==(other);
    }
    const T& operator*() const;

   private:
    const_iterator(size_t pos, const Queue<T>* queue) : pos_(pos), queue_(queue) {}
    friend class Queue<T>;

    // Doesn't point to actual element but to how many steps from back_ we are.
    size_t pos_;
    const Queue<T>* queue_;  // Not owned
  };

  // Iterates from back to front
  const_iterator begin() const;
  const_iterator end() const;

 private:
  void reset_ends() {
    back_ = prev(0);
    front_ = size_;
  }

  void grow_one() {
    if (size_ + 1 == capacity_) {
      reserve(capacity_ * 2);
    }
    // Note: must wait with incrementing size_ until after reserving is finished!
    ++size_;
  }

  void decr_one() {
    CHECK(!empty());
    --size_;
  }

  size_t next(size_t i) const {
    ++i;
    if (i == capacity_) return 0;
    return i;
  }

  size_t prev(size_t i) const {
    if (i == 0) return capacity_ - 1;
    return i - 1;
  }

  T* array_ = nullptr;
  size_t capacity_;
  size_t size_;

  // Always points to the next available position
  size_t back_;
  size_t front_;
};

// Implementation

template <typename T>
Queue<T>::Queue() {
  capacity_ = 10;
  array_ = new T[capacity_];
  size_ = 0;
  reset_ends();
}

template <typename T>
Queue<T>::~Queue() {
  delete[] array_;
}

template <typename T>
void Queue<T>::reserve(const size_t new_capacity) {
  if (new_capacity < size_ || new_capacity == capacity_) {
    return;
  }
  T* new_array = new T[new_capacity];
  if (!empty()) {
    size_t j = 0;
    for (size_t i = next(back_); i != front_; ) {
      new_array[j] = std::move(array_[i]);
      ++j;
      i = next(i);
    }
    CHECK(j == size_) << "j: " << j << " vs size_: " << size_;
  }
  capacity_ = new_capacity;
  // size_ is still the same
  reset_ends();

  delete[] array_;
  array_ = new_array;
}

template <typename T>
void Queue<T>::push_back(T value) {
  grow_one();
  array_[back_] = std::move(value);
  back_ = prev(back_);
}

template <typename T>
void Queue<T>::push_front(T value) {
  grow_one();
  array_[front_] = std::move(value);
  front_ = next(front_);
}

template <typename T>
T&& Queue<T>::pop_back() {
  decr_one();
  back_ = next(back_);
  return std::move(array_[back_]);
}

template <typename T>
T&& Queue<T>::pop_front() {
  decr_one();
  front_ = prev(front_);
  return std::move(array_[front_]);
}

template <typename T>
const T& Queue<T>::back() const {
  CHECK(!empty());
  return array_[next(back_)];
}

template <typename T>
const T& Queue<T>::front() const {
  CHECK(!empty());
  return array_[prev(front_)];
}

// Iterators

template <typename T>
typename Queue<T>::const_iterator& Queue<T>::const_iterator::operator++() {
  CHECK(queue_ != nullptr);
  pos_ = std::min(pos_ + 1, queue_->size_);
  return *this;
}

template <typename T>
bool Queue<T>::const_iterator::operator==(const Queue<T>::const_iterator& it) const {
  if (queue_ != it.queue_) return false;
  if (queue_ == nullptr && it.queue_ == nullptr) return true;

  // This is necessary in order to allow end iterator to continue being valid after deleting elements.
  size_t pos_a = std::min(pos_, queue_->size());
  size_t pos_b = std::min(it.pos_, it.queue_->size());
  return pos_a == pos_b;
}

template <typename T>
const T& Queue<T>::const_iterator::operator*() const {
  CHECK(queue_ != nullptr);
  size_t index = pos_ + queue_->next(queue_->back_);
  index %= queue_->capacity_;
  return queue_->array_[index];
}

template <typename T>
typename Queue<T>::const_iterator Queue<T>::begin() const {
  return Queue<T>::const_iterator(0, this);
}

template <typename T>
typename Queue<T>::const_iterator Queue<T>::end() const {
  return Queue<T>::const_iterator(size_, this);
}

template <typename T>
void Queue<T>::debug() const {
  auto& logger = LOG(INFO) << "Queue { \n\tsize: " << size_
      << "\n\tcapacity: " << capacity_
      << "\n\tback: " << back_
      << "\n\tfront: " << front_
      << "\n\tcontents: [";
  for (int i = next(back_); i != front_; i = next(i)) {
    logger << array_[i] << " ";
  }
  logger << "] \n}";
}
}  // namespace queue
#endif
