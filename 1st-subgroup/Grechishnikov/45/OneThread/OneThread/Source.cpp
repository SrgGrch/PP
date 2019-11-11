#include <conio.h>
#include <iostream>
#include <cassert>
#include <stdio.h>
#include <algorithm>

bool isPrime(int n) {
	for (int j = 3; j <= n / 2; j++) {
		if (n % j == 0) return false;
	}

	int number = n;
	int digitsCounter = 0;

	while (number != 0) {
		digitsCounter++;
		number /= 10;
	}

	if (digitsCounter <= 2) return false;

	//check end
	if ((n % 10) <= 2) return false;

	for (int j = 2; j <= (n % 10)/2; j++) {
		if ((n % 10) % j == 0) return false;
	}

	int powRes = 10;

	for (int j = 0; j < digitsCounter - 2; j++) {
		powRes *= 10;
	}

	if ((n / powRes) <= 2) return false;

	for (int j = 2; j <= (n / powRes) / 2; j++) {
		if ((n / (powRes)) % j == 0) return false;
	}

	powRes = 10;

	for (int j = 0; j < digitsCounter - 3; j++) {
		powRes *= 10;
	}

	int middle = (n / 10) % powRes;

	//check middle
	if (middle <= 2) return false;

	for (int j = 2; j <= middle / 2; j++) {
		if (middle % j == 0) return false;
	}
	return true;
}

void main() {
	int n = 1;
	//while (true) {
	//	if (isPrime(n)) break;
	//}
	std::cout << isPrime(21) << " ";
	std::cout << isPrime(225) << " ";
	std::cout << isPrime(30113) << " ";
	std::cout << isPrime(185) << " ";

	std::cin;
}
