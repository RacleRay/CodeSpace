#include <iostream>
using namespace std;

template <typename Item>
class Array {
	friend ostream& operator<<(ostream&, const Array<Item>&);
	int m_size = 0;
	int m_capacity = 0;
	Item* m_data = NULL;
public:
	Array(int capacity);
	~Array();
	void add(Item value);
	Item get(int index);
	int size();
	Item operator[](int index);
	void display();
};


template <typename Item>
Array<Item>::Array(int capacity) {
	if (capacity <= 0) return;

	this->m_data = new Item[capacity]{};
	this->m_capacity = capacity;
}

template <class Item>
Array<Item>::~Array() {
	if (!this->m_data) return;

	delete[] this->m_data;
	this->m_date = NULL;
}

template <class Item>
void Array<Item>::add(Item value) {
	if (this->m_size == this->m_capacity) {
		// ����
		cout << "��������" << endl;
		return;
	}
	this->m_data[this->m_size++] = value;
}

template <class Item>
Item Array<Item>::get(int index) {
	if (index < 0 || index >= this->m_size) return 0;
	return this->m_data[index];
}

template <class Item>
int Array<Item>::size() {
	return this->m_size;
}

template <class Item>
Item Array<Item>::operator[](int index) {
	return get(index);
}

template <class Item>
void Array<Item>::display() {
	cout << "[";
	for (int i = 0; i < this->m_size; i++) {
		cout << this->m_data[i];
		if (i != this->m_size - 1) {
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

template <class Item>
ostream& operator<<(ostream& cout, const Array<Item>& array) {
	cout << "[";
	for (int i = 0; i < array.m_size; i++) {
		cout << array.m_data[i];
		// if (i != 0)
		if (i != array.m_size - 1) {
			cout << ", ";
		}
	}
	return cout << "]";
}