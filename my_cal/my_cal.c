/*char _my_getchar();
void _my_putchar(char ch);
inling int isdigit(char c) {
	return c >= '0' && c <= '9';
}
int rd() {
	int x = 0;
	char c;
	while(!isdigit(c == _my_getchar()));
	x = c ^ 48;
	while(isdigit(c = _my_getchar())) {
		x = (x << 1) + (x << 3) + (c ^ 48);
	}
	return x;
}
void wr(int x) {
	static char c[10], *s = c;
	*s = 0;
	do *++s=x%10^48;while(x/=10);
	while(*x) _my_putchar(*s--);
}
void my_cal() {
	wr(rd() + rd());
}*/
char _my_getchar();
void _my_putchar(char ch);

void my_cal() {
	char c = _my_getchar();
	char ansArr[105];
	int ct = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	while (c != '\n') {
		a = a * 10 + c - '0';
		c = _my_getchar();
	}
	if (a == 0) {
		_my_putchar('0');
	} else {
		while (a != 0) {
			ansArr[ct++] = a % 2 + '0';
			a /= 2;
		}
		//ansArr[ct++] = a % 2;
		//a /= 2;
	}
	int i;
	for (i = ct - 1;i >= 0;i--) {
		_my_putchar(ansArr[i]);
	}
	/*c = _my_getchar();
	while (c != '\n') {
		b = b * 10 + c - '0';
		c = _my_getchar();
	}
	unsigned int d = a + b;
	if (d == 0) { 
		_my_putchar('0');
	} else {
		int e = 1000000000;
		while (d / e == 0) {
			e = e / 10;
		}
		while (e) { 
			_my_putchar(d / e + '0');
			d = d % e;
			e = e / 10;
		}
	}*/
}
