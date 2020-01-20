#pragma once
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

enum class eObjectType
{
	REAL,
	BIG_INT,
};

class Object // �����-��������
{
public:
	//������ ����� �� � � ����� ������
	virtual std::ostream& PrintStream(std::ostream& oStream) { return oStream; }
	virtual std::istream& ScanStream(std::istream& iStream) { return iStream; }

	virtual int type() = 0; // ���� �������������
	virtual string name() = 0; // ������ � ������ ������
	virtual Object* copy() = 0; // �����������
	virtual int cmp(Object*) = 0;
	virtual void add(Object*) = 0;
	virtual ~Object() {};
protected:
	//������ ����� �� � � ����� ������
	friend std::ostream& operator<<(std::ostream& oStream, Object& a) {
		return a.PrintStream(oStream);
	}
	friend std::istream& operator>>(std::istream& iStream, Object& a) {
		return a.ScanStream(iStream);
	}
	//void set_type(const eObjectType type) { obj_type = type; }
private:
	//eObjectType obj_type;
};
