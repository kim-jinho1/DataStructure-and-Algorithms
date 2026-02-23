#pragma once
#include <iostream>

template <typename T>

class myVector
{
public:
	using iterator = T*;

	// 초기 capacity는 10으로 설정
	// 필요에 따라 capacity 직접 지정
	myVector(const size_t size = 10) {
		_data = new T[size];
		_size = 0;
		_capacity = size;
	}

	~myVector() {
		// 반납
		delete[] _data;
	}

public:
	// 요소 추가
	void push_back(const T& value) {
		// 할당된 메모리 보다 더 큰 요소가 추가될 때마다 capacity를 2배로 늘려서 메모리를 재할당
		if (_size >= _capacity) {							
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}

		_data[_size] = value;
		_size++;
	}

	// 새로운 메몰 할당과 기존 요소 복사
	void reserve(size_t newCap) {
		// 새로운 capacity가 현재 capacity보다 작거나 같으면 아무 작업도 하지 않음
		if (newCap <= _capacity)
			return;

		T* newData = new T[newCap];

		// 기존 요소를 새로운 메모리로 이동
		for (int i = 0; i < _size; i++) {
			newData[i] = move(_data[i]); // move를 사용하여 자원이나 시간을 절약
		}

		// 기존 메모리 해제
		delete[] _data;
		_data = newData;
		_capacity = newCap;
	}

public:
	// 복사 생성자
	myVector& operator=(const myVector& other) {
		// 자기 자신에게 할당하는 경우를 처리
		if (this == &other)
			return *this;

		// 현재 capacity가 다른 벡터의 size보다 작은 경우, 새로운 메모리를 할당
		if (_capacity < other._size) {
			delete[] _data;

			_capacity = other._capacity;
			_data = new T[_capacity];
		}

		_size = other._size;
		// 기존 요소를 다른 벡터의 요소로 복사
		for (int i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}

		return *this;
	}

	// 비교	연산자
	bool operator==(myVector& other) {
		// 크기가 다르면 다른 벡터로 간주
		if (_capacity != other._capacity)
			return false;

		// 요소를 하나씩 비교하여 모든 요소가 동일한지 확인
		for (int i = 0; i < _size; i++) {
			if (_data[i] != other[i])
				return false;
		}

		return true;
	}

	T& operator[](size_t idx) {
		return _data[idx];
	}

	bool operator!=(myVector& other) {
		return (!(*this == other));
	}

public:
	size_t size() const {
		return _size;
	}

	size_t capacity() const {
		return _capacity;
	}

	T& front() {
		return _data[0];
	}

	T& back() {
		return _data[_size - 1];
	}

public:
	iterator begin() {
		return _data;
	}

	iterator end() {
		return _data + _size;
	}

	// 요소 삭제
	iterator erase(iterator pos) {
		// pos가 유효한 범위에 있는지 확인
		if (pos < begin() || pos >= end())
			return end();

		// pos 뒤에 있는 요소들을 앞으로 이동시킴
		for (iterator it = pos; it != end(); ++it) {
			*it = move(*(it + 1));
		}

		// 마지막 요소는 더 이상 유효하지 않으므로 _size를 감소시킴
		_size--;

		// pos는 삭제된 요소의 위치를 가리키므로, 다음 요소로 이동하여 반환
		return pos;
	}

	// 요소 삽입
	iterator insert(iterator pos, const T& value) {
		// pos가 유효한 범위에 있는지 확인
		if (pos < begin() || pos >= end())
			return end();

		// 혹여 reserve발생시 pos는 delete된 요소를 가리키기 때문에 위치를 저장 하여 다시 계산
		size_t index = pos - begin();

		if (_size >= _capacity) {
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}

		// 저장한 위치를 기반으로 pos를 다시 계산
		pos = begin() + index;

		// pos부터 끝가지의 요소들을 뒤로 밀기
		for (iterator it = end(); it > pos; --it){
			*it = move(*(it - 1)); // move를 사용하여 자원이나 시간을 절약
		}

		// pos에 새로운 요소를 삽입
		*pos = value;
		_size++;

		return pos;
	}

private:
	T* _data;		  // 요소를 저장하는 동적 배열
	size_t _size;	  // 현재 벡터에 저장된 요소의 수
	size_t _capacity; // 할당된 메모리의 크기

};
