#include <iostream>
using namespace std;

struct Person {
	int m_id;
	int m_age;
	int m_height;

	void display() {
		cout << "m_id = " << this->m_id << endl;
		cout << "m_age = " << this->m_age << endl;
		cout << "m_height = " << this->m_height << endl;
	}
};

int main() {
	Person person;
	person.m_id = 10;
	person.m_age = 20;
	person.m_height = 30;

	Person* pPerson = (Person*)&person.m_age;
	pPerson->m_id = 40;
	pPerson->m_age = 50;

	person.display();

	getchar();
	return 0;
}

//struct Person {
//	int m_id;
//	int m_age;
//	int m_height;
//
//	void display() {
//		// thisָ��Person�����ָ��
//		// this����洢�ľ���Person����ĵ�ֵַ�����������ߵĵ�ַ���������ʵ���ͬ���������
//		this->m_id = 5;
//		this->m_age = 6;
//		this->m_height = 7;
//
//		/*cout << "m_id = " << this->m_id << endl;
//		cout << "m_age = " << this->m_age << endl;
//		cout << "m_height = " << this->m_height << endl;*/
//	}
//};
//
//int main() {
//	Person person;
//	person.m_id = 1;
//	person.m_age = 2;
//	person.m_height = 3;
//	person.display();
//
//	Person person2;
//	person2.m_id = 4;
//	person2.m_age = 5;
//	person2.m_height = 6;
//	person2.display();
//	
//�ȴ�ָ����ȡ�������ַ
//�����ַ + ƫ�Ƽ����Ա������ַ
//��Ա������ַ�ռ����
//	/*Person *pPerson = &person;
//	pPerson->m_id = 4;
//	pPerson->m_age = 5;
//	pPerson->m_height = 6;*/
//
//	/*
//	ebp-14h == &person
//
//	 mov         dword ptr [ebp-14h],1  
//	 mov         dword ptr [ebp-10h],2  
//	 mov         dword ptr [ebp-0Ch],3 
//
//	 ebp-20h == &pPerson
//
//	 lea         eax,[ebp-14h]  
//	 mov         dword ptr [ebp-20h],eax  
//
//	 ȡ��person����ĵ�ֵַ����ֵ����eax
//	 mov         eax,dword ptr [ebp-20h] 
//	 mov         dword ptr [eax],4  
//
//	 ȡ��person����ĵ�ֵַ����ֵ����eax
//	 mov         eax,dword ptr [ebp-20h]  
//	 mov         dword ptr [eax+4],5  
//
//	 ȡ��person����ĵ�ֵַ����ֵ����eax
//	 mov         eax,dword ptr [ebp-20h]  
//	 mov         dword ptr [eax+8],6 
//	*/
//
//	/*cout << "&person = " << &person << endl;
//	cout << "&person.m_id = " << &person.m_id << endl;
//	cout << "&person.m_age = " << &person.m_age << endl;
//	cout << "&person.m_height = " << &person.m_height << endl;
//
//	cout << sizeof(person) << endl;*/
//
//	getchar();
//	return 0;
//}

//struct Person {
//	int m_age;
//
//	void run() {
//		cout << "run() - age is " << m_age << endl;
//	}
//};
//
//int main() {
//	Person person;
//	person.m_age = 20;
//��person�ĵ�ֵַ���뺯��
//	person.run();
//
//	Person *pPerson = &person;
//	pPerson->m_age = 30;
//��pPersonָ���д���ĵ�ֵַ���뺯��
//	pPerson->run();
//
//	// cout << sizeof(person) << endl;
//
//	getchar();
//	return 0;
//}

//struct Person {
////private:
//	// ��Ա����
//	int age;
//
//	// ��Ա����
//	void run() {
//		cout << "run() - age is " << age << endl;
//	}
//
////public:
////	void test() {
////		run();
////	}
//};
//
//class Student {
//public:
//	int no;
//  
//  �������ڴ���Σ�ֻ��һ�ݣ���new����ʱ��ֻ��new����Ա�����Ŀռ䣬Ȼ��ͨ��call [func]���ú���
//	void study() {
//		cout << "study() - no is " << no << endl;
//	}
//};
//
//int main() {
//	// ��ջ�ռ�������ڴ��person����
//	// ���person������ڴ���Զ����գ����ÿ�����Աȥ����
//	Person person;
//	person.age = 20;
//	person.run();
//
//	Person person2;
//	person2.age = 30;
//	person2.run();
//
//	Student student;
//	student.no = 100;
//	student.study();
//
//	getchar();