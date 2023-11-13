#include <stdio.h>

/**
 * factorial - Calculate the factorial of a number using recursion.
 * @n: The number for which to calculate the factorial.
 *
 * Return: The factorial of the given number.
 */
int factorial(int n)
{
	if (n == 0 || n == 1)
		return (1);
	else
		return (n * factorial(n - 1));
}

/**
 * main - Entry point of the program.
 *
 * Return: Always 0 (success).
 */
int main(void)
{
	int num = 5;
	int result;

	result = factorial(num);

	printf("Factorial of %d is: %d\n", num, result);

	return (0);
}

