#pragma once
#include <string>
#include <stdlib.h>
#include <iostream>
#include "Object.h"

using namespace std;

class Real : public Object
{
public:
	Real();
	Real(string str);
	Real(const Real &x); //копирование
	Real(Real&& x); //перемещение
	Real(const Real&& x); //перемещение
	~Real();

	Real& operator=(Real&&); //перемещение
	Real& operator=(const Real&); //присваивание копированием

	// переопределение общих методов с Object
	int get(string);
	string put();
	int cmp(Object*);
	int type();
	string name();
	Real* copy();
	void add(Object*);

	ostream& PrintStream(ostream& oStream);
	istream& ScanStream(istream& iStream);
	void SaveToFile(FILE*);
	void LoadFromFile(FILE*);

    // арифметика 
	Real operator+(Real&);
	Real operator-(Real&);
	Real operator*(Real&);
	Real& operator-();
	Real& operator--();
	Real& operator++();

	// сравнение
	bool operator>(Real&);
	bool operator<(Real&);
	bool operator==(Real&);
	bool operator>=(Real&);
	bool operator<=(Real&);
	
	//арифметика с текущим обьектом
	int compare(const Real&) const;
	void add(const Real&);
	void minus(const Real&);
	void mul(const Real&);
	
	// вывод
	string toStr() const;
	string toStrNormal() const;
	void print();
	friend ostream& operator<<(ostream&, const Real&);
	friend istream& operator>>(istream&, Real&);
	// геттеры
	int getOrder();
	int getSign();
	char* getMantis();


private:
	int sign;
	int size;
	char* mantissa;
	int order;
	const int HASHCODE = typeid(Real).hash_code();
	//методы
	void removeNull();
};