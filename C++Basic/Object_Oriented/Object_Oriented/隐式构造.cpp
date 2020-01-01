// C++�д�����ʽ���������ĳЩ����£�����ʽ���õ������Ĺ��캯��

// ����ͨ���ؼ���explicit��ֹ����ʽ����


#include <iostream>;
using namespace std;

class Person {
	int m_age;
public:
	Person() {
		cout << "Person() - " << this << endl;
	}
	// ��ˣ�Ҫ������ʽ���������ʱ��ʹ��explicit
	/*explicit Person(int age) :m_age(age) {
		cout << "Person(int) - " << this << endl;
	}*/

	Person(int age) :m_age(age) {
		cout << "Person(int) - " << this << endl;
	}
	Person(const Person& person) {
		cout << "Person(const Person &person) - " << this << endl;
	}
	~Person() {
		cout << "~Person() - " << this << endl;
	}
	void display() {
		cout << "display() - age is " << this->m_age << endl;
	}
};

void test1(Person person) {

}

// �����˵������Ĺ��캯��
Person test2() {
	return 30;
}

int main() {

	// �����˵������Ĺ��캯��
	test1(40);

	// Person person = test2();
	// Person person(10);
	// person = test2();

	// �����˵������Ĺ��캯��
	Person person = 10;

	// Person person(10);
	person = 20;
	// person.display();

	// ��ˣ�Ҫ������ʽ���������ʱ��ʹ��explicit

	getchar();
	return 0;
}