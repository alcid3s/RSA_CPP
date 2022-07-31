#include "RSA.h"

int main() {
	RSA rsa;
	std::string data = "Hello, World!";

	rsa.begin();
	std::cout << "data: " << data << "\n";
	data = rsa.encryptMessage(data);
	std::cout << "Encrypted data: " << data << "\n";
	data = rsa.decryptMessage(data);
	std::cout << "Decrypted data: " << data << "\n";

	return 0;
}