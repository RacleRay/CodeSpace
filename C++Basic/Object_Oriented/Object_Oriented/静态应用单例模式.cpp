#include <iostream>
using namespace std;

/*
����ģʽ��
�ڳ������й����У����ܻ�ϣ��ĳЩ���ʵ��������Զֻ��һ��

	1.�ѹ��캯������������˽�л�
	2.����һ��˽�еľ�̬��Ա����ָ�룬����ָ��������
	3.�ṩһ�������ķ��ص�������ľ�̬��Ա����
*/


class Rocket {
public:
	static Rocket* sharedRoc(){
		if (ms_rocket == NULL) {
			ms_rocket = new Rocket();
		}
		return ms_rocket;
	}

	static void deleteRocket() {
		if (ms_rocket == NULL) return;

		delete ms_rocket;  // ���նѿռ�
		ms_rocket = NULL;  // �����ַ�ռ䣬�����������ۼ���
	}

private:
	Rocket() {
	}
	~Rocket() {
	}
	static Rocket* ms_rocket;
};

int main() {
	Rocket* p1 = Rocket::sharedRoc();
	Rocket::deleteRocket();

	Rocket* p2 = Rocket::sharedRoc();
	Rocket::deleteRocket();

	/*Rocket *p1 = Rocket::sharedRoc();
	Rocket *p2 = Rocket::sharedRoc();
	Rocket *p3 = Rocket::sharedRoc();
	Rocket *p4 = p3->sharedRoc();

	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
	cout << p4 << endl;*/

	getchar();
	return 0;
}
