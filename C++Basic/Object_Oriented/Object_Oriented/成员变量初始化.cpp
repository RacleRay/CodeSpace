#include <iostream>
using namespace std;


// ����Զ����˹��캯����
// ����ȫ�����������ڴ�ռ�ĳ�Ա����Ĭ�϶����ᱻ��ʼ������Ҫ������Ա�ֶ���ʼ��

struct Person {
	int m_age;

	/*Person() {
		cout << "Person()" << endl;
		this->m_age = 0;
	}*/

	// ȫ����Ա������ʼ��Ϊ0��㷽ʽ
	/*Person() {
		memset(this, 0, sizeof(Person));
	}*/
};

// û�й��캯����Ĭ�������£�ȫ��������Ա������ʼ��Ϊ0��
// �й��캯��: ȫ��������Ա������ʼ��Ϊ0��
Person g_person;

int main() {
	// û�й��캯����Ĭ�������£�ջ�ռ䣨��Ա����û�г�ʼ����
	Person person;

	// �ѿռ�
	Person* p1 = new Person; // û�й��캯����Ĭ�������£���Ա����û�г�ʼ��
	Person* p2 = new Person(); // û�й��캯����Ĭ�������£���Ա�����г�ʼ��0

	cout << "g_person " << g_person.m_age << endl;
	cout << "p1 " << p1->m_age << endl;
	cout << "p2 " << p2->m_age << endl;

	getchar();
	return 0;
}