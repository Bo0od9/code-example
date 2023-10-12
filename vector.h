#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <typename T>
class Vector {
 private:
  size_t capacity_;
  size_t size_;
  T* data_;

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : capacity_(0), size_(0), data_(nullptr) {
  }
  explicit Vector(const SizeType size) : capacity_(size), size_(size) {
    if (size > 0) {
      data_ = new T[size];
    } else {
      data_ = nullptr;
    }
  }
  Vector(const SizeType size, const ValueType& value) : capacity_(size), size_(size) {
    if (size > 0) {
      data_ = new T[size];
      try {
        std::fill(data_, data_ + size, value);
      } catch (...) {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    } else {
      data_ = nullptr;
    }
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) : capacity_(std::distance(first, last) * 2), size_(std::distance(first, last)) {
    if (size_ > 0) {
      data_ = new T[capacity_];
      try {
        std::copy(first, last, data_);
      } catch (...) {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    } else {
      data_ = nullptr;
    }
  }
  Vector(const std::initializer_list<ValueType> data) : Vector(data.begin(), data.end()) {
  }
  Vector(const Vector<T>& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (other.size_ > 0) {
      data_ = new T[capacity_];
      try {
        std::copy(other.data_, other.data_ + size_, data_);
      } catch (...) {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    } else {
      data_ = nullptr;
    }
  }
  Vector(Vector<T>&& other) noexcept {
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = other.data_;
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }
  ~Vector() noexcept {
    delete[] data_;
  }
  Vector<T>& operator=(const Vector<T>& other) {
    if (data_ != other.data_) {
      capacity_ = other.capacity_;
      size_ = other.size_;
      auto tmp = new T[capacity_];
      try {
        std::copy(other.data_, other.data_ + other.size_, tmp);
      } catch (...) {
        delete[] tmp;
        throw;
      }
      delete[] data_;
      data_ = tmp;
    }
    return *this;
  }
  Vector<T>& operator=(Vector<T>&& other) noexcept {
    if (data_ != other.data_) {
      delete[] data_;
      capacity_ = other.capacity_;
      size_ = other.size_;
      data_ = other.data_;
      other.capacity_ = 0;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }
  SizeType Size() const {
    return size_;
  }
  SizeType Capacity() const {
    return capacity_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  Pointer Data() {
    return data_;
  }
  ConstPointer Data() const {
    return data_;
  }
  Reference operator[](const SizeType index) {
    return data_[index];
  }
  ConstReference operator[](const SizeType index) const {
    return data_[index];
  }
  Reference At(const SizeType index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range.");
    }
    return data_[index];
  }
  ConstReference At(const SizeType index) const {
    if (index >= size_) {
      throw std::out_of_range("Index out of range.");
    }
    return data_[index];
  }
  Reference Front() {
    return data_[0];
  }
  ConstReference Front() const {
    return data_[0];
  }
  Reference Back() {
    return *(data_ + size_ - 1);
  }
  ConstReference Back() const {
    return *(data_ + size_ - 1);
  }
  void Swap(Vector<T>& other) {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }
  void Reserve(const SizeType new_cap) {
    if (new_cap > capacity_) {
      auto tmp = new T[new_cap];
      std::move(data_, data_ + size_, tmp);
      delete[] data_;
      capacity_ = new_cap;
      data_ = tmp;
    }
  }
  void Resize(const SizeType new_size) {
    if (new_size > capacity_) {
      Reserve(new_size * 2);
    }
    size_ = new_size;
  }
  void Resize(const SizeType new_size, const ValueType& value) {
    SizeType old_size = size_;
    Resize(new_size);
    if (old_size < new_size && data_ != nullptr) {
      std::fill(data_ + old_size, data_ + new_size, value);
    }
  }
  void ShrinkToFit() {
    if (capacity_ != size_) {
      if (size_ == 0) {
        delete[] data_;
        data_ = nullptr;
        capacity_ = 0;
      } else {
        auto tmp = new T[size_];
        std::move(data_, data_ + size_, tmp);
        delete[] data_;
        capacity_ = size_;
        data_ = tmp;
      }
    }
  }
  void Clear() {
    size_ = 0;
  }
  void PushBack(const T& value) {
    if (size_ == capacity_) {
      if (capacity_ == 0) {
        Reserve(2);
      } else {
        Reserve((size_)*2 - 1);
      }
    }
    data_[size_++] = value;
  }
  void PushBack(T&& value) {
    if (size_ == capacity_) {
      if (capacity_ == 0) {
        Reserve(2);
      } else {
        Reserve((size_)*2);
      }
    }
    data_[size_++] = std::move(value);
  }
  void PopBack() {
    if (size_ > 0) {
      --size_;
    }
    if (size_ == 0) {
      delete[] data_;
      data_ = nullptr;
    }
  }
  friend bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size_ != rhs.size_) {
      return false;
    }
    for (size_t i = 0; i < lhs.size_; ++i) {
      if (lhs.data_[i] != rhs.data_[i]) {
        return false;
      }
    }
    return true;
  }
  friend bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    bool equal = lhs == rhs;
    return !(equal);
  }
  friend bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return ((lhs == rhs) || (lhs < rhs));
  }

