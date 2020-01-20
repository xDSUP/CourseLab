#include <iostream>
#include "real.h"
#include <fstream>


Real::Real()
{
	//fstream F("log.txt", ios::app);
	//F << "������ ������� ����������� ��� �������: " << this << endl;
	sign = 1;
	mantissa = (char*)calloc(1, sizeof(char));
	order = 1;
	size = 1;
}

Real::Real(const Real& x)
{
	//fstream F("log.txt", ios::app);
	//F << "������ ����������� ����������� ��� �������: " << this << " and " << &x << endl;
	sign = x.sign;
	order = x.order;
	size = x.size;
	mantissa = (char*)calloc(size, sizeof(char));
	for (int i = 0; i < size; i++)
		mantissa[i] = x.mantissa[i];
}

Real::Real(Real&& x)
{
	//fstream F("log.txt", ios::app);
	//F << "������ ����������� ����������� ��� �������� " << this << " � "<< &x << endl;
	if(size > 0 && mantissa != nullptr)
		free(mantissa);
	sign = x.sign;
	mantissa = x.mantissa;
	order = x.order;
	size = x.size;
	x.mantissa = (char*)calloc(1, sizeof(char));
	x.size = 1;
	x.sign = 1;
	x.order = 1;
}

Real& Real::operator=(Real&& x)
{
	//fstream F("log.txt", ios::app);
	//F << "������ ����������� �����������(operator=) ��� �������� " << this << " � " << &x << endl;
	free(mantissa);
	sign = x.sign;
	mantissa = x.mantissa;
	order = x.order;
	size = x.size;
	x.mantissa = (char*)calloc(1, sizeof(char));
	return *this;
}

Real& Real::operator=(const Real& x)
{
	//fstream F("log.txt", ios::app);
	//F << "������ ����������� �����������(operator=) ��� �������: " << this << " and " << &x << endl;
	if (mantissa != nullptr && size > 0)
		free(mantissa);
	sign = x.sign;
	order = x.order;
	size = x.size;
	mantissa = (char*)calloc(size, sizeof(char));
	for (int i = 0; i < size; i++)
		mantissa[i] = x.mantissa[i];
	return(*this);
}

Real::Real(string str)
{
	//fstream F("log.txt", ios::app);
	//if (F.is_open())
	//	F << "������ ����������� �� ������ ��� �������: "<< this << endl;
	int index, startindex;
	mantissa = (char*)calloc(str.length(), sizeof(char));
	
	//���� ������ ���� - �����
	if (str[0] == '-')
	{
		sign = -1;
		startindex = 1;//����� �������� �� �������
	}
	else
	{
		sign = 1;
		startindex = 0;
	}
	
	index = startindex;
	order = str.length() - index;
	int Ind = 0;
	size = 0;
	while (index < str.length())
	{
		if (str[index] == '.') // ���� ��������� ����� ������� �������� ����� ����������
			order = index - startindex;
		else //������� �������
		{
			size++;
			mantissa[Ind++] = str[index] - '0';
		}
		index++;
	}
	this->removeNull();
}

Real::~Real()
{
//	fstream F;
	//F.open("log.txt", ios::app);
	//F << "������ ���������� ��� �������: " << this << endl;
	free(mantissa);
	sign = 1;
	order = size = 0;
	//F.close();
}

// �������������� � ������
string Real::toStr() const
{
	string str = sign > 0 ? "0." : "-0.";
	//double* m = this->getMantis();
	for (int i = 0; i < size; i++)
	{
		str += mantissa[i] + '0';
	}
	str += " * 10^";
	str += std::to_string(order);
	return str;
}
// ����� � ������� 
void Real::print()
{
	std::cout << this->toStr() << std::endl;
}

//������� ������ ���� ������ � �����
void Real::removeNull()
{
	//bool deleted = false;
	//���� ����� �������
	int front = 0;//� ������ ������� ��� ���� �����
	int end = size; // �� ����� ��������� ���� �����
	while (size > 1 && mantissa[front] == 0)
	{
		front++;
		order--;
		size--;
	}
	//������� ���� ������
	size_t n = size_t(std::fmax((long)1, order)); //�� ������ ������� ���� ������
	while (size > n && mantissa[end - 1] == 0)
	{
		size--;
		end--;
	}
	char* temp = mantissa;
	mantissa = (char*)calloc(size, sizeof(char));
	
	for (int i = front; i < end; i++)
	{
		mantissa[i - front] = temp[i];
	}
	free(temp);
	//���� ������� ����� 1 ����� 
	if (size == 1 && mantissa[0] == 0)
	{
		sign = 1;
		order = 1;
	}
}
// 1 - ������ 0 - ����� -1 - ������
int Real::compare(const Real& x) const
{
	// �������� �� ����� � �������
	if (sign > x.sign || order > x.order) // ���� 1-� ����� � ������ ����� ��� ������� 1 > 2
		return sign;
	else if (sign < x.sign) // ���� 1 ����� � ������ �����
		return sign;
	if (order < x.order) // ������� 1 ����� 2-��
		return -1 * sign;
	else if (order > x.order)
		return sign;
	//���� ���� � ������� �����, �������� �� 1 ���� �����
	for (int i = 0; i < size && i < x.size; i++)
	{
		if (mantissa[i] > x.mantissa[i])
			return sign;
		if (mantissa[i] < x.mantissa[i])
			return -sign;
	}
	if (size == x.size)
		return 0;
	//���� 1 ������ � ��� ���� � ��� ������� �� ������ ������
	return size > x.size ? sign : -sign;
}

