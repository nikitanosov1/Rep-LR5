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
    size_t getSize() { return size; }
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
    friend istream& operator>>(istream&, Polynom& P);
    friend ostream& operator<<(ostream&, const Polynom& P);
};

istream& operator>>(istream& fin, Polynom& P)
{
    fin >> P.size;
    delete[] P.a;
    P.a = new int[P.size];
    for (size_t i = 0; i < P.size; ++i) fin >> P.a[i];
    return fin;
}
ifstream& operator>>(ifstream& fin, Polynom& P)
{
	fin >> P.size;
	delete[] P.a;
	P.a = new int[P.size];
	for (size_t i = 0; i < P.size; ++i) fin >> P.a[i];
	return fin;
}
ostream& operator<<(ostream& fout, const Polynom& P)
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


struct elem   // Структура каждого элемента списка
{
    Polynom value;  // Храниемое значение
    elem* prev = 0;    // Ссылка на следующий элемент списка
    elem* next = 0;    // Ссылка на предыдущий элемент списка
};

class List {
public:
    List() : head(NULL), tail(NULL), size(0) {};  // Конструктор
    elem* getHead() { return head; }  // Геттеры
    elem* getTail() { return tail; }
    size_t getSize() { return size; }
    void add(const Polynom& x)   // Метод добавления элемента x в конец списка
    {
        elem* temp = new elem;  // Выделяем память для нового элемента
        temp->next = NULL;  // Инициализируем его поля
        temp->value = x;
        ++size;   // Увеличиваем размер списка на 1
        if (head == NULL)   // Если список пустой
        {
            temp->prev = NULL;
            head = tail = temp;
        }
        else  // Если в списке уже что-то хранится
        {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
    }
    void showListInConsole()  // Метод вывода списка в консоль
    {
        elem* temp = head;
        while (temp)
        {
            cout << temp->value << " ";
            temp = temp->next;
        }
    }
    void generateList(size_t wantSize)  // Метод генерации списка заданной длины (Значения от 0 до 1000)
    {
        this->~List();  // Уничтожаем текущий список
        size = 0;
        while (wantSize--)
        {
            add(createRand());  // Заполняем новый
        }
    }
    elem* operator[](size_t n) // Поиск n-ого элемента в списке (отсчёт с нуля). Возвращает указатель на этот элемент
    {
        elem* temp = head;
        while (n--) temp = temp->next;
        return temp;
    }
    void swap(elem* a, elem* b)
    {
        // Попробуй создать 4 временных указателя для а, например, анализируешь граничные, 
        if (a == b) return;
        if (a->prev == NULL && b->next == NULL)  // a указывает на первый элемент, b - на последний  //CORRECT
        {
            if (a->next == b)
            {
                b->next = a;
                a->prev = b;
            }
            else
            {
                a->next->prev = b;
                b->prev->next = a;
                b->next = a->next;
                a->prev = b->prev;
            }
            a->next = NULL;
            b->prev = NULL;
            head = b;
            tail = a;
        }
        else if (a->prev == NULL && b->next != NULL) // a указывает на первый элемент, b - не на крайний  // CORRECT
        {
            if (a->next == b)
            {
                b->next->prev = a;
                a->next = b->next;
                b->next = a;
                a->prev = b;
                b->prev = NULL;
                head = b;
            }
            else
            {
                a->next->prev = b;//
                b->prev->next = a;//
                b->next->prev = a;//
                pSwap(b->next, a->next);
                a->prev = b->prev;
                b->prev = NULL;
                head = b;
            }
        }
        else if (a->next == NULL && b->prev == NULL) // a указывает на последний элемент, b - на первый  // CORRECT
        {
            if (b->next == a)
            {
                a->next = b;
                b->prev = a;
            }
            else
            {
                b->next->prev = a;//
                a->prev->next = b;//
                a->next = b->next;
                b->prev = a->prev;
            }
            b->next = NULL;
            a->prev = NULL;
            head = a;
            tail = b;
        }
        else if (a->next == NULL && b->prev != NULL) // a указывает на последний элемент, b - не на крайний //CORRECT
        {
            if (b->next == a)
            {
                b->prev->next = a;
                a->prev = b->prev;
                a->next = b;
                b->prev = a;
                b->next = NULL;
                tail = b;
            }
            else
            {
                b->next->prev = a;
                b->prev->next = a;
                a->prev->next = b;
                a->next = b->next;
                pSwap(b->prev, a->prev);
                b->next = NULL;
                tail = b;
            }
        }
        else if (b->next == NULL)  // a указывает не на крайний, b - на последний  // CORRECT
        {
            if (a->next == b)
            {
                a->prev->next = b;
                b->prev = a->prev;
                b->next = a;
                a->prev = b;
                a->next = NULL;
                tail = a;
            }
            else
            {
                a->next->prev = b;
                a->prev->next = b;
                b->prev->next = a;
                b->next = a->next;
                pSwap(a->prev, b->prev);
                a->next = NULL;
                tail = a;
            }
        }
        else if (b->prev == NULL) // a указывает не на крайний, b - на первый  // CORRECT
        {
            if (b->next == a)
            {
                a->next->prev = b;
                b->next = a->next;
                a->next = b;
                b->prev = a;
                a->prev = NULL;
                head = a;
            }
            else
            {
                b->next->prev = a;
                a->prev->next = b;
                a->next->prev = b;
                pSwap(a->next, b->next);
                b->prev = a->prev;
                a->prev = NULL;
                head = a;
            }
        }
        else  // a, b указывают не на крайние элементы
        {
            if (a->next == b)  // CORRECT
            {
                b->next->prev = a;
                a->prev->next = b;
                b->prev = a->prev;
                a->next = b->next;
                b->next = a;
                a->prev = b;
            }
            else if (b->next == a)  // CORRECT
            {
                a->next->prev = b;
                b->prev->next = a;
                a->prev = b->prev;
                b->next = a->next;
                a->next = b;
                b->prev = a;
            }
            else
            {  // CORRECT
                pSwap(a->prev->next, b->prev->next); // left up
                pSwap(a->next->prev, b->next->prev); // right down
                pSwap(a->next, b->next);  // right up
                pSwap(a->prev, b->prev);  // left down
            }
        }
    }
    ~List()  // Деструктор
    {
        while (head)   // Пока ещё есть элементы в списке
        {
            elem* temp = head->next;
            delete head;    // Удаляем первый элемент, корректируя указатели
            head = temp;
        }
    }
private:
    elem* head;  // Указатель на первый элемент списка 
    elem* tail;  // Указатель на последний элемент списка
    size_t size;  // Размер списка
    void pSwap(elem*& a, elem*& b)  // Процедура обмена указателей
    {
        elem* temp = a;
        a = b;
        b = temp;
    }
};


void SelectionSort(List& list)
{
    elem* temp1 = list.getHead();
    while (temp1) {
        size_t min = temp1->value.getSize();
        elem* pointerMinElem = temp1;
        elem* temp2 = temp1;
        while (temp2)
        {
            if (temp2->value.getSize() < min)
            {
                min = temp2->value.getSize();
                pointerMinElem = temp2;
            }
            temp2 = temp2->next;
        }
        list.swap(temp1, pointerMinElem);
        elem* z = temp1;   // swap указателей
        temp1 = pointerMinElem;
        pointerMinElem = z;
        temp1 = temp1->next;
    }
}

void checkSort(List& L)
{
    if (L.getSize() < 2) { cout << "Sorted" << endl; return; }
    bool flag = true;
    elem* curr = L[0];
    elem* next = L[1];
    for (size_t i = 0; i < L.getSize() - 1; ++i)
    {
        if (curr->value.getSize() >= next->value.getSize()) {flag = false; break;}
        curr = curr->next;
        next = next->next;
    }
    (flag) ? cout << "Sorted" << endl : cout << "Not sorted" << endl;
}

int main()
{
    size_t N = inputN();
    List list;
    list.generateList(N);
    SelectionSort(list);
    checkSort(list);
    return 0;
}