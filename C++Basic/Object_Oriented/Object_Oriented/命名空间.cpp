// �����ռ������������������ͻ

// ����
//	��Ƕ��
//  Ĭ������������ռ�û�����֣�::��ʾ
namespace MJ {
	namespace SS {
		int g_no;
		class Person {

		};

		void test() {

		}
	}

	void test() {

	}
}


void test() {

}

// ʹ��
void test3() {
	test();
	// Ĭ�ϵ������ռ䣬û������
	::test();

	::MJ::SS::g_no = 30;

	MJ::SS::g_no = 20;

	using namespace MJ;
	SS::g_no = 30;

	using namespace MJ::SS;
	g_no = 10;
}

// using namespace std������iostream�ж����std�����ռ�

// Java��ͨ�����ļ�����ʽ���������ռ�
//  package com.mj.ss;  ָ��
//  ����
//  com.mj.ss.Class_name object_name;   ������Ӧ�ռ��е���