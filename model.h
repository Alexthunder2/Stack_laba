#pragma once

#include <iterator>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <sstream>

template<typename T>
class Model;

template<typename T>
class Stack;

template<typename T>
class Visitor {
public:
	virtual T visit(const Model<T>* model) const = 0;
};


template <typename T>
class MaxVisitor : public Visitor<T> {
public:
	T visit(const Model<T>* model) const {
		if (model->IsEmpty()) return "";
		auto current_element = model->cbegin();
		T maximum = *current_element;
		for (int i = 1; i < model->Size(); i++) {
			current_element++;
			if (*current_element > maximum) {
				maximum = *current_element;
			}
		}
		return maximum;
	}
};

template <typename T>
class MinVisitor : public Visitor<T> {
public:
	T visit(const Model<T>* model) const {
		if (model->IsEmpty()) return "";
		auto current_element = model->cbegin();
		T minimum = *current_element;
		for (int i = 1; i < model->Size(); i++) {
			current_element++;
			if (*current_element < minimum) {
				minimum = *current_element;
			}
		}
		return minimum;
	}
};

template<typename T>
class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
	T operator*() const {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't dereference end iterator!");
		}
		return model_->Get(index_ + 1);
	}

	T* operator->() const {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't access end iterator!");
		}
		return &model_->Get(index_ + 1);
	}

	BaseIterator& operator++() {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't move further than end iterator!");
		}
		++index_;
		return *this;
	}

	const BaseIterator operator++(int) {
		BaseIterator result = *this;
		++(*this);
		return result;
	}

	BaseIterator& operator--() {
		if (index_ == 0) {
			throw std::out_of_range("Can't move earlier begin iterator!");
		}
		--index_;
		return *this;
	}

	const BaseIterator operator--(int) {
		BaseIterator result = *this;
		--(*this);
		return result;
	}

	bool operator==(const BaseIterator& other) const {
		return std::tie(model_, index_) == std::tie(other.model_, other.index_);
	}

	bool operator!=(const BaseIterator& other) const {
		return !(*this == other);
	}

private:
	friend class Stack<T>;

	const Model<T>* const model_;
	size_t index_;

	BaseIterator(const Model<T>* const model, size_t index)
		: model_(model), index_(index) {}
};

template<typename T>
class ConstBaseIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
	const T operator*() const {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't dereference end iterator!");
		}
		return model_->Get(index_ + 1);
	}

	const T* operator->() const {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't access end iterator!");
		}
		return &model_->Get(index_ + 1);
	}

	ConstBaseIterator& operator++() {
		if (index_ >= model_->Size()) {
			throw std::out_of_range("Can't move further than end iterator!");
		}
		++index_;
		return *this;
	}

	const ConstBaseIterator operator++(int) {
		if (index_ == model_->Size()) {
			throw std::out_of_range("Can't move later end iterator!");
		}
		ConstBaseIterator result = *this;
		++(*this);
		return result;
	}

	ConstBaseIterator& operator--() {
		if (index_ == 0) {
			throw std::out_of_range("Can't move earlier begin iterator!");
		}
		ConstBaseIterator result = *this;
		--(*this);
		return result;
	}

	const ConstBaseIterator operator--(int) {
		ConstBaseIterator result;
		--(*this);
		return result;
	}

	bool operator==(const ConstBaseIterator& other) const {
		return std::tie(model_, index_) == std::tie(other.model_, other.index_);
	}

	bool operator!=(const ConstBaseIterator& other) const {
		return !(*this == other);
	}

private:
	friend class Stack<T>;

	const Model<T>* const model_;
	size_t index_;

	ConstBaseIterator(const Model<T>* const model, size_t index)
		: model_(model), index_(index) {}
};

template<typename T>
class Model {
public:
	virtual void Push(const T& element) = 0;

	virtual void Pop() = 0;

	virtual T Get(int index) const = 0;

	virtual T Top() const = 0;

	virtual BaseIterator<T> begin() = 0;

	virtual BaseIterator<T> end() = 0;

	virtual ConstBaseIterator<T> cbegin() const = 0;

	virtual ConstBaseIterator<T> cend() const = 0;

	virtual int Size() const = 0;

	virtual int MaxSize() const = 0;

	virtual bool IsEmpty() const = 0;

	virtual bool IsFull() const = 0;

	virtual void Clear() = 0;

	virtual T Accept(const Visitor<T>& visitor) = 0; // return visitor.visit(this);

	virtual std::string GetStringForFirst() = 0;
	virtual std::string GetStringForLast() = 0;
	virtual std::string GetElementsString() = 0;

	virtual void SetStringForFirst(std::string str) = 0;
	virtual void SetStringForLast(std::string str) = 0;
	virtual void SetElementsString(std::string str) = 0;
};

template<typename T>
class Stack : public Model<T> {
public:
	explicit Stack(int max_size = 0) {
		data_ = new T[max_size];
		max_size_ = max_size;
		size_ = 0;
	}
	Stack(const std::initializer_list<T>& elements) : size_(elements.size()), max_size_(elements.size()) {
		data_ = new T[max_size_];
		for (int i = 0; i < size_; ++i) {
			data_[i] = *(elements.begin() + i);
		}
	}
	Stack(Stack&& other) noexcept : size_(other.size_), max_size_(other.max_size_) {


		data_ = other.data_;

		other.size_ = 0;
		other.max_size_ = 1;
		other.data_ = new T[1];
	}
	Stack(const Stack& other) : size_(other.size_), max_size_(other.max_size_) {
		data_ = new T[max_size_];

		for (int i = 0; i < size_; ++i) {
			data_[i] = other.data_[i];
		}
	}

