// ʹ�ö���������Ϊ�����Ĳ������߷���ֵ�����ܻ����һЩ����Ҫ���м����


#include <iostream>
using namespace std;

class Car {
	int m_price;
public:
	Car(int price = 0) :m_price(price) {
		cout << "Car(int) - " << this << " - " << this->m_price << endl;
	}

	Car(const Car& car) :m_price(car.m_price) {
		cout << "Car(const Car &) - " << this << " - " << this->m_price << endl;
	}
};


void test1(Car car) {

}

void test1_ref(Car& car) {

}

Car test2() {
	Car car(10);
	return car;
}

int main() {
	// ����car1����
	Car car1(10);
	// �ڴ��ι����У�test1(Car car = car1),"Car car = car1"�ٴε��ÿ������캯����û�иú�����Ҳ�´����
	// �����м����
	test1(car1);

	test1_ref(car1);  // ʹ������
	
	///////////////////////////////////////////////////////////////////////
	
	// test2���ص�car�����ַ������ջ�ռ䣬��test2�������֮�󣬱�����
	// ���ԣ������������Car�Ŀ������캯������main�����ռ俽��һ��car����
	// ���ԣ����´�����������ι��캯��
	Car car3;
	car3 = test2();

	// ���´���������������ظ��Ŀ������캯�����ϲ�Ϊһ�ο������캯��
	Car car2 = test2();

	// ��ˣ����Ƽ����ض�������

	getchar();
	return 0;
}