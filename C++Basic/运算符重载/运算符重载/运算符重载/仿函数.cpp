//�º������������󣩣���һ��������һ������һ����ʹ��

//�Ա���ͨ����������Ϊ������Ա���״̬��������ʹ�ó�Ա����


#include <iostream>
using namespace std;

class Sum {
	int var_count = 0;
public:
	int operator()(int a, int b) {
		var_count++;
		cout << var_count << endl;
		return a + b;
	}
};