  friend bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return ((lhs == rhs) || (lhs > rhs));
  }
  friend bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size_ == 0 && rhs.size_ == 0) {
      return false;
    }
    if (lhs.size_ == 0 && rhs.size_ != 0) {
      return false;
    }
    if (lhs.size_ != 0 && rhs.size_ == 0) {
      return true;
    }
    if (lhs.size_ == rhs.size_) {
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater<T>());
    }
    if (lhs.size_ < rhs.size_) {
      for (size_t i = 0; i < lhs.size_; i++) {
        if (lhs.data_[i] > rhs.data_[i]) {
          return true;
        }
      }
      return false;
    }
    if (lhs.size_ > rhs.size_) {
      for (size_t i = 0; i < rhs.size_; i++) {
        if (lhs.data_[i] < rhs.data_[i]) {
          return false;
        }
      }
      return true;
    }
    return false;
  }
  friend bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size_ == 0 && rhs.size_ == 0) {
      return false;
    }
    if (lhs.size_ == 0 && rhs.size_ != 0) {
      return true;
    }
    if (lhs.size_ != 0 && rhs.size_ == 0) {
      return false;
    }
    if (lhs.size_ == rhs.size_) {
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::less<T>());
    }
    if (lhs.size_ < rhs.size_) {
      for (size_t i = 0; i < lhs.size_; i++) {
        if (lhs.data_[i] > rhs.data_[i]) {
          return false;
        }
      }
      return true;
    }
    if (lhs.size_ > rhs.size_) {
      for (size_t i = 0; i < rhs.size_; i++) {
        if (lhs.data_[i] < rhs.data_[i]) {
          return true;
        }
      }
      return false;
    }
    return false;
  }
  Iterator begin() {  // NOLINT
    return data_;
  }
  Iterator end() {  // NOLINT
    return data_ + size_;
  }
  ConstIterator begin() const {  // NOLINT
    return data_;
  }
  ConstIterator end() const {  // NOLINT
    return data_ + size_;
  }
  ConstIterator cbegin() const {  // NOLINT
    return data_;
  }
  ConstIterator cend() const {  // NOLINT
    return data_ + size_;
  }
  ReverseIterator rbegin() {  // NOLINT
    return ReverseIterator(end());
  }
  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(begin());
  }
  ConstReverseIterator rbegin() const {  // NOLINT
    return ConstReverseIterator(end());
  }
  ConstReverseIterator rend() const {  // NOLINT
    return ConstReverseIterator(begin());
  }
  ConstReverseIterator crbegin() const {  // NOLINT
    return ConstReverseIterator(end());
  }
  ConstReverseIterator crend() const {  // NOLINT
    return ConstReverseIterator(begin());
  }
};

#endif
