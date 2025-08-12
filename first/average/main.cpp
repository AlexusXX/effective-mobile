#include <benchmark/benchmark.h>

#include <algorithm>
#include <optional>
#include <random>
#include <memory>
#include <vector>

static void by_value_first( benchmark::State& state )
{
	std::vector< std::optional<int> > values;
	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 2000000; i++){
		values.emplace_back(rnd());
	}

	std::ranges::shuffle(values, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : values) {
			avg+=value.value();
		}
		avg = avg/values.size();
		benchmark::DoNotOptimize(avg);
	}
}

static void by_ref_first( benchmark::State& state )
{
	std::vector<std::unique_ptr<int>> pointers; // по возможности заменить int* на std::unique_ptr<int>

	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 2000000; i++){
		pointers.emplace_back(std::make_unique<int>(rnd()));
	}

	std::ranges::shuffle(pointers, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : pointers) {
			avg+=*value;
		}
		avg = avg/pointers.size();
		benchmark::DoNotOptimize(avg);
	}
	// ...
}

BENCHMARK(by_value_first);
BENCHMARK(by_ref_first);

struct Person
{
    enum {male, female} gender;
    std::string first_name, last_name;
	std::size_t age{};
};

static void by_value_second( benchmark::State& state )
{
	std::vector< std::optional<Person> > values;
	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 2000000; i++){
		values.emplace_back(Person{Person::male, "", "", rnd() % 100});
	}

	std::ranges::shuffle(values, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : values) {
			avg+=value.value().age;
		}
		avg = avg/values.size();
		benchmark::DoNotOptimize(avg);
	}
}

static void by_ref_second( benchmark::State& state )
{
	std::vector<std::unique_ptr<Person>> pointers; // по возможности заменить int* на std::unique_ptr<int>

	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 2000000; i++){
		pointers.emplace_back(std::make_unique<Person>(Person::male, "", "", rnd()% 100));
	}

	std::ranges::shuffle(pointers, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : pointers) {
			avg+=value->age;
		}
		avg = avg/pointers.size();
		benchmark::DoNotOptimize(avg);
	}
	// ...
}

BENCHMARK(by_value_second);
BENCHMARK(by_ref_second);

static void by_value_third( benchmark::State& state )
{
	std::vector< std::optional<Person> > values;
	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 1000000; i++){
		values.emplace_back(Person{Person::male, "", "", rnd() % 100});
	}
	for(int i = 0; i < 1000000; i++){
		values.emplace_back(std::nullopt);
	}

	std::ranges::shuffle(values, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : values) {
			if (value) avg+=value->age;
		}
		avg = avg/values.size();
		benchmark::DoNotOptimize(avg);
	}
}

static void by_ref_third( benchmark::State& state )
{
	std::vector<std::unique_ptr<Person>> pointers; // по возможности заменить int* на std::unique_ptr<int>

	// здесь заполнить значениями и перемешать
	std::random_device rd{};
	std::mt19937 rnd(rd());
	for(int i = 0; i < 1000000; i++){
		pointers.emplace_back(std::make_unique<Person>(Person::male, "", "", rnd()% 100));
	}
	for(int i = 0; i < 1000000; i++){
		pointers.emplace_back(nullptr);
	}

	std::ranges::shuffle(pointers, rnd);
	for (auto _ : state)
	{
		int avg = 0;

		// здесь посчитать среднее

		for (const auto & value : pointers) {
			if (value) avg+=value->age;
		}
		avg = avg/pointers.size();
		benchmark::DoNotOptimize(avg);
	}
	// ...
}

BENCHMARK(by_value_third);
BENCHMARK(by_ref_third);
BENCHMARK_MAIN();