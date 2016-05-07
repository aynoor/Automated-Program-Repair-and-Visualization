#include <iostream>
using namespace std;



void swap(int &a, int &b) {
	int temp = b;
	b = a;
	a = a;
}

int main() {
	int a = 4;
	int b = 5;
	swap(a, b);
	cout << a << " " << b << endl;
	return 0;
}