include <stdio.h>


void GenerateString(int n, char* string);

int main()
{

	char str[1000];
	GenerateString(50, str);
	return 0;
}

void GenerateString(int n, char* answerString)
{

	int i = 0;
	char curChar = 'a';

	if (n > 0)
	{
		answerString[0] = 'a';
		i++;
		curChar++;
	}

	while (i < n)
	{
		answerString[i++] = curChar                                                                                                                                 r;

		int j = 0;
		while ((j                                                                                                                                  < i - 1) && (i + j) < n)
		{
			answerString[i + j] = answerString[j];
			j++;
		}

		i = j + i;
		curChar++;
	}

	answerString[i] = 0;
	printf("%s \n%d", answerString, i);
}
