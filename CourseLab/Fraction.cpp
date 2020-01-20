#include "Fraction.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#define TEST

//вспомогательная функция - поиск числителя и знаменателся
void FindChZn(double num, double eps, int& ch, int& zn)
{
	int a = 1; int b = 1;
	int mn = 2; // множитель для начального приближения
	int iter = 0;
	ch = a; zn = b;
	// Поиск начального приближения
	double c = 1;
	do {
		b++;
		c = (double)a / b;
	} while ((num - c) < 0);
	if ((num - c) < eps)
	{
		ch = a; zn = b;
		return;
	}
	b--;
	c = (double)a / b;
	if ((num - c) > -eps)
	{
		ch = a; zn = b;
		return;
	}
	// Уточнение
	while (iter < 20000)
	{
		int cc = a * mn, zz = b * mn;
		iter++;
		do {
			zz++;
			c = (double)cc / zz;
		} while ((num - c) < 0);
		if ((num - c) < eps)
		{
			ch = cc; zn = zz;
			return;
		}
		zz--;
		c = (double)cc / zz;
		if ((num - c) > -eps)
		{
			ch = cc; zn = zz;
			return;
		}
		mn++;
	}
}
//загруз обьек из и в текст строку
std::ostream& Fraction::PrintStream(std::ostream& oStream) {
	oStream << *this;
	return oStream;
}
std::istream& Fraction::ScanStream(std::istream& iStream) {
	iStream >> *this;
	return iStream;
}
//возврат уник идентифик(хеш с минимально возможным количеством коллизий)
int Fraction::type() {
	return typeid(Fraction).hash_code();
}
//возврат указат на строку с именем класа
std::string Fraction::name() {
	return typeid(Fraction).name();
}
//операторы приобразования
Fraction::operator double() {
	double val = number[0] + (double(number[1]) / number[2]);
	if (minus) val *= -1;
	return val;
}
//сложение(обьединение)
void Fraction::add(Object* val) {
	if (val->type() == this->type())
		return;
	this->f_plus(*(Fraction*)val);
}
//создание динам копии обьекта
Fraction* Fraction::copy() {
	Fraction* pval = new Fraction(*this);
	return pval;
}

int Fraction::cmp(Object* x)
{
	if (x->type() != this->type())//если сравниваются разного типа
		return -2;
	return this->compare(*(Fraction*)x);
}


//Методы-конструкторы
//1. Пустой конструктор. Создаёт дробь, эквивалентную 0.
Fraction::Fraction() {
	minus = false;
	number = new int[3];
	for (int i = 0; i < 2; i++)
		number[i] = 0;
	number[2] = 1;
#ifdef TEST
	std::cout << "Created!\n";
#endif // TEST
}

	//2. Конструктор из параметра-строки
Fraction::Fraction(std::string &str) {
	this->load_from_str(str);
	this->normalize();
#ifdef TEST
	std::cout << "Created!\n";
#endif // TEST
}

//3. Конструктор из параметра-массива
		//Массив должен содержать данные в виде {целая часть, числитель, знаменатель}
Fraction::Fraction(const int *mas) {
	//Если в дроби есть отрицательное число - делаем её отрицательной
	if ((mas[0] < 0) || (mas[1] < 0) || (mas[2] < 0))
		minus = true;
	else
		minus = false;
	number = new int[3];
	number[0] = mas[0];
	number[1] = mas[1];
	number[2] = mas[2];
	this->normalize();
#ifdef TEST
	std::cout << "Created!\n";
#endif // TEST
}

// 4. Коснструктор копирования
Fraction::Fraction(const Fraction & src)
{
	minus = src.minus;
	number = new int[3];
	for (int i = 0; i < 3; i++)
		number[i] = src.number[i];
}

Fraction::Fraction(Fraction && src)
{
	delete[] number;
	number = src.number;
	src.number = nullptr;
	minus = src.minus;
	// this->move(src) //Почему нельзя так?
}

Fraction::Fraction(const int i)
{
	minus = false;
	number = new int[3];
	number[0] = i;
	number[1] = 0;
	number[2] = 0;
	normalize();
}

//Метод копирования объекта
	//src - дробь-источник
