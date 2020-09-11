#include <stdio.h>

char number[100];
int n, m;
int main()
{
	int c = 1;
	int x = c++ + c;
	printf("%d", x);
	scanf("%d %d", &n, &m);
	scanf("%s", number);
	long long num10 = 0;

	int it = 0;
	while (number[it + 1] != 0)
		it++;

	long long mult = 1;
	while (it >= 0)
	{
		if ('0' <= number[it] && number[it] <= '9')
			num10 += ((long long)(number[it] - 48)) * mult;
		else
			num10 += ((long long)(number[it] - 'a' + 10)) * mult;
		mult *= n;
		it--;
	}

	mult = m;
	it = 0;
	while (mult <= num10)
	{
		mult *= m;
	}
	mult /= m;

	int isEnd = 0;
	while (!isEnd)
	{
		if (mult == 1) isEnd = 1;

		int curVal = (int)(num10 / mult);
		num10 = num10 % mult;
		mult /= m;
		if (curVal < 10)
			number[it] = (char)(curVal + '0');
		else
			number[it] = (char)('a' + curVal - 10);

		it++;
	}
	number[it] = 0;
	printf("%s", number);
	return 0;
}
