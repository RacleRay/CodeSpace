#include "������ʵ�ַ���.h"
#include <iostream>
using namespace std;

// ::���������
// ʵ�� .cpp Դ�ļ�
Person::Person() {
	cout << "Person()" << endl;
}

Person::~Person() {
	cout << "~Person()" << endl;
}

void Person::setAge(int age) {
	this->m_age = age;
}

int Person::getAge() {
	return this->m_age;
}