void Fraction::copy(const Fraction &src) {
	this->minus = src.minus;
	this->number[0] = src.number[0];
	this->number[1] = src.number[1];
	this->number[2] = src.number[2];
}

void Fraction::move(Fraction && src)
{
	delete[] number;
	number = src.number;
	src.number = nullptr;
	minus = src.minus;
}

//Меняет целую часть дроби
void Fraction::set_whole(const int i)
{
	this->number[0] = i;
}

//Меняет числитель дроби
void Fraction::set_numerator(const int i)
{
	this->number[1] = i;
	this->normalize();
}

//Меняет знаменатель дроби
void Fraction::set_denominator(const int i)
{
	this->number[2] = i;
	this->normalize();
}

//Меняет знак числа
void Fraction::set_pos_neg(const bool znak)
{
	this->minus = znak;
}

//Деструктор
Fraction::~Fraction() {
	delete[] number;
#ifdef TEST
	std::cout << "Deleted!\n";
#endif // TEST
}

//Метод вывода дроби на экран
void Fraction::print_fraction() const {
	if (minus)
		std::cout << "- ";
	std::cout << number[0] << " " << number[1] << "/" << number[2];
}

//Метод ввода (редактирования) дроби с клавиатуры
void Fraction::edit_from_keyboard() {
	std::cout << "Введите элементы правильной дроби в следующем порядке: целая часть, числитель, знаменатель: ";
	std::cin >> number[0] >> number[1] >> number[2];
	this->normalize();
}

//Метод вывода дроби в строку
std::string Fraction::fraction_to_string() {
	std::string Fraction_str;
	//Посимвольно складываем строку
	char *whole_num_char = new char[lenof(number[0]) + 1];
	sprintf_s(whole_num_char, lenof(number[0]) + 1, "%d", number[0]);
	char *numerator_char = new char[lenof(number[1]) + 1];
	sprintf_s(numerator_char, lenof(number[1]) + 1, "%d", number[1]);
	char *denominator_char = new char[lenof(number[2]) + 1];
	sprintf_s(denominator_char, lenof(number[2]) + 1, "%d", number[2]);
	if (minus)
		Fraction_str += "-";
	Fraction_str += whole_num_char;
	Fraction_str += " ";
	Fraction_str += numerator_char;
	Fraction_str += "/";
	Fraction_str += denominator_char;

	//Освобождаем память
	delete whole_num_char;
	delete numerator_char;
	delete denominator_char;
	return Fraction_str;
}

//Нормализирует дробь: делает её правильной, делая числитель меньше знаменателя и выделяя целую часть,
	//если необходимо.
void Fraction::normalize() {
	// Знаменатель не может быть равен 0
	if (this->number[2] == 0) {
		this->number[2] = 1;
	}
	this->abs_frac();
	//Находим НОД числителя и знаменателя и делим их на него
	int nod = this->nod(number[1], number[2]);
	if (nod > 1) {
		number[1] /= nod;
		number[2] /= nod;
	}
	//Если числитель меньше знаменателя, ничего не делаем
	if (number[1] < number[2])
		return;
	//Если знаменатель равен 0 - ничего не делаем
	if (number[2] == 0) {
		std::cout << "Обнаружен знаменатель равный нулю!\n";
		//printf_s("Обнаружен знаменатель равный нулю!\n");
		return;
	}
	//Иначе вычитаем из числителя знаменатель, увеличивая целую часть, пока чис-ль не станет меньше знам-ля
	while (number[1] >= number[2]) {
		number[0]++;
		number[1] -= number[2];
	}
}

//Вспомогательная функция: делает все элементы дроби неотрицательными числами
	//и меняет флаг minus, если необходимо.
void Fraction::abs_frac() {
	if ((minus) || (number[0] < 0) || (number[1] < 0) || (number[2] < 0))
		minus = true;
	else {
		minus = false;
		return;
	}
	number[0] = abs(number[0]);
	number[1] = abs(number[1]);
	number[2] = abs(number[2]);
}

//Вспомогательная функция: возвращает длину числа x
int Fraction::lenof(int x) const {
	int len = 0;
	if (x == 0)
		return 1;
	while (x >= 1)
	{
		x /= 10;
		len++;
	}
	return len;
}

