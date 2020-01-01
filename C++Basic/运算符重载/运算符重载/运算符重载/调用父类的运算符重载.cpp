#include <iostream>;
using namespace std;

class Person {
private:
	int m_age;
public:
	Person& operator=(const Person& person) {
		this->m_age = person.m_age;
	}
};

class Student :public Person {
private:
	int m_score;
public:
	Student& operator=(const Student& student) {
		// ���ø��๹�캯��������m_age
		Person::operator=(student);
		this->m_score = student.m_score;
	}
};