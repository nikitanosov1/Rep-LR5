/*
На языке C++ необходимо реализовать программу, которая в соответствии с
заданным порядком работы, будет создавать заданное количество объектов,
объединять их в заданную структуру и сортировать по возрастанию.
1. Тип объектов - базовый класс Вашего варианта Лабораторной работы №4. (Polynom)
2. Вид структуры соответствует Вашему варианту Лабораторной работы №2. (двусвязный нециклический список)
3. Сортировка соответствует второй сортировке из Вашего варианта
Лабораторной работы №2 (пузырьком, вставками или выбором). (Selection sort, сортировка выбором)
4. Необходимо реализовать функцию, которая будет проверять факт
упорядоченности структуры. (  checkSort()  )
5. Необходимо реализовать функцию, которая будет создавать объект со
случайными параметрами, соответствующий Вашему варианту. (  createRand()  )

*/

#include <iostream>
#include <fstream>
#define MAX_SIZE_POLY 10;
#define MAX_SIZE_COEF 100;
using namespace std;


int power(int x, int y)  // Функция быстрого возведения в степень
{
	if (y == 0) return 1;
	if (y == 1) return x;
	if (y % 2 == 0) return power(x * x, y / 2);
	else return x * power(x, y - 1);
}

class Polynom
{
public:
	Polynom() :size(0), a(NULL) {};  // Конструктор по умолчанию
	Polynom(const Polynom& P) // Конструктор копирования
	{
		size = P.size;
		a = new int[size];
		for (size_t i = 0; i < (size_t)size; ++i) a[i] = P.a[i];
	};
	Polynom(size_t s, const int* m) // Конструктор инициализации объекта
	{
		size = s;
		a = new int[s];
		for (size_t i = 0; i < s; ++i) a[i] = m[i];
	};
	bool isRoot(int x)  // Метод проверки корня x
	{
		int temp = 0;
		for (size_t i = 0; i < size; ++i)
		{
			temp += a[i] * power(x, size - i - 1);
		}
		return (!temp);
	}
	const Polynom operator+(const Polynom& P)
	{
		Polynom temp;
		if (size > P.size)
		{
			temp.size = size;
			temp.a = new int[size];
			for (size_t i = 0; i < size; ++i)
				if (i > size - P.size - 1)
					temp.a[i] = a[i] + P.a[i - size + P.size];
				else
					temp.a[i] = a[i];
		}
		else
		{
			temp.size = P.size;
			temp.a = new int[P.size];
			for (size_t i = 0; i < P.size; ++i)
				if (i > P.size - size - 1)
					temp.a[i] = P.a[i] + a[i - P.size + size];
				else
					temp.a[i] = P.a[i];
		}
		return temp;
	}
	const Polynom operator*(const Polynom& P)
	{
		Polynom temp;
		temp.size = size + P.size - 1;
		temp.a = new int[size + P.size - 1];
		for (size_t i = 0; i < size + P.size - 1; ++i) temp.a[i] = 0;
		for (size_t i = 0; i < size; ++i)
			for (size_t j = 0; j < P.size; ++j)
				temp.a[i + j] += a[i] * P.a[j];
		return temp;
	}
	void operator*=(int x)  // Домножение на константу
	{
		for (size_t i = 0; i < size; ++i) a[i] *= x;
	}
	virtual void addCoef(int x)  // Добавление коэффициента
	{
		++size;
		int* b = new int[size];
		for (size_t i = 0; i < size - 1; ++i) b[i] = a[i];
		b[size - 1] = x;
		delete[] a;
		a = b;
	}
	double definiteIntegral(int a_, int b_)  // Подсчёт определённого интеграла от a_ до b_
	{
		double ans = 0;
		for (size_t i = 0; i < size; ++i)
		{
			if (a[i] != 0) ans += ((double)power(b_, size - i) - power(a_, size - i)) * a[i] / (size - i);
		}
		return ans;
	}
	double calcDerivative(int x)  // Вычисление производной в точке x
	{
		double ans = 0;
		for (size_t i = 0; i < size - 1; ++i)
		{
			if (a[i] != 0) ans += (double)a[i] * (size - i - 1) * power(x, size - i - 2);
		}
		return ans;
	}
	bool operator<(const Polynom& P)
	{
		return (size < P.size);
	}
	bool operator>(const Polynom& P)
	{
		return (size > P.size);
	}
	bool operator<=(const Polynom& P)
	{
		return (size <= P.size);
	}
	bool operator>=(const Polynom& P)
	{
		return (size >= P.size);
	}
	bool operator==(const Polynom& P)  // Проверка на равенство
	{
		if (size > P.size) return false;
		if (size > P.size) return false;
		for (size_t i = 0; i < size; ++i) if (a[i] != P.a[i]) return false;
		return true;
	}
	const Polynom& operator=(const Polynom& P)  // Оператор присваивания
	{
		if (this == &P) return *this;
		size = P.size;
		delete[] a;
		a = new int[size];
		for (size_t i = 0; i < size; ++i) a[i] = P.a[i];
		return P;
	}
	~Polynom()
	{
		delete[] a;
	}
private:
	size_t size;    // степень полинома  
	int* a;    // массив для хранения коэффициентов полинома   
	friend ifstream& operator>>(ifstream&, Polynom& P);
	friend ofstream& operator<<(ofstream&, const Polynom& P);
};

ifstream& operator>>(ifstream& fin, Polynom& P)
{
	fin >> P.size;
	delete[] P.a;
	P.a = new int[P.size];
	for (size_t i = 0; i < P.size; ++i) fin >> P.a[i];
	return fin;
}
ofstream& operator<<(ofstream& fout, const Polynom& P)
{
	for (size_t i = 0; i < P.size; ++i)
	{
		if (i == 0) fout << P.a[i] << "*x^" << P.size - i - 1;
		else
		{
			if (P.a[i] > -1) fout << "+";
			fout << P.a[i] << "*x^" << P.size - i - 1;
		}
	}
	return fout;
}

size_t inputN()  // Функция читает и возвращает число без знака из файла "input.txt"
{
    ifstream fin("input.txt");
    if (!fin.is_open()) throw("File does not exist");
    size_t N;
    fin >> N;
    fin.close();
    return N;
}

Polynom createRand()  // Функция возвращает случайно сгенерированный полином
{
	size_t size = rand() % MAX_SIZE_POLY;
	int* a = new int[size];
	for (size_t i = 0; i < size; ++i) a[i] = rand() % MAX_SIZE_COEF;
	Polynom P(size, a);
	delete[] a;
	return P;
}

int main()
{
    size_t N = inputN();



    return 0;
}