void Real::add(const Real& x)
{
	//���� �� ��������� �����, �� ���������� �� ���������
	if (sign > x.sign)// a + -b
	{
		Real temp(x);
		temp.sign *= -1;//������ ����
		this->minus(temp); // a - b
		return;
	}
	if (sign < x.sign)// -a + b
	{
		Real temp(x);
		sign *= -1;//������ ����
		temp.minus(*this); // b - a
		free(mantissa);
		mantissa = temp.mantissa;
		temp.mantissa = nullptr;
		//sign *= -1;
		order = temp.order;
		size = temp.size;		
		return;
	}
	int torder = order, xorder = x.order;
	int temp_order = int(std::fmax(torder, xorder));
	
	int m1Size = size;
	int m2Size = x.size;

	//����������� �������
	while (torder != temp_order)
	{
		m1Size++;
		torder++;
	}
	while (xorder != temp_order)
	{
		m2Size++;
		xorder++;
	}

	size_t msize = size_t(fmax(m1Size, m2Size));

	char* m1 = (char*)calloc(msize, sizeof(char));
	for (int i = temp_order - order, j = 0; i < m1Size; i++, j++)
		m1[i] = mantissa[j];

	char* m2 = (char*)calloc(msize, sizeof(char));
	for (int i = temp_order - x.order, j = 0; i < m2Size; i++, j++)
		m2[i] = x.mantissa[j];
	size_t len = 1 + msize;


	//Real temp(*this); // ������ ����� �����

	sign = sign; // ���� ���������� ��������� �� ������ �����
	free(mantissa);
	mantissa = (char*)calloc(len,sizeof(char)); // ������ ������ �� len �����

	// ��������� ������ ������ ������� ������ ���� ��������������� ����
	for (size_t i = 0; i < msize; i++)
		mantissa[i + 1] = m1[i] + m2[i];

	// ��������� ������������
	for (size_t i = len - 1; i > 0; i--) {
		mantissa[i - 1] += mantissa[i] / 10; // ���������� � ����� �������� ������� ������� ��������
		mantissa[i] %= 10; // ��������� � �������� ������� ������ �������
	}

	size = len;
	order = temp_order + 1; // ��������������� ����������
	removeNull(); // ������� ����
}

void Real::mul(const Real& x)
{
	//����� �������� ����� ����� �� ����� ��� ����� �� �����
	int len = size + x.size;
	Real temp;
	temp.size = len;
	temp.mantissa = (char*)calloc(len, sizeof(char));
	temp.sign = sign * x.sign;
	temp.order = order + x.order;
	//������������ � �������
	for (int i = 0; i < size; i++)
		for (int j = 0; j < x.size; j++)
			temp.mantissa[i + j + 1] += mantissa[i] * x.mantissa[j];
	//���� ���������� ���������� ����� ���������� � ������� ������ 
	for (int i = len - 1; i > 0; i--)
	{
		temp.mantissa[i - 1] += temp.mantissa[i] / 10; // �������� ������� � ����� �� ���
		temp.mantissa[i] %= 10; // ������ ������� � ���
	}
	free(mantissa);
	mantissa = temp.mantissa;
	size = len;
	sign = temp.sign;
	order = temp.order;
	temp.mantissa = nullptr;
	removeNull();
}

