#include <iostream>
#include "real.h"
#include <fstream>


Real::Real()
{
	//fstream F("log.txt", ios::app);
	//F << "Вызван обычный конструктор для обьекта: " << this << endl;
	sign = 1;
	mantissa = (char*)calloc(1, sizeof(char));
	order = 1;
	size = 1;
}

Real::Real(const Real& x)
{
	//fstream F("log.txt", ios::app);
	//F << "Вызван конструктор копирования для обьекта: " << this << " and " << &x << endl;
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
	//F << "Вызван конструктор перемещения для обьектов " << this << " и "<< &x << endl;
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
	//F << "Вызван конструктор перемещения(operator=) для обьектов " << this << " и " << &x << endl;
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
	//F << "Вызван конструктор копирования(operator=) для обьекта: " << this << " and " << &x << endl;
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
	//	F << "Вызван конструктор из строки для обьекта: "<< this << endl;
	int index, startindex;
	mantissa = (char*)calloc(str.length(), sizeof(char));
	
	//если первый знак - минус
	if (str[0] == '-')
	{
		sign = -1;
		startindex = 1;//тогда начинаем со второго
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
		if (str[index] == '.') // если встретили точку запишем значение нашей экспоненты
			order = index - startindex;
		else //запишем циферку
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
	//F << "Вызван деструктор для обьекта: " << this << endl;
	free(mantissa);
	sign = 1;
	order = size = 0;
	//F.close();
}

// преобразование в строку
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
// вывыд в консоль 
void Real::print()
{
	std::cout << this->toStr() << std::endl;
}

//удаляем лишние нули справа и слева
void Real::removeNull()
{
	//bool deleted = false;
	//нули слева удаляем
	int front = 0;//с какого индекса нач знач цифры
	int end = size; // на каком кончаются знач цифры
	while (size > 1 && mantissa[front] == 0)
	{
		front++;
		order--;
		size--;
	}
	//удаляем нули справа
	size_t n = size_t(std::fmax((long)1, order)); //до какого удаляем нули справа
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
	//если остался всего 1 нолик 
	if (size == 1 && mantissa[0] == 0)
	{
		sign = 1;
		order = 1;
	}
}
// 1 - больше 0 - равно -1 - меньше
int Real::compare(const Real& x) const
{
	// проверка по знаку и порядку
	if (sign > x.sign || order > x.order) // если 1-й полож а другой отриц или порядок 1 > 2
		return sign;
	else if (sign < x.sign) // если 1 отриц а другой полож
		return sign;
	if (order < x.order) // порядок 1 меньш 2-го
		return -1 * sign;
	else if (order > x.order)
		return sign;
	//если знак и порядок равны, проверим по 1 знач цифре
	for (int i = 0; i < size && i < x.size; i++)
	{
		if (mantissa[i] > x.mantissa[i])
			return sign;
		if (mantissa[i] < x.mantissa[i])
			return -sign;
	}
	if (size == x.size)
		return 0;
	//если 1 больше и все элем у них совпали на данный момент
	return size > x.size ? sign : -sign;
}

void Real::add(const Real& x)
{
	//если не одинаковы знаки, то отправляем на вычитание
	if (sign > x.sign)// a + -b
	{
		Real temp(x);
		temp.sign *= -1;//меняем знак
		this->minus(temp); // a - b
		return;
	}
	if (sign < x.sign)// -a + b
	{
		Real temp(x);
		sign *= -1;//меняем знак
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

	//выравниваем порядок
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


	//Real temp(*this); // создаём новое число

	sign = sign; // знак результата совпадает со знаком чисел
	free(mantissa);
	mantissa = (char*)calloc(len,sizeof(char)); // создаём вектор из len нулей

	// заполняем каждую ячейку вектора суммой двух соответствующих цифр
	for (size_t i = 0; i < msize; i++)
		mantissa[i + 1] = m1[i] + m2[i];

	// проверяем переполнения
	for (size_t i = len - 1; i > 0; i--) {
		mantissa[i - 1] += mantissa[i] / 10; // прибавляем к более старшему разряду десятки текущего
		mantissa[i] %= 10; // оставляем у текущего разряда только единицы
	}

	size = len;
	order = temp_order + 1; // восстанавливаем экспоненту
	removeNull(); // убираем нули
}

void Real::mul(const Real& x)
{
	//длина перемнож числа будет не более чем сумма их длинн
	int len = size + x.size;
	Real temp;
	temp.size = len;
	temp.mantissa = (char*)calloc(len, sizeof(char));
	temp.sign = sign * x.sign;
	temp.order = order + x.order;
	//перемножение в столбик
	for (int i = 0; i < size; i++)
		for (int j = 0; j < x.size; j++)
			temp.mantissa[i + j + 1] += mantissa[i] * x.mantissa[j];
	//если получились двузначные числа перенемсем в страший разряд 
	for (int i = len - 1; i > 0; i--)
	{
		temp.mantissa[i - 1] += temp.mantissa[i] / 10; // добавили десятки к левой от тек
		temp.mantissa[i] %= 10; // убрали десятки в тек
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
	//если не одинаковы знаки, то отправляем на сложение  тк -a - b or a -(-b)
	if (sign != x.sign)
	{
		sign *= -1; // делаем знаки одинаковыми
		this->add(x);
		sign *= -1; // вырнем обратно
		return;
	}
	//если два отрицательных числа -а -(-b)
	if (sign == -1 && x.sign == -1)
	{
		Real temp(x);
		temp.sign *= -1; // меняем знак у a и b
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

	// если два положительных числа а - б 		
	bool cmp = compare(x) >= 0 ? true : false; // флаг больше ли первое число чем второе
	int orderMin = cmp ? x.order : order;
	int orderMax = cmp ? order : x.order;
	int temp_order = int(std::fmax(orderMax, orderMin));

	int mBSize = cmp ? size : x.size;
	int mSSize = cmp ? x.size : size;

	//выравниваем порядок
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
	mantissa = (char*)calloc(len, sizeof(char)); // создаём вектор из len нулей

	// заполняем каждую ячейку вектора суммой двух соответствующих цифр
	for (size_t i = 0; i < maxSize; i++)
		mantissa[i + 1] = mB[i] - mS[i];

	// проверяем переполнения
	for (size_t i = len - 1; i > 0; i--) 
	{
		if (mantissa[i] < 0)
		{
			mantissa[i] += 10; // прибавляем к более старшему разряду десятки текущего
			mantissa[i-1]--; // оставляем у текущего разряда только единицы
		}
	}

	size = len;
	order = temp_order + 1; // восстанавливаем экспоненту
	sign = cmp ? 1 : -1; // знак результата совпадает со знаком большего числа
	removeNull(); // убираем нули
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
// 1 - а > b; 0 - a = b; -1 - a < b; -2 - typeA != typeB
int Real::cmp(Object* x)
{
	if (x->type() != this->type())//если сравниваются разного типа
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
	if (x->type() != this->type()) throw;//если сравниваются разного типа
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