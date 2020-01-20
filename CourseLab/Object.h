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

class Object // класс-интерфес
{
public:
	//загруз обьек из и в текст строку
	virtual std::ostream& PrintStream(std::ostream& oStream) { return oStream; }
	virtual std::istream& ScanStream(std::istream& iStream) { return iStream; }

	virtual int type() = 0; // уник идентификатор
	virtual string name() = 0; // строка с именем класса
	virtual Object* copy() = 0; // копирование
	virtual int cmp(Object*) = 0;
	virtual void add(Object*) = 0;
	virtual ~Object() {};
protected:
	//загруз обьек из и в текст строку
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
