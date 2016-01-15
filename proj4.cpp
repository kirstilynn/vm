#include <iostream>

using namespace std;

int factorial(int number) {
	int temp;

	if(number <= 1) return 1;

	temp = number * factorial(number - 1);
	return temp;
}

int main() {  
	int num;
	cin >> num;
	while (num != 0) {
		cout << "Factorial of " << num << " is " << factorial(num) << endl;
		cin >> num;
	}
	return 0;
}
