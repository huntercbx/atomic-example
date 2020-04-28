#include <chrono>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <thread>

using namespace std::chrono;

// количество найденных простых чисел в диапазоне [2, N]
unsigned long n_prime_numbers = 0;


////////////////////////////////////////////////////////////////////////////////
// Функция для проверки, является ли заданное число простым
////////////////////////////////////////////////////////////////////////////////
bool is_prime(unsigned long x)
{
	unsigned long last = (long)floor(sqrt(x));
	for (unsigned long i = 2; i <= last; ++i)
		if ((x % i) == 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Функция потока
////////////////////////////////////////////////////////////////////////////////
void thread_func(unsigned long N, unsigned long i, unsigned long P)
{
	for (unsigned long j = 2 + i; j <= N; j = j + P)
	{
		if (is_prime(j))
		{
			++n_prime_numbers;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Основная программа
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// узнаем число процессоров из переменной окружения
	const char* env_number_of_processors = std::getenv("NUMBER_OF_PROCESSORS");
	const int P = env_number_of_processors == nullptr ? 1 : atoi(env_number_of_processors);

	// определение размера решаемой задачи из параметров командной строки
	const unsigned long N = (argc == 2) ? atol(argv[1]) : 30000000;

	// запускаем расчеты
	printf("Searching prime numbers in interval [%lu, %lu] using %d threads\n", 2, N, P);

	// время начала рассчетов
	auto start = high_resolution_clock::now();

	// создаем потоки
	std::vector<std::thread> threads;
	for (unsigned long i = 0; i < P; ++i)
		threads.emplace_back(thread_func, N, i, P);

	// ждем завершения
	for (auto& th : threads)
		th.join();

	// время окончания рассчетов
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	// вывод количества найденых простых чисел в диапазоне
	printf("Found %lu prime numbers\n", n_prime_numbers);

	// вывод затраченного времени
	printf("Execution time: %llu ms\n", duration.count());

	return 0;
}