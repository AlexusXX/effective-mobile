//
// Created by alexus on 27.08.2025.
//


#include <functional>
#include <optional>
#include <random>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>

struct Person {
	std::string name;
	int age;
};

void generate(std::vector<Person> &pOut) {

	std::random_device rd{};
	std::mt19937 rnd(rd());
	pOut.reserve(2000000);
	for(int i = 0; i < 2000000; i++){
		pOut.emplace_back("Person " + std::to_string(i), static_cast<int>(rnd()) % 100);
	}

}

static void pointer(benchmark::State& state) {
	std::vector<Person> people;
	generate(people);
	for (auto _ : state) {
		std::ranges::sort(people, std::less<int>{}, &Person::age);
		benchmark::DoNotOptimize(people);
	}
}

static void lambda(benchmark::State& state) {
	std::vector<Person> people;
	generate(people);
	for (auto _ : state) {
		std::ranges::sort(people, std::less<int>{}, [](const Person& p) { return p.age; });
		benchmark::DoNotOptimize(people);
	}
}
static int func(const Person& p) {
	return p.age;
}
static void pointer2function(benchmark::State& state) {
	std::vector<Person> people;
	generate(people);
	for (auto _ : state) {
		std::ranges::sort(people, std::less<int>{}, func);
		benchmark::DoNotOptimize(people);
	}
}

static void function(benchmark::State& state) {
	std::vector<Person> people;
	generate(people);
	for (auto _ : state) {
		std::ranges::sort(people, std::less<int>{}, std::function(func));
		benchmark::DoNotOptimize(people);
	}
}

BENCHMARK(pointer);
BENCHMARK(lambda);
BENCHMARK(pointer2function);
BENCHMARK(function);
BENCHMARK_MAIN();

