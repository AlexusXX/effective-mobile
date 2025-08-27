//
// Created by alexus on 10.08.2025.
//

#include <iostream>

struct Heavy
{
	Heavy() = default;

	Heavy( const Heavy& other )
	{
		std::cout << "a Heavy was just copied" << std::endl;
	}
};

class Container
{
public:
	const Heavy &extract() const
	{
		return item;
	}

private:
	Heavy item;
};

Container generate()
{
	return {};
}

int main()
{
	auto container = generate();
	auto &heavy1 = container.extract();
	auto &heavy2 = generate().extract();

	return 0;
}