//Вспомогательная функция: возвращает НОД чисел a и b
int Fraction::nod(int a, int b) const {
	int t;
	if(a < b) {
		t = a;
		a = b;
		b = t;
	}
	while (b != 0) {
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

int Fraction::nok(int n1, int n2) const
{
	return n1 * n2 / nod(n1, n2);
}

//Возвращает указатель на number
int* Fraction::get_number() {
	return number;
}

bool Fraction::get_minus()
{
	return minus;
}

// Реализация +
Fraction& Fraction::operator+(const Fraction & fract_2) const
{
	Fraction& temp_fract = *(new Fraction(*this));
	temp_fract += fract_2;
	return temp_fract;
}

Fraction& Fraction::operator-(const Fraction & fract_2) const
{
	Fraction& temp_fract = *(new Fraction(*this));
	temp_fract -= fract_2;
	return temp_fract;
}

Fraction& Fraction::operator*(const Fraction & fract_2) const
{
	Fraction& temp_fract = *(new Fraction(*this));
	temp_fract *= fract_2;
	return temp_fract;
}

Fraction& Fraction::operator/(const Fraction & fract_2) const
{
	Fraction& temp_fract = *(new Fraction(*this));
	temp_fract /= fract_2;
	return temp_fract;
}

// Реализация =
Fraction & Fraction::operator=(const Fraction & right)
{
	if (this != &right) {
		this->copy(right);
	}

	return *this; // Конвеер ссылок
}

Fraction & Fraction::operator=(Fraction && right)
{
	if (this != &right) {
		delete[] number;
		number = right.number;
		right.number = nullptr;
		minus = right.minus;
	}

	return *this; // Конвеер ссылок
}


std::ostream & operator<<(std::ostream & out, Fraction & fract)
{
	if (fract.minus)
		out << "-";
	if (fract.number[0] != 0) {
		out << fract.number[0] << " ";
	}
	if (fract.number[1] != 0) {
		out << fract.number[1] << "/" << fract.number[2];
	}
	return out;
}

std::istream & operator>>(std::istream & in, Fraction & fract)
{
	fract.minus = false;
	in >> fract.number[0] >> fract.number[1] >> fract.number[2];
	fract.normalize();
	return in;
}


bool Fraction::operator==(const Fraction & right) const
{
	// "compare" возвращает 0, если числа равны. "!" позволяет нам ловить этот случай.
	return compare(right) == 0;
}

bool Fraction::operator<(const Fraction & right) const
{
	// Проверяем: если compare возвращает -1, то возвращаем true
	return compare(right) < 0;
}

bool Fraction::operator>(const Fraction & right) const
{
	// Проверяем: если compare возвращает -1, то возвращаем true
	return compare(right) > 0;
}

bool Fraction::operator<=(const Fraction & right) const
{
	return compare(right) <= 0;
}

bool Fraction::operator>=(const Fraction & right) const
{
	return compare(right) >= 0;
}

void Fraction::f_plus(const Fraction &fract_2){   // f_plus(const Fraction& p2)
	Fraction temp;
	temp.copy(fract_2);

	// Делаем дроби неправильными
	this->number[1] += this->number[0] * this->number[2];
	this->number[0] = 0;

	temp.number[1] += temp.number[0] * temp.number[2];
	temp.number[0] = 0;

	//Если дробь отрицательна - умножаем числитель на -1
	if (this->minus) {
		this->number[1] *= -1;
		this->minus = false;
	}
	if (temp.minus)
		temp.number[1] *= -1;

	//Находим НОК знаменателей
	int nok_znamen = nok(this->number[2], temp.number[2]);
	//Умножаем каждую из дробей (кроме целой части) на НОК/знаменатель
	this->number[1] *= nok_znamen / this->number[2];
	this->number[2] *= nok_znamen / this->number[2];
	temp.number[1] *= nok_znamen / temp.number[2];
	temp.number[2] *= nok_znamen / temp.number[2];
	//Складываем числители
	this->number[1] += temp.number[1];
	this->normalize();
}

void Fraction::f_minus(const Fraction & fract_2)
{
	Fraction temp;
	temp.copy(fract_2);
	temp.minus = !temp.minus;
	this->f_plus(temp);
}

void Fraction::f_mult(const Fraction & fract_2){
	Fraction temp;
	temp.copy(fract_2);

	//Прибавляем к числителю целую часть, умноженную на знаменатель
	this->number[1] += this->number[0] * this->number[2];
	this->number[0] = 0;
	temp.number[1] += temp.number[0] * temp.number[2];

	this->number[1] *= temp.number[1];
	this->number[2] *= temp.number[2];
	
	if (temp.minus)
		this->minus = !this->minus;

	this->normalize();
}

void Fraction::f_div(const Fraction & fract_2)
{
	Fraction temp;
	temp.copy(fract_2);
	temp.number[1] += temp.number[0] * temp.number[2];
	temp.number[0] = 0;

	//Меняем местами числитель и знаменатель
	int t = temp.number[2];
	temp.number[2] = temp.number[1];
	temp.number[1] = t;

	this->f_mult(temp);
}

//Реализация +=
Fraction& Fraction::operator+=(const Fraction &fract_2) {
	this->f_plus(fract_2);
	return *this;
}

//Реализация /=
Fraction& Fraction::operator/=(const Fraction &fract_2) {
	this->f_div(fract_2);
	return *this;
}

//Реализация -=
Fraction& Fraction::operator-=(const Fraction &fract_2) {
	this->f_minus(fract_2);
	return *this;
}

//Реализация *=
Fraction& Fraction::operator*=(const Fraction &fract_2) {
	this->f_mult(fract_2);
	return *this;
}

//Реализация сравнения
// Возвращает 0, если дроби равны, -1 - первая дробь меньше, 1 - первая дробь больше
int Fraction::compare(const Fraction & fract_2) const
{
	// Делаем копию первого операнда
	Fraction& temp = *(new Fraction(*this));
	// Вычитаем одно число из другого
	temp -= fract_2;
	// Если полученное число - отрицательное - первое число меньше
	if (temp.minus) {
		delete &temp;
		return -1;
	}
	// Если полученное число - положительное (числитель > 0 или целая часть > 0) - первое число больше
	if (temp.number[0] > 0 || temp.number[1] > 0) {
		delete& temp;
		return 1;
	}
	// Если полученное число 0 - числа равны
	delete& temp;
	return 0;
}

bool Fraction::load_from_str(std::string& str)
{
	char sub_str[3][20] = { NULL };

	int cur_position = 0; //Текущая позиция "курсора" в строке
	int cur_sub_str = 0; //Текущая суб-строка
	int cur_sub_str_pos = 0; //Текущая позиция "курсора" в суб-строке

	int found_space = false; //Дошли ли до пробела?
	int found_slash = false; //Дошли ли до слеша-деления?

	//Строка должна выглядеть так: 5 4/6
	//или -5 4/6

	//Пока не дошли до конца строки
	while (str[cur_position] != '\0') {
		//Если нашли минус
		if (str[cur_position] == '-') {
			minus = true;
			cur_position++;
			continue;
		}

		//Когда прошли пробел
		if (found_space) {
			//Когда прошли слеш-деление
			if (found_slash) {
				//Идём по знаменателю
				sub_str[cur_sub_str][cur_sub_str_pos] = str[cur_position];
				cur_position++;
				cur_sub_str_pos++;
			}
			else {
				//Идём по числителю
				if (str[cur_position] == '/') {
					found_slash = true;
					cur_position++;
					cur_sub_str = 2;
					cur_sub_str_pos = 0;
					continue;
				}
				sub_str[cur_sub_str][cur_sub_str_pos] = str[cur_position];
				cur_position++;
				cur_sub_str_pos++;
			}
		}
		//Когда ещё не дошли до пробела
		else {
			//Идём по целой части
			if (str[cur_position] == ' ') {
				found_space = true;
				cur_position++;
				cur_sub_str = 1;
				cur_sub_str_pos = 0;
				continue;
			}
			sub_str[cur_sub_str][cur_sub_str_pos] = str[cur_position];
			cur_position++;
			cur_sub_str_pos++;
		}
	}
	//Имеем три подстроки. Переводим каждую из них в int
	number = new int[3]();
	number[0] = atoi(sub_str[0]);
	number[1] = atoi(sub_str[1]);
	number[2] = atoi(sub_str[2]);
	return true;
}
