//
// Created by alexus on 10.08.2025.
//

#ifndef EFFECTIVE_MOBILE_LIBRARY_H
#define EFFECTIVE_MOBILE_LIBRARY_H
#include <string>
#include <vector>

namespace my::perfect::library
{

	struct BookShelf
	{
		std::vector< std::string > books;
	};

	constexpr decltype(BookShelf::books)::const_iterator begin(const BookShelf &shelf) noexcept {
		return shelf.books.begin();
	}

	constexpr decltype(BookShelf::books)::const_iterator end(const BookShelf &shelf) noexcept {
		return shelf.books.end();
	}
} // namespace my::perfect::library
#endif //EFFECTIVE_MOBILE_LIBRARY_H