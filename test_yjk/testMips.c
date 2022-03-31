int fib(int n)
{
	if (n == 0 || n == 1) {
		return 1;
	}
	return fib(n-1) + fib(n-2);
}

int main()
{
	int i;
	int sum = 0;
	for (i = 0;i < 10;i++) {
		sum += fib(i);
	}
	
	return 0;
}
