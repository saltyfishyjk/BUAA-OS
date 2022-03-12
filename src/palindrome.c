#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int num[15] = {0};
	int ct = 0;
	int i;
	while (n != 0) {
		num[ct++] = n % 10;
		n /= 10;
	}
	
	int judge = 1;

	for (i = 0 ; i < ct;i++) {
		if (num[i] != num[ct - i - 1]) {
			judge = 0;
			break;
		}
	}
	





	if(judge == 1){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
