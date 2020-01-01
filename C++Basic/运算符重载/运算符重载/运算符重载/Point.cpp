#include "Point.h"

Point::Point(int x, int y) :m_x(x), m_y(y) { }

// �������������������
// ��һ��const����������ֵ���ڶ���const����Ϊ��������p1 + p2 + p3������p1 + p2
// ����const���󣬶�const����ֻ�ܵ���const��Ա��������ֹ��Ա���޸ģ�const�����
// ��ˣ����ϵڶ���const
Point Point::operator+(const Point& point) const {
	return Point(this->m_x + point.m_x, this->m_y + point.m_y);
}

Point Point::operator-(const Point& point) const {
	return Point(this->m_x - point.m_x, this->m_y - point.m_y);
}

// ��������
const Point Point::operator-() const {
	return Point(-this->m_x, -this->m_y);
}

// ���ض������ã���ֹ�м�������
// �����ؾ���Ķ���Point��1�� 2��������Ҫָ��const
Point& Point::operator+=(const Point& point) {
	this->m_x += point.m_x;
	this->m_y += point.m_y;
	return *this;
}

Point& Point::operator-=(const Point& point) {
	this->m_x -= point.m_x;
	this->m_y -= point.m_y;
	return *this;
}

bool Point::operator==(const Point& point) {
	// 1 YES true
	// 0 NO false
	/*if (this->m_x == point.m_x && this->m_y == point.m_y) {
		return 1;
	}
	else {
		return 0;
	}*/
	return (this->m_x == point.m_x) && (this->m_y == point.m_y);
}

bool Point::operator!=(const Point& point) {
	return (this->m_x != point.m_x) || (this->m_y != point.m_y);
}

// ǰ++
Point& Point::operator++() {
	this->m_x++;
	this->m_y++;
	return *this;
}

// ��++
// ʹ����ʱpoint������
// cout�������ǲ���ʹ��=��ֵ����ģ���Ϊ=�����غ���д��private��
const Point Point::operator++(int) {
	Point point(this->m_x, this->m_y);
	this->m_x++;
	this->m_y++;
	return point;
}

// ע���Ƿ������ڳ�Ա������
// ע��ostream��cout����������Ϊconst
// ע��Ҫ����cout���󣬲����ٴν��� << ����
// �൱��*this.operator<<(cout, point)��������
ostream& operator<<(ostream& cout, const Point& point) {
	return cout << "(" << point.m_x << ", " << point.m_y << ")";
}

// cin���룬�޸�point������point����Ϊconst
istream& operator>>(istream& cin, Point& point) {
	cin >> point.m_x;
	cin >> point.m_y;
	return cin;
}