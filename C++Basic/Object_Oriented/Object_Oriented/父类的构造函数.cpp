#include <iostream>
using namespace std;


// ����Ĺ��캯��Ĭ�ϻ���ø�����޲ι��캯�� (�ȵ���)
// �������Ĺ��캯����ʽ�ص����˸�����вι��캯�����Ͳ�����ȥĬ�ϵ��ø�����޲ι��캯��
// �������ȱ���޲ι��캯��������Ĺ��캯��������ʽ���ø�����вι��캯��

class Person {
	int m_age;
public:
	Person(int age = 90) :m_age(age) {
		cout << "Person(int age)" << endl;
	}

	~Person() {
		cout << "~Person" << endl;
	}
};

class Student : public Person {
	int m_score;
public:
	// �������ȱ���޲ι��캯��������Ĺ��캯��������ʽ���ø�����вι��캯��
	Student(int age = 18, int score = 80) :m_score(score), Person(age) {

	}

	// ���������˳���෴�������࣬�ٸ���
	~Student() {
		cout << "~Student" << endl;
	}
};

int main() {
	Student student;

	getchar();
	return 0;
}