	~Stack() {
		delete[] data_;
	}
	int Size() const {
		return size_;
	}
	int MaxSize() const {
		return max_size_;
	}
	bool IsEmpty() const {
		return (size_ == 0);
	}
	bool IsFull() const {
		return (size_ == max_size_);
	}

	T Get(int index = 1) const {
		if (index < 1) {
			throw std::invalid_argument("Index must be more than zero");
		}
		else if (size_ < index) {
			throw std::out_of_range("Stack has too few elements");
		}
		else {
			return data_[size_ - index];
		}
	}

	void Push(const T& element) {
		if (IsFull()) {
			T* data_copy_ = new T[max_size_ * 2 + 1];
			for (int i = 0; i < size_; i++) {
				data_copy_[i] = data_[i];
			}
			delete[] data_;
			data_ = data_copy_;
			max_size_ *= 2;
			max_size_++;
		}
		size_++;
		data_[size_ - 1] = element;
	}

	BaseIterator<T> begin() override {
		return BaseIterator<T>(this, 0);
	}

	BaseIterator<T> end() override {
		return BaseIterator<T>(this, size_);
	}

	ConstBaseIterator<T> cbegin() const override {
		return ConstBaseIterator<T>(this, 0);
	}

	ConstBaseIterator<T> cend() const override {
		return ConstBaseIterator<T>(this, size_);
	}

	void Clear() {
		size_ = 0;
		max_size_ = 1;
		delete[] data_;
		data_ = new T[1];
	}

	T Accept(const Visitor<T>& visitor) override {
		return visitor.visit(this);
	}

	Stack<T>& operator=(const Stack<T>& other) {
		max_size_ = other.max_size_;
		size_ = other.size_;
		delete[] data_;
		T* new_data_ = new T[max_size_];
		for (int i = 0; i < max_size_; i++) {
			new_data_[i] = other.data_[i];
		}
		data_ = new_data_;
		return *this;
	}
	Stack<T>& operator+=(const Stack<T>& other) {
		for (int i = 0; i < other.size_; i++) {
			Push(other.data_[i]);
		}
		return *this;
	}
	Stack<T> operator+(const Stack<T>& other) {
		Stack<T> summa(1);
		for (int i = 0; i < size_; i++) {
			summa.Push(data_[i]);
		}
		for (int i = 0; i < other.size_; i++) {
			summa.Push(other.data_[i]);
		}
		return summa;
	}

	Stack<T>& operator=(Stack&& other) noexcept {
		delete[] data_;
		size_ = other.size_;
		other.size_ = 0;
		max_size_ = other.max_size_;
		other.max_size_ = 1;
		data_ = other.data_;
		other.data_ = new T[1];
		return *this;
	}
	bool operator==(const Stack<T>& other) {
		if (size_ != other.size_) return false;
		for (int i = 0; i < size_; i++) {
			if (data_[i] != other.data_[i]) return false;
		}
		return true;
	}
	bool operator!=(const Stack<T>& other) {
		if (size_ != other.size_) return true;
		for (int i = 0; i < size_; i++) {
			if (data_[i] != other.data_[i]) return true;
		}
		return false;
	}

	T Top() const {
		if (IsEmpty()) {
			throw std::out_of_range("Stack is empty!");
		}
		else {
			return data_[size_ - 1];
		}
	}

	void Pop() {
		if (IsEmpty()) {
			throw std::out_of_range("Stack is empty");
		}
		else {
			size_--;
		}
	}

	void Swap(Stack<T>& other) {
		Stack<T> other_copy = other;
		other = std::move(*this);
		*this = std::move(other_copy);
	}
	
	std::string GetStringForFirst() {
		return str_for_first;
	}
	
	std::string GetStringForLast() {
		return str_for_last;
	}
	
	std::string GetElementsString() {
		return elements_string;
	}
	
	void SetStringForFirst(std::string str) {
		str_for_first = str;
	}
	
	void SetStringForLast(std::string str) {
		str_for_last = str;
	}
	
	void SetElementsString(std::string str) {
		elements_string = str;
	}

	friend std::istream& operator>>(std::istream& in, Stack<T>& stack);

	friend std::ostream& operator<<(std::ostream& out, const Stack<T>& stack);
private:
	T* data_;
	int size_;
	int max_size_;
	std::string str_for_first = "";

	std::string str_for_last = "";

	std::string elements_string = "";
};

template<typename T>
std::istream& operator>>(std::istream& in, Stack<T>& stack) {
	std::string line;
	while (getline(in, line)) {
		std::ostringstream line_reader(line);
		T str;
		while (line_reader >> str) {
			stack.Push(str);
		}
	}

	return in;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Stack<T>& stack) {
	if (stack.Size() > 0) {
		out << stack.Get(0);
		for (int i = 1; i < stack.Size(); ++i) {
			out << " " << stack.Get(i);
		}
	}

	return out;
}
