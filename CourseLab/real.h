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
	Real(const Real &x); //�����������
	Real(Real&& x); //�����������
	Real(const Real&& x); //�����������
	~Real();

	Real& operator=(Real&&); //�����������
	Real& operator=(const Real&); //������������ ������������

	// ��������������� ����� ������� � Object
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

    // ���������� 
	Real operator+(Real&);
	Real operator-(Real&);
	Real operator*(Real&);
	Real& operator-();
	Real& operator--();
	Real& operator++();

	// ���������
	bool operator>(Real&);
	bool operator<(Real&);
	bool operator==(Real&);
	bool operator>=(Real&);
	bool operator<=(Real&);
	
	//���������� � ������� ��������
	int compare(const Real&) const;
	void add(const Real&);
	void minus(const Real&);
	void mul(const Real&);
	
	// �����
	string toStr() const;
	string toStrNormal() const;
	void print();
	friend ostream& operator<<(ostream&, const Real&);
	friend istream& operator>>(istream&, Real&);
	// �������
	int getOrder();
	int getSign();
	char* getMantis();


private:
	int sign;
	int size;
	char* mantissa;
	int order;
	const int HASHCODE = typeid(Real).hash_code();
	//������
	void removeNull();
};