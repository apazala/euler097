#include <stdio.h>
#include <inttypes.h>

#define ONEMILL 1000000

typedef struct twelvedigits {
	int a1;
	int a0;
}twelvedigits;

//n1 += n2
void add(twelvedigits *n1, const twelvedigits *n2)
{
	n1->a0 += n2->a0;
	if (n1->a0 >= ONEMILL)
	{
		n1->a0 -= ONEMILL;
		n1->a1++;
	}

	n1->a1 += n2->a1;
	if (n1->a1 >= ONEMILL)
		n1->a1 -= ONEMILL;
}

//n1 *= n2
void mult(twelvedigits *n1, const twelvedigits *n2)
{
	int64_t c0 = n1->a0 * (int64_t)n2->a0;
	int64_t c1 = n1->a0*(int64_t)n2->a1;
	c1 += n1->a1*(int64_t)n2->a0;
	if (c0 >= ONEMILL)
	{
		c1 += c0 / ONEMILL;
		c0 %= ONEMILL;
	}
	if (c1 >= ONEMILL)
		c1 %= ONEMILL;

	n1->a1 = (int)c1;
	n1->a0 = (int)c0;
}



twelvedigits modpow(twelvedigits *base, int exp)
{
	twelvedigits result = { 0, 1 };
	for (;;)
	{
		if (exp & 1)
			mult(&result, base);

		exp >>= 1;
		if (!exp)
			break;

		mult(base, base);
	}

	return result;
}

void settwelvedigits(twelvedigits *td, int v)
{
	if (v >= ONEMILL)
	{
		td->a0 = v%ONEMILL;
		td->a1 = v / ONEMILL;
	}
	else
	{
		td->a0 = v;
		td->a1 = 0;
	}
}

int main()
{
	int a, b, c, d;
	twelvedigits td1, td2;
	twelvedigits tdacc = { 0, 0 };

    //a*b^c + d
	a = 28433;
    b = 2;
    c = 7830457;
    d = 1;

    //td1 = b
    settwelvedigits(&td1, b);

    //td1 = b^c
    td1 = modpow(&td1, c);

    //td2 = a;
    settwelvedigits(&td2, a);

    //td1 *= td2
    mult(&td1, &td2);

    //td2 = d
    settwelvedigits(&td2, d);

    //td1 += td2
    add(&td1, &td2);

    //tdacc += td1
    add(&tdacc, &td1);
	


	//print solution
	printf("%06d%06d\n", tdacc.a1, tdacc.a0);

	return 0;
}
