#include <iostream>
using namespace std;

// ��̳У����ӳ���ĸ��Ӷ�

// ��̳���һ���ܺõ���;��
// һ����ʵ�ֶ���ӿڣ�Java��
// һ�������ض��Э�飨OC��
// һ����̳ж�������ࣨC++��

class JobBaomu {
	virtual void clean() = 0;
	virtual void cook() = 0;
};

class JobTeacher {
	virtual void playBaseball() = 0;
	virtual void playFootball() = 0;
};

class SalesMan : public JobBaomu {
	void clean() {

	}

	void cook() {

	}
};

class Student : public JobBaomu, public JobTeacher {
	void clean() {

	}

	void cook() {

	}

	void playBaseball() {

	}

	void playFootball() {

	}
};

int main() {
	// ��ְ����
	// ��ƸһЩ��������ְ
	// ��ְ�ĸ�λ�ܶ��֣���ķ����ʦ�����ھ����
	// ӦƸ��������Ҳ�ܶࣨѧ����XXX��

	getchar();
	return 0;
}