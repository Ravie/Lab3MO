// Lab3MO.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
using namespace std;

struct IterTable
{
	double f_x;
	double a;
	double b;
	double length;
	double ratio;
	// Fibonacci, golden section
	double x1;
	double x2;
	double f_x1;
	double f_x2;
	// Newton
	double df_x;
	double approx;
};
double func(double);
void passive_search(double, double, double);
void dichotomy(double, double, double);
int Fib(int);
void Fibonacci(double, double, double, double);
void golden_section(double, double, double);
void Newton(double, double, double, double);
double deriv(double);

int main()
{
	setlocale(LC_ALL, "Russian");
	double x0, l, eps, a, b;
	cout << "Введите погрешность: ";
	cin >> eps;
	cout << "Введите начальную точку: ";
	cin >> a;
	cout << "Введите конечную точку: ";
	cin >> b;
	//passive_search(eps, a, b);
	//dichotomy(eps, a, b);

	//cout << "Введите допустимую конечную длину интервала неопределенности: ";
	//cin >> l;
	//Fibonacci(l, eps, a, b);

	//golden_section(eps, a, b);
	cout << "Начальное приближение: ";
	cin >> x0;
	Newton(x0, eps, a, b);

    return 0;
}

double func(double x)
{
	return pow(x - 2, 2);
}

void passive_search(double eps, double a, double b)
{
	int N = int((b - a) / eps + 1);
	//cout << "Кол-во измерений: " << N << endl;
	IterTable *passive_search = new IterTable[N];
	passive_search[0].f_x = func(a);
	passive_search[0].a = a;
	passive_search[0].b = b;
	passive_search[0].length = b - a;
	passive_search[0].ratio = 0;
	for (int i = 1; i < N; i++)
	{
		passive_search[i].f_x = func(eps * i + a);
		passive_search[i].a = eps * i + a;
		passive_search[i].b = b;
		passive_search[i].length = passive_search[i].b - passive_search[i].a;
		passive_search[i].ratio = passive_search[i - 1].length / passive_search[i].length;
		cout << passive_search[i].f_x << " " << passive_search[i].a << " " << passive_search[i].b << " " << passive_search[i].length << " " << passive_search[i].ratio << endl;
	}
	double min = numeric_limits<double>::max();
	for (int i = 0; i < N; i++)
	{
		if (passive_search[i].f_x < min)
			min = passive_search[i].f_x;
	}
	cout << "Min: " << min << endl;
	delete[] passive_search;
}

void dichotomy(double eps, double a, double b)
{
	double f1, f2;
	int N = int(log2((b - a) / eps) + 1);
	cout << "Кол-во измерений: " << N << endl;
	IterTable *dichotomy = new IterTable[N];
	f1 = func((a + b) / 2 - eps);
	f2 = func((a + b) / 2 + eps);
	if (f1 < f2)
	{
		dichotomy[0].f_x = f1;
		dichotomy[0].a = a;
		dichotomy[0].b = (a + b) / 2;
	}
	else
	{
		dichotomy[0].f_x = f2;
		dichotomy[0].a = (a + b) / 2;
		dichotomy[0].b = b;
	}
	dichotomy[0].length = (b - a) / 2;
	dichotomy[0].ratio = 0;
	for (int i = 1; i < N; i++)
	{
		f1 = func((dichotomy[i - 1].a + dichotomy[i - 1].b) / 2 - eps);
		f2 = func((dichotomy[i - 1].a + dichotomy[i - 1].b) / 2 + eps);
		if (f1 < f2)
		{
			dichotomy[i].f_x = f1;
			dichotomy[i].a = dichotomy[i - 1].a;
			dichotomy[i].b = (dichotomy[i - 1].a + dichotomy[i - 1].b) / 2;
		}
		else
		{
			dichotomy[i].f_x = f2;
			dichotomy[i].a = (dichotomy[i - 1].a + dichotomy[i - 1].b) / 2;
			dichotomy[i].b = dichotomy[i - 1].b;
		}
		dichotomy[i].length = (dichotomy[i].b - dichotomy[i].a) / 2;
		dichotomy[i].ratio = dichotomy[i - 1].length / dichotomy[i].length;
		cout << dichotomy[i].f_x << " " << dichotomy[i].a << " " << dichotomy[i].b << " " << dichotomy[i].length << " " << dichotomy[i].ratio << endl;
	}
	cout << "Min: " << dichotomy[N - 1].f_x << endl;
	delete[] dichotomy;
}

int Fib(int i)
{
	int Fi;
	if (i == 0 || i == 1)
	{
		Fi = 1;
	}
	else
	{
		Fi = Fib(i - 2) + Fib(i - 1);
	}
	return Fi;
}

