// ����Lambda���ʽ�������б��е� auto

#include <iostream>
using namespace std;

auto func = [](auto a, auto b) {return a + b; };


// �Բ���ı������г�ʼ��
int a;
auto func_2 = [a = 10](){
	cout << a << endl;
};



// C++17
int main() {
	//���Խ��г�ʼ����if��switch���
	if (int a = 10; a > 10) {
		a = 1;
	} else if (int b = 20; a > 5 && b > 10) {
		b = 2;
		a = 2;
	}
}
