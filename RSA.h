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

class RSA {
public:
	void begin() {
		std::cout << "Creating RSA keys...\n";
		srand(time(NULL));
		size_t p = generatePrimeNumber();
		size_t q = generatePrimeNumber();

		// To prevent duplicate prime numbers.
		while (p == q) {
			q = generatePrimeNumber();
		}

		// calculate n, needed for encryption and decryption.
		this->n = p * q;
		size_t z = (p - 1) * (q - 1);

		// calculating e, this will be your public encryption key.
		this->e = generateE(z);

		// calculating d, this will be your private decryption key.
		this->d = generateD(z, e);
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
				size_t x = hexToDec(hexVal);
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
	size_t e = 0;
	size_t n = 0;
	size_t d = 0;
	size_t generatePrimeNumber() {
		srand(rand());

		size_t num = 0;
		bool isPrime = false;
		size_t maxValue = (int)sqrt(sqrt(ULLONG_MAX));
		while (!isPrime) {
			num = (rand() % maxValue);
			if (num >= 7919) {
				bool flag = false;
				for (size_t i = 2; i < num; i++) {

					// if this condition is true 'num' is not a prime number.
					if (num % i == 0) {
						flag = true;
						srand(num);
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
	size_t generateE(size_t z) {
		std::vector<size_t> arr;

		//Ф(n) = z
		//1 < e < Ф(n) and must be coprime with this->n and Ф(n)
		for (size_t i = 2; i < z; i++) {
			if (gcd(i, z) == 1) {
				arr.push_back(i);
				if (arr.size() == 100) {
					break;
				}
			}
		}

		return arr.at(rand() % arr.size());
	}
	size_t generateD(size_t z, size_t e) {
		std::vector<size_t> array;
		for (size_t d = 2; d < z; d++) {

			// e * d (mod Ф(n)) = 1 your private key must obey this condition.
			if (e * d % z == 1) {
				return d;
			}
		}
	}
	size_t gcd(size_t var1, size_t var2) {
		if (var1 == 0) {
			return var2;
		}
		else {
			return gcd(var2 % var1, var1);
		}
	}
	size_t hexToDec(std::string hex) {
		return std::stoi(hex, 0, 16);
	}
	size_t cipher(size_t var, size_t power, size_t n) {
		size_t result = 1;
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