void Real::minus(const Real& x)
{
	//���� �� ��������� �����, �� ���������� �� ��������  �� -a - b or a -(-b)
	if (sign != x.sign)
	{
		sign *= -1; // ������ ����� �����������
		this->add(x);
		sign *= -1; // ������ �������
		return;
	}
	//���� ��� ������������� ����� -� -(-b)
	if (sign == -1 && x.sign == -1)
	{
		Real temp(x);
		temp.sign *= -1; // ������ ���� � a � b
		this->sign *= -1;
		temp.minus(*this);
		free(mantissa);
		mantissa = temp.mantissa;
		temp.mantissa = nullptr;
		sign = temp.sign *-1;
		size = temp.size;
		order = temp.order;
		return;
	}

	// ���� ��� ������������� ����� � - � 		
	bool cmp = compare(x) >= 0 ? true : false; // ���� ������ �� ������ ����� ��� ������
	int orderMin = cmp ? x.order : order;
	int orderMax = cmp ? order : x.order;
	int temp_order = int(std::fmax(orderMax, orderMin));

	int mBSize = cmp ? size : x.size;
	int mSSize = cmp ? x.size : size;

	//����������� �������
	while (orderMin != temp_order)	{
		mSSize++;
		orderMin++;
	}
	while (orderMax != temp_order)	{
		mBSize++;
		orderMax++;
	}

	size_t maxSize = size_t(fmax(mBSize, mSSize));

	char* mB = (char*)calloc(maxSize, sizeof(char));
	for (int i = temp_order - (cmp ? order : x.order), j = 0; i < mBSize; i++, j++)
		mB[i] = cmp ? mantissa[j] : x.mantissa[j];

	char* mS = (char*)calloc(maxSize, sizeof(char));
	for (int i = temp_order - (cmp ? x.order : order), j = 0; i < mSSize; i++, j++)
		mS[i] = cmp ? x.mantissa[j] : mantissa[j];
	size_t len = 1 + maxSize;

	free(mantissa);
	mantissa = (char*)calloc(len, sizeof(char)); // ������ ������ �� len �����

	// ��������� ������ ������ ������� ������ ���� ��������������� ����
	for (size_t i = 0; i < maxSize; i++)
		mantissa[i + 1] = mB[i] - mS[i];

	// ��������� ������������
	for (size_t i = len - 1; i > 0; i--) 
	{
		if (mantissa[i] < 0)
		{
			mantissa[i] += 10; // ���������� � ����� �������� ������� ������� ��������
			mantissa[i-1]--; // ��������� � �������� ������� ������ �������
		}
	}

	size = len;
	order = temp_order + 1; // ��������������� ����������
	sign = cmp ? 1 : -1; // ���� ���������� ��������� �� ������ �������� �����
	removeNull(); // ������� ����
}


int Real::getSign()
{
	return this->sign;
}

int Real::getOrder()
{
	return this->order;
}

char* Real::getMantis()
{
	return this->mantissa;
}


string Real::toStrNormal () const
{
	string str = "";
	if (sign == -1)
		str += "-";
	int o = order;
	if (order <= 0)
	{
		str += "0.";
		while (o < 0) 
		{
			str += "0";
			o++;
		}
	}
	for (int i = 0; i < size; i++)
	{
		if (order > 0 && i == order)
		{
			str += '.';
		}
		str += mantissa[i] + '0';
	}
	return str;
}

ostream& operator<<(ostream& o, const Real& x)
{
	o << x.toStrNormal() << " ";
	return o;
}

istream& operator>>(istream& i, Real& x)
{
	//free(x.mantissa);
	string n;
	i >> n;
	x = Real(n);
	//i >> x.sign;
	//i >> x.order;
	//i >> x.size;
	//x.mantissa = (char*)malloc(sizeof(char)*x.size);
	//for (int j = 0; j < x.size; j++)
	//{
	//	char t;
	//	i >> t;
	//	x.mantissa[j] = t - '0';
	//}
	return i;
}

Real Real::operator+(Real& x)
{
	Real temp(*this);
	temp.add(x);
	return temp;
}

Real Real::operator-(Real& x)
{
	Real temp(*this);
	temp.minus(x);
	return temp;
}

Real Real::operator*(Real& x)
{
	Real temp(*this);
	temp.mul(x);
	return temp;
}

Real& Real::operator-()
{
	sign *= 1;
	return *this;
}

Real& Real::operator--()
{
	order--;
	return *this;
}

Real& Real::operator++()
{
	order++;
	return *this;
}

bool Real::operator>(Real& x)
{
	return this->compare(x) == 1;
}

bool Real::operator<(Real& x)
{
	return this->compare(x) == -1;
}

bool Real::operator==(Real& x)
{
	return this->compare(x) == 0;
}

bool Real::operator>=(Real& x)
{
	return this->compare(x) >= 0;
}

bool Real::operator<=(Real& x)
{
	return this->compare(x) <= 0;
}


int Real::get(string str)
{
	Real temp(str);
	*this = temp;
	temp.mantissa = nullptr;
	return 1;
}

string Real::put()
{
	return this->toStr();
}

string Real::name()
{
	return typeid(Real).name();
}
// 1 - � > b; 0 - a = b; -1 - a < b; -2 - typeA != typeB
int Real::cmp(Object* x)
{
	if (x->type() != this->type())//���� ������������ ������� ����
		return -2;
	return this->compare(*(Real*)x);
}

Real* Real::copy()
{
	return new Real(*this);
}

int Real::type()
{
	return HASHCODE;
}


void Real::add(Object* x)
{
	if (x->type() != this->type()) throw;//���� ������������ ������� ����
	this->add(*(Real*)x);
}


ostream& Real::PrintStream(ostream& oStream)
{
	oStream << *this;
	return oStream;
}

istream& Real::ScanStream(istream& iStream)
{
	iStream >> *this;
	return iStream;
}

void SaveToFile(FILE*)
{

}

void LoadFromFile(FILE*)
{

}