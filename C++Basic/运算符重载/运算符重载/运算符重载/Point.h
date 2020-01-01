#pragma once

#include <iostream>
using namespace std;

class Point {
	friend ostream& operator<<(ostream&, const Point&);
	friend istream& operator>>(istream&, Point&);
	int m_x;
	int m_y;
public:
	Point(int x, int y);

	// �������������������
	Point operator+(const Point& point) const;
	Point operator-(const Point& point) const;
	// ����
	const Point operator-() const;
	// �����Լ�����Ҫ�����¶���
	Point& operator+=(const Point& point);
	Point& operator-=(const Point& point);

	bool operator==(const Point& point);
	bool operator!=(const Point& point);

	// ǰ++
	Point& operator++();
	// ��++��ע��Ҫдint��һ�㶼����д���������м����
	const Point operator++(int);
};