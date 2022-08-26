# RSA_CPP
The [**RSA encryption**](https://brilliant.org/wiki/rsa-encryption/) (Rivest-Shamir-Adleman) is a method for encrypting and decrypting data.
This encryption method is widely used for secure data transmission. This project is an implementation of said algorithm with randomisation
so the encryption and decryption key cannot be guessed right off the bat. The RSA.h header file can be used for educational purposes only,
this is due to the prime numbers being too insignificant to actually secure any data efficiently and effectively. It is however a great way 
to see how the RSA encryption method works in practice.

## RSA breakdown
For the RSA encryption method the program starts with 2 prime numbers.
```cpp
		size_t p = generatePrimeNumber();
		size_t q = generatePrimeNumber();

		while (p == q) {
			q = generatePrimeNumber();
		}
```
For now let's say P is equal to 11 and Q is equal to 13. With those two variables the program will calculate the N value by multiplying both the
p and the q together. N = P * Q, N = 11 * 13, N = 143. 

Now comes a mathematical function called [Euler's totient function](https://www.youtube.com/watch?v=qa_hksAzpSg) or "Euler's phi" that bears the Ф mark.
In the program the phi of N (Ф(N)) gots the character Z. Z is calculated like so: Z = (P - 1) * (Q - 1), Z = (11 - 1) * (13 - 1), Z = 120.
With those numbers done the program goes on to calculate the E and the D. 

The E must obey the following conditions: 1 < e < Ф(N) and e has to be [coprime](https://en.wiktionary.org/wiki/coprime)
with both N and Ф(N). The program does this by checking the [Greatest Common Divisor](https://www.youtube.com/watch?v=JUzYl1TYMcU) (gcd) of all numbers up to Ф(N) and if the gcd is equal to 1 it obeys the conditions given above.
```cpp
	size_t generateE(size_t z) {
		std::vector<size_t> arr;

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
```
For this example let's take 7 to keep it simple.

For the D there are another set of conditions it has to follow. The formula for D is as follows: de(mod Ф(N)) = 1. For the current situation it'll be like this: 7 * d (mod 120) = 1.
The program gets D simply by using a for loop starting from 2 all the way up too Ф(N). If it finds a number that obeys these conditions it will return that number as D.
```cpp
	size_t generateD(size_t z, size_t e) {
		std::vector<size_t> array;
		for (size_t d = 2; d < z; d++) {

			// e * d (mod Ф(n)) = 1 your private key must obey this condition.
			if (e * d % z == 1) {
				return d;
			}
		}
	}
```

For this example that would be 103.

Now the program has got all the values it'll need:
```
N = 143
E = 7
D = 103
```

Let's encrypt a character to see if it all works. Let's take the letter E and assign it to the number 5. To encrypt this we'll do the following (M = Message): 
M^N (mod N), or in our case: 5^7 (mod 143) = 47. This number isn't very handy to store so the program converts this number to a hexadecimal number. 
47 in hexadecimal is equal to '2F'.
```cpp
		std::stringstream ss;
		for (size_t i = 0; i < message.length(); i++)
		{
			// encrypts the character on message[i] and converts the number to base 16.
			ss << std::hex << (cipher(message.at(i), this->e, this->n));
			ss << " ";
		}
		return ss.str();
	}
```

In order to decrypt this message it first needs to be put back to decimal (or base-10). We already now 2F is 47, now in order to decrypt this message the exact same thing is done.
Except now instead of using the E, the D will be used. Now the formula is as follows: M^D (mod 143), in this case: 47^103 (mod 143). 47^103 is a very, very, very big number. It cannot
be stored in a 64 bit integer. To understand the solution to this problem better let's first take a look at the code:
```cpp
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
```
In this code, 'power' is our E or D, in this case it will function as our D. Our var is 47 and our n is 143. While we haven't checked all bits of power (47) the code keeps going. 
47 in binary is 1100111. Let's start with the first bit on the right. It's 1 so result = result * 1 or result = 1 * 47 % 143 = 47. var = var * var % 143 or var = 47 * 47 % 143 = 64.
this keeps going on. However when the fourth bit is checked it will be 0. So only the var will change and result will stay the same. Keep doing this and when the while loop is finished 
result will be equal to 5. 

My notes for this example:
```
power = 1100111
result = 1
var = 47
n = 143

first loop:
if(1):
result = 1 * 47 % 143 = 47
var = 47 * 47 % 143 = 64

second loop:
if(1):
result = 47 * 64 % 143 = 5
var = 64 * 64 % 143 = 92

third loop:
if(1):
result = 5 * 92 % 143 = 31
var = 92 * 92 % 143 = 27

fourth loop:
if(0):
var = 27 * 27 % 143 = 14

fifth loop:
if(0):
var = 14 * 14 % 143 = 53

sixth loop:
if(1):
result = 31 * 53 % 143 = 70
var = 53 * 53 % 143 = 92

final loop:
if(1):
result = 70 * 92 % 143 = 5
var = 92 * 92 % 143 = 27
```

## Output
Upon building and running RSA_CPP.cpp. The output could be something along the lines of this:
```
Creating RSA keys...
Done!
data: Hello, World!
Encrypted data: 4cafdfd ce3793 4e53794 4e53794 47a8dbb 2c237ee 2ee48f3 b341b0 47a8dbb 2a1255 4e53794 2dcc66e 311be7d
Decrypted data: Hello, World!
```

The encrypted data will differ from your output. 
