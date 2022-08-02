/*
* Copyright (c) 2022
* Author: Alcides
*
* Permission to use, copy and modify this software for educational purpose only,
* provided that the authors name and this permission notice appear in supporting
* documentation. Do not use this software for commercial purposes, secure communication
* cannot be guarranteed with this software.
*/

#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

const auto UINT1024T_MAX = std::numeric_limits<int1024_t>::max();
const auto UINT512T_MAX = std::numeric_limits<int512_t>::max();
const auto UINT256T_MAX = std::numeric_limits<int256_t>::max();
const auto UINT128T_MAX = std::numeric_limits<int128_t>::max();

using namespace boost::multiprecision;
class RSA {
public:
	void begin() {
		std::cout << "Creating RSA keys...\n";
		srand(time(NULL));
		uint1024_t p = generatePrimeNumber();
		uint1024_t q = generatePrimeNumber();

		// To prevent duplicate prime numbers.
		while (p == q) {
			q = generatePrimeNumber();
		}

		// calculate n, needed for encryption and decryption.
		this->n = p * q;
		uint1024_t z = (p - 1) * (q - 1);

		// calculating e, this will be your public encryption key.
		this->e = generateE(z);

		// calculating d, this will be your private decryption key.
		this->d = generateD(z, this->e);
		std::cout << "Done!\n";
	}
	std::string encryptMessage(std::string& message) {
		std::stringstream ss;
		for (size_t i = 0; i < message.length(); i++)
		{
			// encrypts the character on message[i] and converts the number to base 16.
			ss << std::hex << (cipher(message.at(i), this->e, this->n));
			ss << " ";
		}
		return ss.str();
	}
	std::string decryptMessage(std::string& cipherText) {
		std::string encryptedMessage;
		std::string hexVal;
		for (size_t i = 0; i < cipherText.length(); i++)
		{
			// a space indicates the end of a base 16 number afterwards the cipher method will decrypt the character.
			if (cipherText.at(i) == ' ') {
				uint1024_t x = hexToDec(hexVal);
				encryptedMessage += (char)cipher(x, this->d, this->n);
				hexVal.clear();
			}
			else {
				hexVal += cipherText.at(i);
			}
		}
		return encryptedMessage;
	}
private:
	uint1024_t e = 0;
	uint1024_t n = 0;
	uint1024_t d = 0;

	uint1024_t generatePrimeNumber() {
		srand(rand());

		uint1024_t num = 0;
		bool isPrime = false;
		size_t maxValue = sqrt(UINT1024T_MAX);
		while (!isPrime) {
			num = (uint1024_t) pow(pow(rand(), rand()), pow(rand(), rand()));
			num %= maxValue;
			if (num >= 7919) {
				bool flag = false;
				for (size_t i = 2; i < num; i++) {

					// if this condition is true 'num' is not a prime number.
					if (num % i == 0) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					isPrime = true;
				}
			}
		}
		return num;
	}
	uint1024_t generateE(uint1024_t z) {
		std::vector<uint1024_t> arr;

		//Ф(n) = z
		//1 < e < Ф(n) and must be coprime with this->n and Ф(n)
		for (uint1024_t i = 2; i < z; i++) {
			if (gcd(i, z) == 1) {
				arr.push_back(i);
				if (arr.size() == 100) {
					break;
				}
			}
		}

		return arr.at(rand() % arr.size());
	}
	uint1024_t generateD(uint1024_t z, uint1024_t e) {
		std::vector<uint1024_t> array;
		for (uint1024_t d = 2; d < z; d++) {

			// e * d (mod Ф(n)) = 1 your private key must obey this condition.
			if (e * d % z == 1) {
				return d;
			}
		}
	}
	uint1024_t gcd(uint1024_t var1, uint1024_t var2) {
		if (var1 == 0) {
			return var2;
		}
		else {
			return gcd(var2 % var1, var1);
		}
	}
	uint1024_t hexToDec(std::string hex) {
		size_t len = hex.length();
		size_t base = 1;
		uint1024_t temp = 0;
		for (size_t i = len -1; i >= 0; i--)
		{
			if (hex.at(i) >= '0' && hex[i] <= '9') {
				temp += (hex[i] - 48) * base;
				base = base * 16;
			}
			else if (hex[i] >= 'A' && hex[i] <= 'F') {
				temp += (hex[i] - 55) * base;
				base = base * 16;
			}
		}
		return temp;
	}
	uint1024_t cipher(uint1024_t var, uint1024_t power, uint1024_t n) {
		uint1024_t result = 1;
		while (power > 0) {
			if ((power & 1) == 1) {
				result = result * var % n;
			}
			var = var * var % n;
			power >>= 1;
		}
		return result;
	}
};