//
// Created by alexus on 10.08.2025.
//

#include "library.h"

#include <iostream>

void print( const my::perfect::library::BookShelf & books )
{
	for ( auto book : books )
	{
		std::cout << book << std::endl;
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	my::perfect::library::BookShelf shelf;
	shelf.books.emplace_back("Гарри Поттер");
	shelf.books.emplace_back("Мёртвые души");
	shelf.books.emplace_back("Преступление и наказание");
	print(shelf);
}
