#include <iostream>
#include "sum.h"
using namespace std;


// ��extern "C"���εĴ���ᰴ��C���Եķ�ʽȥ����
// �������ͬʱ��������ʵ�֣�Ҫ�ú���������extern "C"���Σ�����ʵ�ֿ��Բ�����

extern "C" void func();
extern "C" void func_2();

void func() {
	cout << "func()" << endl;
}
	
void func_2(int a) {
	cout << "func(int a) " << a << endl;
}


// ����C��C++�������Ĳ�ͬ����C��C++��Ͽ���ʱ�����ܻᾭ���������²���

// �������������ʹ��Cд��
// ͨ��extern "C"ָ������.c�ļ�ȥ����

// ���ַ�ʽ����ʵ�֣����ǲ��������
//extern "C" {
//	#include "math.h"
//}

