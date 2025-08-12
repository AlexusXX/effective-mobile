//
// Created by alexus on 18.07.2025.
//

#include "Bitmap.h"

#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#define BMP_MAGIC_LENDIAN 0x4D42
#define BMP_MAGIC_BENDIAN 0x424D

void throw_exception(const std::string &message, const std::ios_base::failure & e) {
	throw std::runtime_error(std::format("{}: {} ({}) ", message, e.what(), e.code().message()));
}

std::shared_ptr<Bitmap> Bitmap::loadFromFile(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Could not open file " + filename);

	file.exceptions(std::_S_failbit);

	uint16_t fileMagic;
	try {
		file.read(reinterpret_cast<std::istream::char_type*>(&fileMagic), sizeof(fileMagic));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not read the magic number", e);
	}

	if (fileMagic != BMP_MAGIC_LENDIAN && fileMagic != BMP_MAGIC_BENDIAN)
		throw std::runtime_error("Invalid magic number. It seems that your file is not a bitmap (.bmp) file");

	uint32_t offBits;
	uint32_t bfSize;
	try {
		file.read(reinterpret_cast<std::istream::char_type*>(&bfSize), sizeof(bfSize));
		//Skip bfReserved1 (2), bfReserved2 (2);
		file.seekg(2+2, std::_S_cur);
		file.read(reinterpret_cast<std::istream::char_type*>(&offBits), sizeof(offBits));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not read the bitmap file header", e);
	}

	std::shared_ptr<Bitmap> bitmap = create();
	try {
		file.read(reinterpret_cast<std::istream::char_type*>(&bitmap->version), sizeof(bitmap->version));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not read the version of the bitmap file", e);
	}
	uint16_t depth = 0;
	try {
		std::streamsize dimensionsBytes = bitmap->getDimensionsByteCount();
		file.read(reinterpret_cast<std::istream::char_type*>(&bitmap->width), dimensionsBytes);
		file.read(reinterpret_cast<std::istream::char_type*>(&bitmap->height), dimensionsBytes);
		//skip bcPlanes
		file.seekg(2, std::_S_cur);
		file.read(reinterpret_cast<std::istream::char_type*>(&depth), sizeof(depth));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not read the dimensions of the bitmap file", e);
	}

	if (depth!=24) {
		throw std::runtime_error("Unsupported depth. Only 24 bits supported");
	}

	bitmap->data.resize(bfSize-offBits);

	try {
		file.seekg(offBits);
		file.read(reinterpret_cast<std::istream::char_type*>(bitmap->data.data()), static_cast<std::streamsize>(bitmap->data.size()));

	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not read the bitmap file pixel data", e);
	}

	file.close();
	return bitmap;
}

void Bitmap::drawLine(const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const Pixel24 color) {
	float dx = static_cast<float>(x1) - static_cast<float>(x2);
	float dy = static_cast<float>(y1) - static_cast<float>(y2);
	float step = std::abs(std::abs(dx) >= std::abs(dy) ? dx : dy);
	dx = dx / step;
	dy = dy / step;
	auto x = static_cast<float>(x2);
	auto y = static_cast<float>(y2);
	float i = 0;

	while (i <= step) {
		setPixelAt(std::round(x), std::round(y), color);
		x = x + dx;
		y = y + dy;
		i = i + 1;
	}
}

void Bitmap::printBitmap() const {

	std::cout << std::endl;
	for (uint32_t x = 0; x < width; x++) {
		for (uint32_t y = 0; y < height; y++) {
			auto pixel = getPixelAt(width - x - 1, y) ;
			if (pixel == Pixel24(255)) {//White
				std::cout << "\033[107m   ";
			}else {//Black
				std::cout << "\033[40m   ";
			}
		}
		std::cout<<"\033[49m" << std::endl;
	}
}

void Bitmap::saveToFile(const std::string &filename) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not create and open file " + filename);

	file.exceptions(std::_S_failbit);

	uint16_t fileMagic = BMP_MAGIC_LENDIAN;
	try {
		file.write(reinterpret_cast<std::istream::char_type*>(&fileMagic), sizeof(fileMagic));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not write the magic number", e);
	}
	uint32_t offBits = 54;
	uint32_t size = static_cast<std::streamsize>(data.size())+offBits;
	try {
		file.write(reinterpret_cast<std::istream::char_type*>(&size), sizeof(size));
		//Skip bfReserved1 (2), bfReserved2 (2);
		file.seekp(2+2, std::_S_cur);
		file.write(reinterpret_cast<std::istream::char_type*>(&offBits), sizeof(offBits));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not write the bitmap file header", e);
	}
	try {
		file.write(reinterpret_cast<std::istream::char_type*>(&version), sizeof(version));
	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not write the version of the bitmap file", e);
	}

	try {
		std::streamsize dimensionsBytes = getDimensionsByteCount();
		file.write(reinterpret_cast<std::istream::char_type*>(&width), dimensionsBytes);
		file.write(reinterpret_cast<std::istream::char_type*>(&height), dimensionsBytes);
		uint16_t bcPlanes = 1;
		file.write(reinterpret_cast<std::istream::char_type*>(&bcPlanes), sizeof(bcPlanes));
		uint16_t depth = 24;
		file.write(reinterpret_cast<std::istream::char_type*>(&depth), sizeof(depth));

	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not write the dimensions of the bitmap file", e);
	}

	try {
		file.seekp(offBits);
		file.write(reinterpret_cast<std::istream::char_type*>(data.data()), static_cast<std::streamsize>(data.size()));

	}catch (const std::ios_base::failure &e) {
		throw_exception("Could not write the bitmap file pixel data", e);
	}

	file.close();
}
