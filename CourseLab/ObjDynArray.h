#pragma once
#include "Object.h"


//динамический массив для хранения столбца
//сюда пихаем объекты только одного типа
class ObjDynArray
{
public:
	ObjDynArray(int size)
	{
		_arr = (Object**)calloc(size, sizeof(Object*));
		_size = size;
		_cursize = 0;
		//_name = nullptr;
	}
	ObjDynArray()
	{
		_arr = nullptr;
		_size = 0;
		_cursize = 0;
		_type = 0;
		_name = "";
	}
	ObjDynArray(ObjDynArray&& x)
	{
		_arr = x._arr;
		_size = x._size;
		_cursize = x._cursize;
		_name = x._name;
		_type = x._type;
		x._arr = nullptr;
		x._cursize = 0;

	}
	ObjDynArray(int size, string name, int type)
	{
		_arr = (Object**)calloc(size, sizeof(Object*));
		_size = size;
		_cursize = 0;
		_name = name;
		_type = type;
	}
	ObjDynArray(string name, int type)
	{
		_size = 0;
		_arr = nullptr;
		_cursize = 0;
		_name = name;
		_type = type;
	}
	ObjDynArray& operator=(const ObjDynArray& x)
	{
		_size = x._size;
		_cursize = x._cursize;
		_name = x._name;
		_type = x._type;
		_arr = (Object**)calloc(_size, sizeof(Object*));
		for (int i = 0; i < _cursize; i++)
		{
			_arr[i] = x._arr[i];
		}
		return(*this);
	}
	~ObjDynArray()
	{
		free(_arr);
	}

	bool add(Object* obj)
	{
		//если нет места, то расширим
		if (_cursize == _size)
			_arr = (Object**)realloc(_arr, ++_size * sizeof(obj));
		//вставочка)
		_arr[_cursize++] = obj;
		return true;
	}
	bool replace(int ind, Object* obj)
	{
		if (_arr[ind]->type() != obj->type())
			return false;
		_arr[ind] = obj;
		return true;
	}
	bool remove(int ind)
	{
		_arr[ind]->~Object();
		_arr[ind] = nullptr;

		//сдвигаем весь массив влево, чтобы не было нуля
		for (int i = ind; i < _size - 1; i++)
		{
			Object* temp = _arr[i];
			_arr[i] = _arr[i + 1];
			_arr[i + 1] = temp;
			temp = nullptr;
		}
		_arr = (Object**)realloc(_arr, --_size * sizeof(Object*));
		_cursize--;
		return true;
	}
	void print()
	{
		for (int i = 0; i < _size; i++)
			if (_arr[i] != nullptr)
				cout << _arr[i]->name() << " " << *_arr[i] << " ";
		cout << endl;
	}
	Object* operator[](int k)
	{
		try{
			if (k > _size)
				throw 121;
			return _arr[k];
		}
		catch(int i)
		{
			cout << endl << "ERROR " << i << " noncorrect index"<< endl;
		}
		//return _arr[k];
	}
	void sort()
	{
		for (int n = 1; n < _size; n++)
		{
			for (int i = n; i > 0; i--)
			{
				int r = _arr[i]->cmp(_arr[i - 1]);
				if (r == -1 || r == 0) // если первое меньше, то ничего не делаем
					continue;
				Object* temp = _arr[i];
				_arr[i] = _arr[i - 1];
				_arr[i - 1] = temp;
				//print();
			}
		}
	}
	Object* min()
	{
		Object* min = _arr[0];
		for (int i = 1; i < _size; i++)
		{
			if (_arr[i] != nullptr && min->cmp(_arr[i]) == 1)
				min = _arr[i];
		}
		return min;
	}
	Object* max()
	{
		Object* max = _arr[0];
		for (int i = 1; i < _size; i++)
		{
			if (_arr[i] != nullptr && max->cmp(_arr[i]) == -1)
				max = _arr[i];
		}
		return max;
	}

	void setSize(int x) { _cursize = x; }
	int size() { return _size; }
	int count() { return _cursize; }
	string name() { return _name; }
	int type() { return _type; }
private:
	//меняет два указателя местами
	void swap(void* x, void* y)
	{
		cout << x << " " << y << endl;
		void* temp = x;
		x = y;
		y = temp;
		//temp = nullptr;
		cout << x << " " << y << endl;
	}
private:
	Object** _arr;
	int _size;
	int _cursize;
	//хранимый класс
	int _type;
	string _name;

};
