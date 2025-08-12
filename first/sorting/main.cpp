//
// Created by alexus on 10.08.2025.
//
#include <algorithm>
#include <format>
#include <iostream>
#include <string>
#include <vector>

struct Person
{
	enum {male, female} gender;
	std::string first_name, last_name;
	std::size_t id;
};

void print(const std::vector<Person> & persons) {

	for (const auto & person : persons) {
		std::cout << std::format("{}, {} {}\n", person.id, person.first_name, person.last_name);
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	std::vector<Person> persons;
	persons.emplace_back(Person::male, "Филипп", "Смирнов", 1);
	persons.emplace_back(Person::female, "Мария", "Уварова", 2);
	persons.emplace_back(Person::female, "Мария", "Селезнева", 3);
	persons.emplace_back(Person::female, "Есения", "Родионова", 4);
	persons.emplace_back(Person::male, "Вадим", "Сорокин", 5);
	persons.emplace_back(Person::male, "Эмир", "Плотников", 6);
	persons.emplace_back(Person::female, "Яна", "Баранова", 7);
	persons.emplace_back(Person::female, "Оливия", "Гусева", 8);
	persons.emplace_back(Person::female, "Варвара", "Шарова", 9);
	persons.emplace_back(Person::male, "Михаил", "Ермолаев", 10);

	std::cout<<"Несорт:\n";
	print(persons);

	std::cout<<"\nСорт по имени и фамилии:\n";
	std::ranges::sort(persons, [](const Person & a, const Person & b) {
		return a.first_name < b.first_name || (a.first_name == b.first_name && a.last_name < b.last_name);
	});
	print(persons);

	std::cout<<"\nСорт по фамилии:\n";
	std::ranges::sort(persons, std::ranges::greater{}, &Person::last_name);
	print(persons);

	std::cout<<"\nСорт по id (сначала мужчины):\n";
	std::ranges::sort(persons, [](const Person & a, const Person & b) {
		return a.gender != b.gender ? a.gender == Person::male : a.id < b.id;
	});
	print(persons);


	return 0;
}
