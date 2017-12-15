// Lab3MO.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
using namespace std;

struct IterTable
{
	long double f_x;
	long double a;
	long double b;
	long double length;
	long double ratio;
	// Fibonacci, golden section
	long double x1;
	long double x2;
	long double f_x1;
	long double f_x2;
	// Newton
	long double df_x;
	long double approx;
};
long double func(long double);
void passive_search(long double, long double, long double);
void dichotomy(long double, long double, long double);
int Fib(int);
void Fibonacci(long double, long double, long double, long double);
void golden_section(long double, long double, long double);
void Newton(long double, long double, long double, long double);
long double deriv(long double);

int main()
{
	setlocale(LC_ALL, "Russian");
	long double x0, l, eps, a, b;
	cout << "Введите погрешность: ";
	cin >> eps;
	cout << "Введите начальную точку: ";
	cin >> a;
	cout << "Введите конечную точку: ";
	cin >> b;
	
	passive_search(eps, a, b);

	dichotomy(eps, a, b);
	
	cout << "Введите допустимую конечную длину интервала неопределенности: ";
	cin >> l;
	Fibonacci(l, eps, a, b);

	golden_section(eps, a, b);
	
	cout << "Начальное приближение: ";
	cin >> x0;
	Newton(x0, eps, a, b);

    return 0;
}

long double func(long double x)
{
	return pow(x - 2, 2);
}

void passive_search(long double eps, long double a, long double b)
{
	int N = int((b - a) / eps + 1);
	cout << "Кол-во измерений: " << N << endl;
	IterTable *passive_search = new IterTable[N];
	passive_search[0].f_x = func(a);
	passive_search[0].a = a;
	passive_search[0].b = b;
	passive_search[0].length = b - a;
	passive_search[0].ratio = 0;	
	//cout << "0" << "\t" << passive_search[0].a << "\t" << passive_search[0].b << "\t" << passive_search[0].length << "\t" << passive_search[0].ratio << endl;
	for (int i = 1; i < N; i++)
	{
		passive_search[i].f_x = func(eps * i + a);
		passive_search[i].a = eps * i + a;
		passive_search[i].b = b;
		passive_search[i].length = passive_search[i].b - passive_search[i].a;
		passive_search[i].ratio = passive_search[i - 1].length / passive_search[i].length;
		//cout << i << "\t" << passive_search[i].a << "\t" << passive_search[i].b << "\t" << passive_search[i].length << "\t" << passive_search[i].ratio << endl;
	}
	long double min = numeric_limits<long double>::max();
	for (int i = 0; i < N; i++)
	{
		if (passive_search[i].f_x < min)
			min = passive_search[i].f_x;
	}
	//cout << "Min: " << min << endl;
	delete[] passive_search;
}

void dichotomy(long double eps, long double a, long double b)
{
	long double f1, f2;
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
	//cout << "0" << "\t" << dichotomy[0].a << "\t" << dichotomy[0].b << "\t" << dichotomy[0].length << "\t" << dichotomy[0].ratio << endl;
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
		//cout << i << "\t" << dichotomy[i].a << "\t" << dichotomy[i].b << "\t" << dichotomy[i].length << "\t" << dichotomy[i].ratio << endl;
	}
	//cout << "Min: " << dichotomy[N - 1].f_x << endl;
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