void Fibonacci(double length, double eps, double a, double b)
{
	int N;
	vector<int> Fib_seq;
	for (int i = 0; Fib(i) < (b - a) / length; i++)
	{
		Fib_seq.push_back(Fib(i));
		N = i + 1;
	}
	Fib_seq.push_back(Fib(N));
	IterTable *Fibonacci = new IterTable[N];
	Fibonacci[0].a = a;
	Fibonacci[0].b = b;
	Fibonacci[0].x1 = a + ((double)Fib_seq[N - 2] / Fib_seq[N]*(b - a));
	Fibonacci[0].x2 = a + ((double)Fib_seq[N - 1] / Fib_seq[N]*(b - a));
	Fibonacci[0].f_x1 = func(Fibonacci[0].x1);
	Fibonacci[0].f_x2 = func(Fibonacci[0].x2);
	for (int k = 0; k < N - 2; k++)
	{
		if (Fibonacci[k].f_x1 > Fibonacci[k].f_x2)
		{
			Fibonacci[k + 1].a = Fibonacci[k].x1;
			Fibonacci[k + 1].b = Fibonacci[k].b;
			Fibonacci[k + 1].x1 = Fibonacci[k].x2;
			Fibonacci[k + 1].f_x1 = func(Fibonacci[k].x2); // этого не было в алгоритме
			Fibonacci[k + 1].x2 = Fibonacci[k + 1].a + ((double)Fib_seq[N - k - 2] / Fib_seq[N - k - 1]*(Fibonacci[k + 1].b - Fibonacci[k + 1].a));
			Fibonacci[k + 1].f_x2 = func(Fibonacci[k + 1].x2);
		}
		else
		{
			Fibonacci[k + 1].a = Fibonacci[k].a;
			Fibonacci[k + 1].b = Fibonacci[k].x2;
			Fibonacci[k + 1].x2 = Fibonacci[k].x1;
			Fibonacci[k + 1].f_x2 = func(Fibonacci[k].x1); // этого не было в алгоритме
			Fibonacci[k + 1].x1 = Fibonacci[k + 1].a + ((double)Fib_seq[N - k - 3] / Fib_seq[N - k - 1]*(Fibonacci[k + 1].b - Fibonacci[k + 1].a));
			Fibonacci[k + 1].f_x1 = func(Fibonacci[k + 1].x1);
		}
	}
	Fibonacci[N - 1].x1 = Fibonacci[N - 2].x1;
	Fibonacci[N - 1].x2 = Fibonacci[N - 1].x1 + eps;
	if (Fibonacci[N - 1].f_x1 == Fibonacci[N - 1].f_x2)
	{
		Fibonacci[N - 1].a = Fibonacci[N - 1].x1;
		Fibonacci[N - 1].b = Fibonacci[N - 2].b;

	}
	else if (Fibonacci[N - 1].f_x1 < Fibonacci[N - 1].f_x2)
	{
		Fibonacci[N - 1].a = Fibonacci[N - 2].a;
		Fibonacci[N - 1].b = Fibonacci[N - 1].x2;
	}
	cout << "Оптимальное решение содержится в интервале [" << Fibonacci[N - 1].a << ", " << Fibonacci[N - 1].b << "]" << endl;
	delete[] Fibonacci;
}

void golden_section(double eps, double a, double b)
{
	double fert = (1 + sqrt(5)) / 2;
	//cout << fert;
	vector<IterTable> gold_sec_array;
	IterTable gold_sec;
	gold_sec.a = a;
	gold_sec.b = b;
	while (abs(gold_sec.b - gold_sec.a) > eps)
	{
		gold_sec.x1 = gold_sec.b - ((gold_sec.b - gold_sec.a) / fert);
		gold_sec.x2 = gold_sec.a + ((gold_sec.b - gold_sec.a) / fert);
		gold_sec.f_x1 = func(gold_sec.x1);
		gold_sec.f_x2 = func(gold_sec.x2);
		gold_sec_array.push_back(gold_sec);
		if (gold_sec.f_x1 >= gold_sec.f_x2)
		{
			gold_sec.a = gold_sec.x1;
		}
		else
		{
			gold_sec.b = gold_sec.x2;
		}
	}
	cout << "Оптимальное решение содержится в интервале [" << gold_sec.a << ", " << gold_sec.b << "]" << endl;
}

void Newton(double st_aprx, double eps, double a, double b)
{
	vector<IterTable> Newton_array;
	IterTable Newton;
	Newton.approx = st_aprx;
	Newton.f_x = func(st_aprx);
	Newton.df_x = deriv(st_aprx);
	while (abs(Newton.df_x) > eps)
	{
		Newton_array.push_back(Newton);
		Newton.approx -= Newton.f_x / Newton.df_x;
		Newton.f_x = func(Newton.approx);
		Newton.df_x = deriv(Newton.approx);
		cout << Newton.approx << " ";
	}
}

double deriv(double x)
{
	return 2 * x - 4;
}
