//����������ʱ����

//��������û�б�������û�б�ָ��ָ��Ķ�����������ϵ�������


#include <iostream>
using namespace std;

class Car {
	int m_price;
public:
	Car() {
		cout << "Car(int) - " << this << " - " << this->m_price << endl;
	}

	Car(const Car& car) {
		cout << "Car(const Car &) - " << this << " - " << this->m_price << endl;
	}

	~Car() {

	}
};


void test1(Car car) {

}

Car test2() {
	Car car;
	return car;
}

int main() {
	Car(); // ��������,�������Ż���������ÿ������죬��������������ϵ���������
}