void Fibonacci(long double length, long double eps, long double a, long double b)
{
	int N;
	vector<int> Fib_seq;
	for (int i = 0; Fib(i) < (b - a) / length; i++)
	{
		Fib_seq.push_back(Fib(i));
		N = i + 1;
	}
	cout << "Кол-во измерений: " << N << endl;
	Fib_seq.push_back(Fib(N));
	IterTable *Fibonacci = new IterTable[N];
	Fibonacci[0].a = a;
	Fibonacci[0].b = b;
	Fibonacci[0].x1 = a + ((long double)Fib_seq[N - 2] / Fib_seq[N]*(b - a));
	Fibonacci[0].x2 = a + ((long double)Fib_seq[N - 1] / Fib_seq[N]*(b - a));
	Fibonacci[0].f_x1 = func(Fibonacci[0].x1);
	Fibonacci[0].f_x2 = func(Fibonacci[0].x2);
	Fibonacci[0].length = abs(Fibonacci[0].b - Fibonacci[0].a);
	Fibonacci[0].ratio = 0;
	//cout << "0" << "\t" << Fibonacci[0].a << "\t" << Fibonacci[0].b << "\t" << Fibonacci[0].length << "\t" << Fibonacci[0].ratio << endl;
	for (int k = 0; k < N - 2; k++)
	{
		if (Fibonacci[k].f_x1 > Fibonacci[k].f_x2)
		{
			Fibonacci[k + 1].a = Fibonacci[k].x1;
			Fibonacci[k + 1].b = Fibonacci[k].b;
			Fibonacci[k + 1].x1 = Fibonacci[k].x2;
			Fibonacci[k + 1].f_x1 = func(Fibonacci[k].x2); // этого не было в алгоритме
			Fibonacci[k + 1].x2 = Fibonacci[k + 1].a + ((long double)Fib_seq[N - k - 2] / Fib_seq[N - k - 1]*(Fibonacci[k + 1].b - Fibonacci[k + 1].a));
			Fibonacci[k + 1].f_x2 = func(Fibonacci[k + 1].x2);
		}
		else
		{
			Fibonacci[k + 1].a = Fibonacci[k].a;
			Fibonacci[k + 1].b = Fibonacci[k].x2;
			Fibonacci[k + 1].x2 = Fibonacci[k].x1;
			Fibonacci[k + 1].f_x2 = func(Fibonacci[k].x1); // этого не было в алгоритме
			Fibonacci[k + 1].x1 = Fibonacci[k + 1].a + ((long double)Fib_seq[N - k - 3] / Fib_seq[N - k - 1]*(Fibonacci[k + 1].b - Fibonacci[k + 1].a));
			Fibonacci[k + 1].f_x1 = func(Fibonacci[k + 1].x1);
		}
		Fibonacci[k + 1].length = abs(Fibonacci[k + 1].b - Fibonacci[k + 1].a);
		Fibonacci[k + 1].ratio = Fibonacci[k].length / Fibonacci[k + 1].length;
		//cout << k + 1 << "\t" << Fibonacci[k + 1].a << "\t" << Fibonacci[k + 1].b << "\t" << Fibonacci[k + 1].length << "\t" << Fibonacci[k + 1].ratio << endl;
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
	Fibonacci[N - 1].length = abs(Fibonacci[N - 1].b - Fibonacci[N - 1].a);
	Fibonacci[N - 1].ratio = Fibonacci[N - 2].length / Fibonacci[N - 1].length;
	//cout << N - 1 << "\t" << Fibonacci[N - 1].a << "\t" << Fibonacci[N - 1].b << "\t" << Fibonacci[N - 1].length << "\t" << Fibonacci[N - 1].ratio << endl;
	//cout << "Оптимальное решение содержится в интервале [" << Fibonacci[N - 1].a << ", " << Fibonacci[N - 1].b << "]" << endl;
	delete[] Fibonacci;
}

void golden_section(long double eps, long double a, long double b)
{
	long double fert = (1 + sqrt(5)) / 2;
	//cout << fert;
	vector<IterTable> gold_sec_array;
	IterTable gold_sec;
	int k = 0;
	gold_sec.a = a;
	gold_sec.b = b;
	gold_sec.length = abs(gold_sec.b - gold_sec.a);
	gold_sec.ratio = 0;
	gold_sec_array.push_back(gold_sec);
	//cout << 0 << "\t" << gold_sec.a << "\t" << gold_sec.b << "\t" << gold_sec.length << "\t" << 0 << endl;
	while (abs(gold_sec.b - gold_sec.a) > eps)
	{
		gold_sec.x1 = gold_sec.b - ((gold_sec.b - gold_sec.a) / fert);
		gold_sec.x2 = gold_sec.a + ((gold_sec.b - gold_sec.a) / fert);
		gold_sec.f_x1 = func(gold_sec.x1);
		gold_sec.f_x2 = func(gold_sec.x2);
		if (gold_sec.f_x1 >= gold_sec.f_x2)
		{
			gold_sec.a = gold_sec.x1;
		}
		else
		{
			gold_sec.b = gold_sec.x2;
		}
		gold_sec.length = abs(gold_sec.b - gold_sec.a);
		k++;
		gold_sec_array.push_back(gold_sec);
		gold_sec.ratio = gold_sec_array[k - 1].length / gold_sec_array[k].length;
		gold_sec_array[k].ratio = gold_sec.ratio;
		//cout << k << "\t" << gold_sec.a << "\t" << gold_sec.b << "\t" << gold_sec.length << "\t" << gold_sec.ratio << endl;
	}
	cout << "Кол-во измерений: " << k << endl;
	//cout << "Оптимальное решение содержится в интервале [" << gold_sec.a << ", " << gold_sec.b << "]" << endl;
}

void Newton(long double st_aprx, long double eps, long double a, long double b)
{
	vector<IterTable> Newton_array;
	IterTable Newton;
	int i = 0;
	Newton.approx = st_aprx;
	Newton.f_x = func(st_aprx);
	Newton.df_x = deriv(st_aprx);
	//cout << i << "\t" << Newton.approx << endl;
	while (abs(Newton.df_x) > eps)
	{
		Newton_array.push_back(Newton);
		Newton.approx -= Newton.f_x / Newton.df_x;
		Newton.f_x = func(Newton.approx);
		Newton.df_x = deriv(Newton.approx);
		i++;
		//cout << i << "\t" << Newton.approx << endl;
	}
	cout << "Кол-во измерений: " << i << endl;
	//cout << "Min: " << Newton.approx << endl;
}

long double deriv(long double x)
{
	return 2 * x - 4;
}
