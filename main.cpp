#include <iostream>

#include "Bitmap.h"

int main() {
	std::cout << "Enter input BMP file name: ";
	std::string inputFilePath;
	std::cin >> inputFilePath;

	auto bitmap = Bitmap::loadFromFile(inputFilePath);
	std::cout << "Here is BMP file contents: ";
	bitmap->printBitmap();

	auto width = bitmap->getWidth();
	auto height = bitmap->getHeight();
	bitmap->drawLine(0, 0, width-1, height-1, Pixel24(0));
	bitmap->drawLine(width-1, 0, 0, height-1, Pixel24(0));

	std::cout << "Here is BMP with the cross: ";
	bitmap->printBitmap();

	std::cout << "Make sure that your terminal viewport is big enough to fit the image!\n";
	std::cout << "Enter output BMP file name: ";
	std::string outputFilePath;
	std::cin >> outputFilePath;

	bitmap->saveToFile(outputFilePath);
	return 0;
}
