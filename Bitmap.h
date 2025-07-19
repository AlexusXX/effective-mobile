//
// Created by alexus on 18.07.2025.
//

#ifndef EFFECTIVE_MOBILE_TEST_BITMAP_H
#define EFFECTIVE_MOBILE_TEST_BITMAP_H
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

enum class Version : uint32_t{
	VERSION_CORE = 12,
	VERSION_3 = 40,
	VERSION_4 = 108,
	VERSION_5 = 124
};

struct Pixel24 {
	uint8_t r{};
	uint8_t g{};
	uint8_t b{};

	Pixel24() = default;

	explicit Pixel24(uint8_t val)
			: r(val),
			  g(val),
			  b(val) {}

	bool operator==(const Pixel24 &pRhs) const {
		return this->r == pRhs.r
			   && this->g == pRhs.g
			   && this->b == pRhs.b;
	}
};

class Bitmap {
	friend class BitmapLoader;
	Version version = Version::VERSION_3;
	uint32_t width{}, height{};
	std::vector<uint8_t> data;

protected:
	Bitmap() = default;
public:
	static std::shared_ptr<Bitmap> create() { return std::shared_ptr<Bitmap>(new Bitmap()); }

	/**
	 * @brief Load bitmap from file.
	 * @param filename Path to file.
	 * @return Pointer to Bitmap that represents the loaded bitmap.
	 * @throw std::runtime_error When failed to open file, read its contents or parse the bitmap structure.
	 */
	static std::shared_ptr<Bitmap> loadFromFile(const std::string & filename);

	/**
	 * @brief Get color data at the specified coordinates.
	 * @param x X image coordinate.
	 * @param y Y image coordinate.
	 * @return Pixel color.
	 * @throw std::out_of_range When the specified coordinates is greater than image size.
	 */
	[[nodiscard]] Pixel24 getPixelAt(uint32_t x, uint32_t y) const {
		const auto pos = coordsToPos(x, y);
		if (data.size()<=pos) throw std::out_of_range("Bitmap::getPixelAt");
		Pixel24 pixelOut{0};
		std::memcpy(&pixelOut, &data[pos], sizeof(pixelOut));
		return pixelOut;
	}

	/**
	 * @brief Set pixel color at the specified coordinates.
	 * @param x image coordinate.
	 * @param y image coordinate.
	 * @param color Color to set.
	 * @throw std::out_of_range When the specified coordinates is greater than image size.
	 */
	void setPixelAt(uint32_t x, uint32_t y, Pixel24 color) {
		const auto pos = coordsToPos(x, y);
		if (data.size()<=pos) throw std::out_of_range("Bitmap::setPixelAt");
		std::memcpy(&data[pos], &color, sizeof(color));
	}

	[[nodiscard]] uint32_t getWidth() const { return width; }

	[[nodiscard]] uint32_t getHeight() const { return height; }

	/**
	 * @brief Draw line in the bitmap image.
	 * @param x1 X coordinate of the start point.
	 * @param y1 Y coordinate of the start point.
	 * @param x2 X coordinate of the end point.
	 * @param y2 Y coordinate of the end point.
	 * @param color Line color.
	 */
	void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Pixel24 color);

	/**
	 * @brief Print bitmap image to stdout.
	 * @note Since image stored in the bmp-file is flipped, method flips it back using the following expression:
	 * `width - x - 1`
	 */
	void printBitmap() const;

	/**
	 * @brief Save the current bitmap to file.
	 * @param filename Path to file.
	 * @throw std::runtime_error When failed to open file, save its contents or parse the bitmap structure.
	 */
	void saveToFile(const std::string & filename);

private:
	/**
	 * @brief Get width/height byte count depending on bitmap version.
	 * @return 2 bytes for core version, 4 bytes for other versions.
	 */
	[[nodiscard]] std::streamsize getDimensionsByteCount() const {
		if (version == Version::VERSION_CORE) return sizeof(uint16_t);
		return sizeof(uint32_t);
	}

	/**
	 * @brief Convert image coordinates to byte offset in data buffer from start.
	 * @param x X coord.
	 * @param y Y coord.
	 * @return byte offset.
	 */
	[[nodiscard]] size_t coordsToPos(const uint32_t x, const uint32_t y) const {
		return (y * width + x) * 3ll;
	}
};



#endif //EFFECTIVE_MOBILE_TEST_BITMAP_H