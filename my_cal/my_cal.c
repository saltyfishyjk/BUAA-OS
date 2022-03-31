char _my_getchar();
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
}
