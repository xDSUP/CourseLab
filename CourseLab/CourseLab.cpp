
#include <iostream>
#include <string>
#include <fstream>
#include "real.h"
#include "Fraction.h"
#include "Table.h"
#include <random>
#include <ctime>

#define KEY_EXIT 0
#define KEY_SAVETABLE 1
#define KEY_LOADTABLE 2
#define KEY_TABLEADDCOL 3
#define KEY_TABLEADDLINE 4
#define KEY_TABLEEDITELEM 5
#define KEY_TABLEREMOVELINE 6
#define KEY_TABLEREMOVECOL 7
#define KEY_TABLESORTCOL 8
#define KEY_TABLEPRINT 9
//
//#define FRACTION 44468887
//#define REAL 1743979489
//


using namespace std;

class Interface
{
public:
	Interface()
	{
		cout << "SUPER MEGA TABLE\n";
		menuTable = "\nChoise operation \n";
		menuTable += "1 - save table\n";
		menuTable += "2 - load table\n";
		menuTable += "3 - add col in table\n";
		menuTable += "4 - add line in table\n";
		menuTable += "5 - edit object\n";
		menuTable += "6 - remove line\n";
		menuTable += "7 - remove row\n";
		menuTable += "8 - sort table by col\n";
		menuTable += "9 - print table\n";
		menuTable += "0 - exit\n";
		menuTable += "Press key: ";
		Table table;
	}
	int getUserChoise()
	{
		int choise = -1;
		cout << menuTable;
		cin >> choise;
		/*switch (choise)
		{
		case KEY_EXIT:
			return 0;
		default:
			break;
		}*/
		return choise;
	}

	void addColInTable()
	{
		cout << "suported classes: Real[0], Fraction[1]" << endl;
		int choise = -1;
		do
		{
			cout << "Input classname index: ";
			cin >> choise;
		} while (choise < 0 || choise > 1);
		switch (choise)
		{
		case 0:
			table.addCol("class Real", REAL); break;
		case 1:
			table.addCol("class Fraction", FRACTION); break;
		default:
			return;
		}
		table[table.getCol() - 1].setSize(table.getLine());
	}
	
	void addLineInTable()
	{
		table.printHeader();//вывели заголовок
		Object* temp = nullptr;
		ObjDynArray result(table.getCol());
		for (int col = 0; col < table.getCol(); col++)
		{
			cout << "input object's: ";
			temp = table.getObject(table[col].type());
			if (!temp) // значит пздц
				return;
			cin >> *temp;
			cout << endl;
			result.add(temp);
			temp = nullptr;
		}
		table.addLine(result);
	}

	void sortTable()
	{
		int ind = -1;
		cout << "Input index col for sorting: ";
		cin >> ind;
		if (ind >= 0 && ind < table.getCol())
			table.sort(ind);
		else
			cout << "index is not correct";
	}

	void editElemInTable()
	{
		int i = -1, j = -1;
		cout << "Input elem col index: ";
		cin >> j;
		cout << "Input elem line index: ";
		cin >> i;
		if (1)
		{
			Object* temp = table.getObject(table[j].type());
			cout << "Input new object: ";
			cin >> *temp;
			//if (table[i][j] == 0)
				//table[i][j] = temp;
			table.editElem(j, i, temp);
		}
		else
			cout << "Indexs is not correct";
	}

	void printTable() { table.print(); }

	void loadTable()
	{
		string filename;
		cout << "Input filename: ";
		cin >> filename;
		int res;
		res = table.load(filename);
		if (res == -1)
			cout << "Unknown type" << endl;
		else if (res == 0)
			cout << "File not find or not open" << endl;
		else
			cout << "SUCSESS" << endl;
	}

	void saveTable()
	{
		string filename;
		cout << "Input filename: ";
		cin >> filename;
		table.save(filename);
	}

	void removeLineInTable()
	{
		int ind;
		cout << "Input line index: ";
		cin >> ind;
		if (1)
		{
			table.deleteLine(ind);
		}
		else
			cout << "Indexs is not correct";
	}

	void removeColInTable()
	{
		int ind; 
		cout << "Input col index: ";
		cin >> ind;
		if (1)
		{
			table.deleteCol(ind);
		}
		else
			cout << "Indexs is not correct";
	}
	// осн шаг интерефейса
	bool step()
	{
		int user_choise = getUserChoise();
		switch (user_choise)
		{
		case KEY_EXIT: return false;
		case KEY_LOADTABLE: loadTable(); return true;
		case KEY_TABLEADDCOL: addColInTable(); return true;
		case KEY_TABLEADDLINE: addLineInTable(); return true;
		case KEY_TABLEEDITELEM: editElemInTable(); return true;
		case KEY_TABLEREMOVECOL: removeColInTable(); return true;
		case KEY_TABLEREMOVELINE: removeLineInTable(); return true;
		case KEY_TABLESORTCOL: sortTable(); return true;
		case KEY_TABLEPRINT: printTable(); return true;
		case KEY_SAVETABLE: saveTable(); return true;
		
		default:
			cout << "not correct" << endl;
			break;
		}
	}

private:
	// создает новый обьект нужного типа и возвращает на него указатель
	
	string menuMain;
	string menuTable;
	Table table;
};

int main() {
	
	Interface it;
	//работа интерфейса
	//while (it.step());
	// ручной тест
	ObjDynArray x(3);
	Real z("1");
	x.add(&z);
	x.add(&z);
	x.add(&z);
	x.print();
	ObjDynArray x1(3);
	Fraction zyey(1);
	x1.add(&zyey);
	x1.add(&zyey);
	x1.add(&zyey);
	x1.print();

	Table table;
	cout << x[0]->type() << " ";
	cout << x[1]->type();
	
	for (int i = 0; i < 3; i++)
		table.addCol(x[i]->name(), x[i]->type());
	//table.print();
	for (int i = 1; i < 1000; i++)
	{
		table.addLine(x);
		x[0]->add((Object*)new Real("1"));
		x.replace(1, new Real(std::to_string(rand() % 100 + 1900)));
		x.replace(2, new Real(std::to_string(rand() % 100000)));
		x.replace(0, new Real(std::to_string(i)));
	//	x.print();
	}
	
	table.print();
	//table.addLine(x1);
	//table.print();
	
	auto test = table[0][0]->copy();
	cout << *test;
	//table.editElem(0, 0, new Real("-2229"));
	//table.editElem(0, 1, new Real("0.1"));
	//table.editElem(2, 0, new Real("99999"));
	//table.sort(0); // доделать
	//table.print();
	table.save("test.txt");
	//table.deleteLine(998);
	//table.print();
	time_t downdload = 0;
	time_t sorted = 0;
	Table text;
	for (int i = 0; i < 10; i++)
	{
		time_t st = clock();
		text.load("test.txt");
		st = clock() - st;
		downdload += st;
		//text.print();
		cout << "DOWNLOAD 999 OF TEXTFILE AT " << st << endl;

		st = clock();
		text.sort(2);
		st = clock() - st;
		sorted += st;
		//text.print();
		cout << "SORT 999 ELEM AT " << st << endl;
	}	
	cout << "Average time download: " << downdload/10 << endl;
	cout << "Average time sort: " << sorted/10 << endl;
	text.save("sorted");
	system("pause");
}