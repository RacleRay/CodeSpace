//��C++�У�(a+b) ���ű��ʽ�ǿ��Ը�ֵ��

//const�ǳ�������˼���������εı��������޸� 
//	������ε����ࡢ�ṹ�壨��ָ�룩�����ԱҲ�����Ը���

//const���ε������ұߵ�����

#include <iostream>
using namespace std;


struct Student {
	int age;
};

int main() {
	const Student d = { 18 };
	Student d2 = { 17 };

	//d = d2; ������

	const Student* p = &d2;
	//p->age = 19;  ����ͨ��ָ���޸�
	//(*p).age = 19;

	d2.age = 19; // ����


	int var1 = 10, var2 = 20;

	// * p1�����޸ģ�p1��
	// int��constλ�ÿ��Խ���
	const int* p1 = &var1;
	// p1�����޸ģ�*p1��
	int* const p1 = &var1;

	// �������޸�
	const int* const p2 = &var2;

}
