//#pragma once
//��ifdef��define,endif��Ŀ����һ�µģ�ֻ����Ը��ļ�ȫ������

#ifndef __SUM_H
#define __SUM_H
// ʹ���ļ�����Ψһ�����궨�塣
// �궨�����ڷ�ֹ�ظ�����

// ����ֻ��C++�����еĵ��룬���϶���extern "C"
// C++�ļ�����Ĭ���к궨��#define __cplusplus
// C���򲻻����extern "C"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	int sum(int a, int b);
	int minus(int a, int b);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !__SUM_H
