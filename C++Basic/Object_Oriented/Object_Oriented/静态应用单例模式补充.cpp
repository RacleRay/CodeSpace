#include <iostream>
using namespace std;

#include <iostream>
using namespace std;
class Rocket {
public:
	static Rocket* sharedRocket() {
		if (ms_rocket) return ms_rocket;
		return ms_rocket = new Rocket();
	}
	static void deleteRocket() {
		if (!ms_rocket) return;

		delete ms_rocket;
		ms_rocket = NULL;
	}

private:
	static Rocket* ms_rocket;
	Rocket() { }
	// ��������˽�л�
	Rocket(const Rocket& rocket) { }
	// ��ֵ����˽�л�
	void operator=(const Rocket& rocket) { }
};

Rocket* Rocket::ms_rocket = NULL;

/*
������Ҫ��ֹ����������Ϊ����ֵ��Ϊ
*/

int main() {

	Rocket* p1 = Rocket::sharedRocket();
	Rocket* p2 = Rocket::sharedRocket();

	getchar();
	return 0;
}