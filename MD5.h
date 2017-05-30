#pragma once

#include <string>

/*
* uint32_t is unsigned int
* uint8_t is unsigned char
* size_t is unsigned int
*/

class MD5
{
public:
	MD5();
	~MD5();

	void runMD5(const std::string& init_msg);

	/**
	* Private Class Data for MD5
	*/
	const uint32_t r[64]
	{
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

	uint32_t k[64];
	uint8_t result[16];

	/**
	* Private Functions
	*/
	void fill_k();
	void to_bytes(const uint32_t& val, uint8_t* const bytes);
	uint32_t to_int32(const uint8_t* const bytes);
	uint32_t shift_left(const uint32_t& x, const uint32_t& c);

	uint32_t f(const uint32_t& x, const uint32_t& y, const uint32_t& z) const;
	uint32_t g(const uint32_t& x, const uint32_t& y, const uint32_t& z) const;
	uint32_t h(const uint32_t& x, const uint32_t& y, const uint32_t& z) const;
	uint32_t i(const uint32_t& x, const uint32_t& y, const uint32_t& z) const;

	void printMD5() const;
};