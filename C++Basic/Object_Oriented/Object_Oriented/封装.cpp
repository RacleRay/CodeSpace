#include <iostream>
using namespace std;

//class Person {
//public:
//	int m_age;
//	void run() {
//		cout << "run()" << endl;
//	}
//};
//
//int main() {
//	Person person;
//	person.m_age = 10;
//	person.run();
//
//	return 0;
//}

struct Person {
private:
	int m_age;

public:
	void setAge(int age) {
		// ����
		if (age < 0) return;

		this->m_age = age;
	}

	int getAge() {
		return this->m_age;
	}
};


// ��C\C++�У������ں�������ľ���ȫ�ֱ���
// ȫ�ֱ������ص㣺�����������й����ж�����
int age;
Person p;

int main() {
	Person person;
	person.setAge(-20);

	cout << person.getAge() << endl;

	getchar();
	return 0;
}