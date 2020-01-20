#pragma once
#include "ObjDynArray.h"

#define FRACTION 44468887
#define REAL 1743979489


class Table
{
public:
	Table()
	{
		_countCol = 0;
		_countLine = 0;
		_header = nullptr;
	}
	Table(int m, int n) // c������� ���� � ��� ���-�� ����� � ��������
	{
		_countCol = m;
		_countLine = n;
		_header = (ObjDynArray*)calloc(_countLine, sizeof(ObjDynArray));
	}
	~Table()
	{
		delete _header;
	}
	ObjDynArray& operator[](int k)
	{
		return _header[k];
	}
	bool addCol(string name, int type)
	{
		if (_header == nullptr) // ��������� �� ������
		{
			_header = new ObjDynArray[1]; // ��� ��� 1 ����
			_header[0] = ObjDynArray(name, type);
			_countCol++;
			return true;
		}
		//����������� ������ �� ��� 1 �������
		this->resizeCol(++_countCol);
		//ObjDynArray temp()
		_header[_countCol - 1] = ObjDynArray(_countLine, name, type); //��������
		for (int i = 0; i < _countLine; i++)
		{
			//�������� ��������
			if(type == REAL)
				_header[_countCol - 1].add(new Real("0"));
			else if (type == FRACTION)
				_header[_countCol - 1].add(new Fraction(0));
		}
	}
	//��������� ������ line, � ���� �������
	bool addLine(ObjDynArray& line)
	{
		if (_header == nullptr || line.count() != _countCol)
			return false;
		int j = 0;
		_countLine += 1;
		// ������ ������� ����� ������ ������� � ����� ���������� �������
		for (int i = 0; i < line.count(); i++)
			//���� ������ �������
			for (j; j < _countCol; j++)
				if (line[i]->name() == _header[j].name() && line[i]->type() == _header[j].type())
				{
					_header[j].add(line[i]); // ������ ���������� ������������� ������ ���������� � ����� ������� �������
					j++;
					break;
				}
		return true;
	}
	//������ ��-�� �� ���
	bool editElem(int i, int j, Object* obj)
	{
		//������ �������� ��� ���� ��������
		//if (_header[i].type() != obj->type()) //���� ������������ ��� ������ �� ���� ����
			//return false;
		if (_header[i].replace(j, obj)) // ���� ������� ��������
			return true;
		return false;
	}
	// ������� ������ �� �������
	bool deleteLine(int ind)
	{
		for (int i = 0; i < _countCol; i++)
			_header[i].remove(ind);
		_countLine--;
		return true;
	}

	bool deleteCol(int ind)
	{
		_header[ind].~ObjDynArray();
		for (int i = ind; i < _countCol - 1; i++)
			_header[i] = _header[i+1];
		if (resizeCol(_countCol - 1))
		{
			if (_countCol == 1)
			{
				_countLine = 0;
				_header = nullptr;
			}
			_countCol--;
			return true;
		}
		return false;
	}
	// ��������� �� ������� ���������
	void sort(int ind)
	{
		for (int n = 1; n < _countLine; n++)
		{
			for (int i = n; i > 0; i--)
			{
				int r = _header[ind][i]->cmp(_header[ind][i-1]);
				if (r == -1 || r == 0) // ���� ������ ������ ��� �����, �� ������ �� ������
					continue;
				for (int c = 0; c < _countCol; c++)
				{//���� ������, �� ������������ ����� ��� ������
					auto temp = _header[c][i];
					//������ �� �������� ����� ����������.
					this->editElem(c, i, _header[c][i - 1]);
					this->editElem(c, i - 1, temp);
				}
			}
		}
	}

	void print()
	{
		cout << "Index" << " ";
		printHeader();
		for (int i = 0; i < _countLine; i++)
			this->printLine(i);
	}

	void printHeader()
	{
		for (int i = 0; i < _countCol; i++)
			cout << _header[i].name() << " ";
		cout << endl;
	}

	void save(string filename)
	{
		fstream f(filename.c_str(), ios_base::out);
		if (!f.is_open())
			return;
		f << _countCol << " " << _countLine << endl;
		for (int i = 0; i < _countCol; i++)
		{
			f << _header[i].name() << " " << _header[i].type() << " ";
		}
		f << endl;
		for (int i = 0; i < _countLine; i++)
		{
			for (int j = 0; j < _countCol; j++)
			{
				f << *_header[j][i] << " ";
			}
			f << endl;
		}
	}

	int load(string filename)
	{
		fstream f(filename.c_str(), ios_base::in);
		if (!f.is_open())
			return 0;
		this->~Table();
		_countCol = 0;
		_countLine = 0;
		int col, line;
		// ��������� ������ ����
		f >> col >> line;

		// ��������� ��������� �������
		int* types = new int[col];
		for (int i = 0; i < col; i++)
		{
			string name;
			int type;
			f >> name >> name >> type;
			this->addCol("class " + name, type);
			if (name == "Real")
				types[i] = REAL;
			else if (name == "Fraction")
				types[i] = FRACTION;
			else {
				cout << " � ��� ����, ��� ��������� ��� �����������";
				types[i] = -1;
			}
		}
		// ��������� ��������� �������
		for (int i = 0; i < line; i++)
		{
			string name;
			Object* temp = nullptr;
			ObjDynArray result(col);
			for (int j = 0; j < _countCol; j++)
			{
				f >> name;
				//����-�� ������ �������
				temp = getObject(types[j], name);
				if (!temp)
					return -1;
				result.add(temp);
			}
			this->addLine(result);
		}
		delete[] types;
		return 1;
	}

	Object* getObject(int type)
	{
		Object* temp;
		switch (type)
		{
		case REAL:
			temp = new Real(); break;
		case FRACTION:
			temp = new Fraction(); break;
		default:
			cout << "Error" << endl;
			return nullptr;
		}
		return temp;
	}
	Object* getObject(int type, string init)
	{
		Object* temp;
		switch (type)
		{
		case REAL:
			temp = new Real(init); break;
		case FRACTION:
			temp = new Fraction(init); break;
		default:
			cout << "Error" << endl;
			return nullptr;
		}
		return temp;
	}

	void printLine(int k)
	{
		cout << k << " ";
		for (int j = 0; j < _countCol; j++)
		{
			if (_header[j][k] != 0)
				cout << *_header[j][k] << " ";
			else
				cout << "0" << " ";
		}
		cout << endl;
	}

	int getCol() { return _countCol; }

	int getLine() { return _countLine; }

private:
	//������ ���-�� ��������
	bool resizeCol(int newSize)
	{
		ObjDynArray* temp = _header;
		_header = new ObjDynArray[newSize];
		for (int i = 0; i < _countCol - 1; i++)
			_header[i] = temp[i];
		//delete temp;
		return true;
	}

	ObjDynArray* _header;
	int _countCol;
	int _countLine;
};
