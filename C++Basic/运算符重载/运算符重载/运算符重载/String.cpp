#include "String.h"

String::String(const char* cstring) {
	/*if (!cstring) return;
	cout << "String(const char *) - new[] - " << cstring << endl;

	this->m_cstring = new char[strlen(cstring) + 1] {};
	strcpy(this->m_cstring, cstring);*/
	assign(cstring);
}

String::String(const String& string) {
	// *this = string.m_cstring;
	// (*this).operator=(string.m_cstring);
	assign(string.m_cstring);
}

String::~String() {
	// if (!this->m_cstring) return;
	// cout << "~String() - delete[] - " << this->m_cstring << endl;
	//delete[] this->m_cstring;
	//this->m_cstring = NULL;

	// operator=(NULL);
	// *this = NULL;
	/*(*this).operator=(NULL);
	this->operator=(NULL);
	operator=(NULL);*/
	assign(NULL);
}

String& String::operator=(const char* cstring) {
	return assign(cstring);
}

String& String::operator=(const String& string) {
	// return operator=(string.m_cstring);
	// return *this = string.m_cstring;
	return assign(string.m_cstring);
}

String& String::assign(const char* cstring) {
	// ָ��һ���Ķѿռ�
	if (this->m_cstring == cstring) return *this;

	// �ͷžɵ��ַ���
	if (this->m_cstring) {
		cout << "delete[] - " << this->m_cstring << endl;

		delete[] this->m_cstring;
		this->m_cstring = NULL;
	}

	// ָ���µ��ַ���
	if (cstring) {
		cout << "new[] - " << cstring << endl;

		this->m_cstring = new char[strlen(cstring) + 1]{};
		strcpy(this->m_cstring, cstring);
	}

	return *this;
}

String String::operator+(const char* cstring) {
	String str;
	char* newCString = join(this->m_cstring, cstring);
	if (newCString) {
		// �ͷžɵĶѿռ�
		str.assign(NULL);
		// ֱ��ָ���¿��ٵĶѿռ�
		str.m_cstring = newCString;
	}
	return str;
}

String String::operator+(const String& string) {
	return operator+(string.m_cstring);
}

String& String::operator+=(const char* cstring) {
	char* newCString = join(this->m_cstring, cstring);
	if (newCString) {
		this->assign(NULL);
		this->m_cstring = newCString;
	}
	return *this;
}

String& String::operator+=(const String& string) {
	return operator+=(string.m_cstring);
}

bool String::operator>(const char* cstring) {
	if (!this->m_cstring || !cstring) return 0;
	return strcmp(this->m_cstring, cstring) > 0;
}

bool String::operator>(const String& string) {
	return operator>(string.m_cstring);
}

char String::operator[](int index) {
	if (!this->m_cstring || index < 0) return '\0';
	if (index >= strlen(this->m_cstring)) return '\0';
	return this->m_cstring[index];
}

char* String::join(const char* cstring1, const char* cstring2) {
	if (!cstring1 || !cstring2) return NULL;

	char* newCString = new char[strlen(cstring1) + strlen(cstring2) + 1]{};
	strcat(newCString, cstring1);
	strcat(newCString, cstring2);

	cout << "new[] - " << newCString << endl;
	return newCString;
}


ostream& operator<<(ostream& cout, const String& string) {
	if (!string.m_cstring) return cout;
	return cout << string.m_cstring;
}