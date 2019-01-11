#ifndef ARRAY_H
#define ARRAY_H

/*
Author: Timothy Williams
Created On : 10-14-2018
Modified On: 10-16-2018
*/

#include <stdexcept>
#include <initializer_list>


template<class T>
class Array {
	
	T* arr;
	size_t m_length;
	
	void alloc();
	void dealloc();
	
public:
	
	Array();
	Array(size_t size);
	Array(std::initializer_list<T> array);
	Array(const Array<T>& other);
	Array(Array<T>&& other);
	~Array();
	
	Array<T>& operator=(const Array<T>& rhs);
	Array<T>& operator=(Array<T>&& rhs);
	
	T& operator[](int i);
	const T& operator[](int i) const;
	
	size_t length() const;
	size_t size() const;
	
	int find(const T& val) const;
	bool in(const T& val) const;
	
	Array<T> clone() const;
	void swap(Array<T>& other);
	
};


template<class T>
void Array<T>::alloc() {
	if (m_length > 0)
		arr = new T[m_length];
	else
		arr = NULL;
}
template<class T>
void Array<T>::dealloc() {
	if (arr != NULL)
		delete[] arr;
}

template<class T>
Array<T>::Array() {
	m_length = 0;
	arr = NULL;
}

template<class T>
Array<T>::Array(size_t size) {
	m_length = size;
	alloc();
}

template<class T>
Array<T>::Array(std::initializer_list<T> array) {
	this->m_length = array.size();
	alloc();
	for (int i = 0; i < this->m_length; i++)
		arr[i] = array.begin()[i];
}

template<class T>
Array<T>::Array(const Array<T>& other) {
	m_length = other.m_length;
	alloc();
	for (int i = 0; i < m_length; i++) 
		arr[i] = other.arr[i];
}

template<class T>
Array<T>::Array(Array<T>&& other) {
	m_length = other.m_length;
	this->arr = other.arr;
	other.arr = NULL;
}

template<class T>
Array<T>::~Array() {
	dealloc();
}

template<class T>
Array<T>& Array<T>::operator=(const Array<T>& rhs) {
	m_length = rhs.m_length;
	dealloc();
	alloc();
	for (int i = 0; i < m_length; i++) {
		arr[i] = rhs.arr[i];
	}
	return *this;
}

template<class T>
Array<T>& Array<T>::operator=(Array<T>&& rhs) {
	m_length = rhs.m_length;
	arr = rhs.arr;
	rhs.arr = NULL;
	return *this;
}

template<class T>
T& Array<T>::operator[](int i) {
	if (i >= m_length || i < 0)
		throw std::out_of_range("index out of range");
	return arr[i];
}

template<class T>
const T& Array<T>::operator[](int i) const {
	if (i >= m_length || i < 0)
		throw std::out_of_range("index out of range");
	return arr[i];
}

template<class T>
size_t Array<T>::length() const {
	return m_length;
}
template<class T>
size_t Array<T>::size() const {
	return m_length;
}

template<class T>
int Array<T>::find(const T& val) const {
	for (int i = 0; i < m_length; i++)
		if (arr[i] == val)
			return i;
	return -1;
}

template<class T>
bool Array<T>::in(const T& val) const {
	return find(val) + 1; //-1->0=false; 0->1=true
}

template<class T>
Array<T> Array<T>::clone() const {
	Array<T> a = *this;
	return a;
}

template<class T>
void Array<T>::swap(Array<T>& other) {
	int tmpLength = m_length;
	m_length = other.m_length;
	other.m_length = tmpLength;
	
	T* tmpArr = arr;
	arr = other.arr;
	other.arr = tmpArr;
}



#endif