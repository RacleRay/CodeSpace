#include <iostream>
using namespace std;

struct Car {
	int m_price;
	Car() {
		cout << "Car()" << endl;
	}

	~Car() {
		cout << "~Car()" << endl;
	}
};

// �����ڲ�����Ķѿռ䣬�ɶ����ڲ�����

struct Person {
	int m_age; // 4
	Car* m_car; // 4��ջ�ռ��ָ��

	// ��ʼ������
	Person() {
		cout << "Person()" << endl;

		this->m_car = new Car();  // ָ��ѿռ�Ķ���

		/*Car car;
		this->m_car = &car;*/
	}

	// �ڴ���ա�������(����Person�����ڲ�����Ķѿռ�)
	//	���û��������������ô�������ù��̵�ջ�ռ���ղ����ڲ�����Ķѿռ䡣
	~Person() {
		cout << "~Person()" << endl;

		delete this->m_car;
	}
};

void test() {
	Person* p = new Person();

	delete p;
}

int main() {
	test();

	getchar();
	return 0;
}