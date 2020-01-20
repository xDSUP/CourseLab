#pragma once
#include <string>
#include <iostream>
#include "Object.h"

//Класс "Дробь"
class Fraction : public Object {
public:
	//Методы для абстрактного класса
	//загруз обьек из и в текст строку
	std::ostream& PrintStream(std::ostream& oStream);
	std::istream& ScanStream(std::istream& iStream);
	//возврат уник идентифик(хеш с минимально возможным количеством коллизий)
	int type();
	//возврат указат на строку с именем класа
	std::string name();
	//сложение(обьединение)
	void add(Object*);
	//операторы приобразования
	operator double();
	//создание динам копии обьекта
	Fraction* copy();
	int cmp(Object* x);
public:
	//Методы-конструкторы

	//1. Пустой конструктор. Создаёт дробь, эквивалентную 0.
	Fraction();

	//2. Конструктор из параметра-строки
	Fraction(std::string& str);

	//3. Конструктор из параметра-массива
		//Массив должен содержать данные в виде {целая часть, числитель, знаменатель}
	Fraction(const int* mas);

	//4. Конструктор копирования
	Fraction(const Fraction&);

	// Конструктор перемещения
	Fraction(Fraction&&);

	// Вспомогательный конструктор из int-а. Дробь будет иметь только целую часть.
	Fraction(const int);

	//Деструктор
	~Fraction();

	//Меняет целую часть дроби
	void set_whole(const int i);

	//Меняет числитель дроби
	void set_numerator(const int i);

	//Меняет знаменатель дроби
	void set_denominator(const int i);

	//Меняет знак числа
	void set_pos_neg(const bool znak);

	//Метод вывода дроби на экран
	void print_fraction() const;

	//Метод ввода (редактирования) дроби с клавиатуры
	void edit_from_keyboard();

	//Метод вывода дроби в строку
	std::string fraction_to_string();

	//Возвращает указатель на number
	int* get_number();
	// Возвращает знак дроби
	bool get_minus();


	// Реализация = (копирующего присваивания)
	Fraction& operator=(const Fraction& right);
	// Реализация = (перемещающего присваивания)
	Fraction& operator=(Fraction&& right);

	// Реализация <<
	friend std::ostream& operator<<(std::ostream& out, Fraction& fract);
	// Реализация >>
	friend std::istream& operator>>(std::istream& in, Fraction& fract);

	// Логические опреаторы
	// Реализация ==
	bool operator==(const Fraction& right) const;

	// Реализация <
	bool operator<(const Fraction& right) const;

	// Реализация >
	bool operator>(const Fraction& right) const;

	// Реализация <=
	bool operator<=(const Fraction& right) const;

	// Реализация >=
	bool operator>=(const Fraction& right) const;


	//Операции математических действий (без равно)
	// Реализация +
	Fraction& operator+(const Fraction& fract_2) const;
	// Реализация -
	Fraction& operator-(const Fraction& fract_2) const;
	// Реализация *
	Fraction& operator*(const Fraction& fract_2) const;
	// Реализация /
	Fraction& operator/(const Fraction& fract_2) const;


	//Операции математических действий (с равно)
	//Реализация +=
	Fraction& operator+=(const Fraction& fract_2);

	//Реализация /=
	Fraction& operator/=(const Fraction& fract_2);

	//Реализация -=
	Fraction& operator-=(const Fraction& fract_2);

	//Реализация *=
	Fraction& operator*=(const Fraction& fract_2);

private:
	//Метод копирования объекта
	//src - дробь-источник
	void copy(const Fraction& src);

	//Метод перемещения объектра
	//src - дробь-источник
	void move(Fraction&& scr);

	//Нормализирует дробь: делает её правильной, делая числитель меньше знаменателя и выделяя целую часть,
	//если необходимо.
	void normalize();

	//Вспомогательная функция: делает все элементы дроби неотрицательными числами
	//и меняет флаг minus, если необходимо.
	void abs_frac();

	//Вспомогательная функция: возвращает длину числа x
	int lenof(int x) const;

	//Вспомогательная функция: возвращает НОД чисел a и b
	int nod(int a, int b) const;

	//Вспомогательная функция: возвращает НОК чисел a и b
	int nok(int n1, int n2) const;

	//Складывает текущую дробь с fract_2 и возвращает результат в текущую дробь
	void f_plus(const Fraction& fract_2);

	//Складывает текущую дробь с fract_2 и возвращает результат в текущую дробь
	void f_minus(const Fraction& fract_2);

	//Перемножает текущую дробь с fract_2 и возвращает результат в текущую дробь
	void f_mult(const Fraction& fract_2);

	//Делит текущую дробь на fract_2 и возвращает результат в текущую дробь
	void f_div(const Fraction& fract_2);

	//Реализация сравнения
	// Возвращает 0, если дроби равны, -1 - первая дробь меньше, 1 - первая дробь больше
	int compare(const Fraction& fract_2) const;

	// Функция загрузки дроби из строки
	bool load_from_str(std::string& str);

private:
	//Поля объекта
	bool minus; //Является ли число отрицательным
	int* number; //Динамический массив из 3-х чисел

	//int whole_num; //Целая часть
	//int numerator; //Числитель
	//int denominator; //Знаменатель
};