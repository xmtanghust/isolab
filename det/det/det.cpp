// det.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <stdlib.h>
#define N 4
double mt[N][N] = { { 0, 2, 0.5, 7 }, { 0.7, 0, 1, 7 }, { 1.4, 0, 2, 7 }, { 1.4, 0, 3, 7 } };
void swap(int row1, int row2)
{
	double temp;
	for (int i = row1; i < N; i++)
	{
		temp = mt[row1][i];
		mt[row1][i] = mt[row2][i];
		mt[row2][i] = temp;
	}
}
double det(int col)
{
	double temp;
	if (col == N - 1)return mt[N - 1][N - 1];
	
	if (mt[col][col] != 0)
	{
		for (int i = col + 1; i < N; i++)
		{
			temp = mt[i][col]/ mt[col][col];
			for (int j = col; j < N; j++)
			{
				mt[i][j] -= mt[col][j] * temp;
			}
		}
		return mt[col][col] * det(col + 1);
	}
	else
	{
		for (int i = col + 1; i < N; i++)
		{
			if (mt[i][col] != 0)
			{
				swap(col, i);
				return -det(col);
			}
		}
		return 0;
	}

}
void SieveOfEratosthenes(char composite[],int len)
{
	int d1 = 8;
	int d2 = 8;
    int p1 = 3;
    int p2 = 7;
    int s = 7;
    int s2 = 3;
    int n = 0;
    bool toggle = false;
    while (s < len)             // --  scan the sieve
    {
        if (!composite[n++])    // --  if a prime is found
		{                       // --  cancel its multiples
		    int inc = p1 + p2;
			for (int k = s; k < len; k += inc)
			{
				 composite[k] = true;
			}
			for (int k = s + s2; k < len; k += inc)
			{
			   composite[k] = true;
			}
         }
		if (toggle = !toggle)
		{
			s += d2;
			d1 += 16;
			p1 += 2;
			p2 += 2;
			s2 = p2;
		}
		else
		{
			s += d1;
			d2 += 8;
			p1 += 2;
			p2 += 6;
			s2 = p1;
		}
     }
}

int MakePrimeList(int n,int primes[])
{
	char *composite = (char *)calloc(n / 3,1);
	SieveOfEratosthenes(composite,n/3);
	bool toggle = false;
	int p = 5, i = 0, j = 2;
	primes[0] = 2;
	primes[1] = 3;
	while (p <= n)
	{
		if (!composite[i++])
		{
			primes[j++] = p;
		}
		p += (toggle = !toggle) ? 2 : 4;
	}
	free(composite);
	return j; // number of primes
}

/*
总体原则是，如果m=ps是一个合数，p是m的最小素因子，那么m仅在primes[j]=p,i=m的时候被标记为合数
*/
const int MAXN = 100000000+1;
void getPrime(int *primes,int n)
{
	//memset(primes, 0, n*sizeof(int));
	int i,temp;
	for (i = 2; i <= n/2; i++) 
	{
		if (!primes[i])primes[++primes[0]] = i;//primes[0]表示已发现素数的总数，primes[i]逐步地保存第i个素数，例如primes[1]=2
		for (int j = 1; j <= primes[0]; j++) 
		{
			temp = primes[j] * i;
			if (temp > n)break;
			primes[temp] = 1;//将所有已确定的素数的i倍标记为合数
			if (i%primes[j] == 0) break;//如果i是某个素数的倍数,退出，保证只在最小素因子处被标记一次
		}
	}
	for (; i <= n; i++)
	{
		if (!primes[i])primes[++primes[0]] = i;//剩余的
	}

}
/*偶数不考虑*/
void getPrime2(int *primes, int n)
{
	//memset(primes, 0, n*sizeof(int));
	int i,temp;
	primes[0] = 1; 
	primes[1] = 2;
	for (i = 3; i <= n/3; i+=2) 
	{
		if (!primes[i])primes[++primes[0]] = i;
		for (int j = 2; j <= primes[0]; j++) 
		{
			temp = primes[j] * i;
			if (temp > n)break;
			primes[temp] = 1;
			if (i%primes[j] == 0) break;
		}
	}
	for (; i <= n / 3; i += 2)
	{
		if (!primes[i])primes[++primes[0]] = i;
	}
}
/*2和3的倍数不考虑*/
void getPrime3(int *primes, int n)
{
	//memset(primes, 0, n*sizeof(int));
	int i,temp,temp2;
	int flag,flag1,flag2;
	
	primes[0] = 2; 
	primes[1] = 2;
	primes[2] = 3;
	
	for (i = 5; i <= n/5; i += 6) 
	{
		if (!primes[i])primes[++primes[0]] = i;//6k-1
		if (!primes[i+2])primes[++primes[0]] = i+2;//6k+1
		flag1 = flag2 = 1;
		for (int j = 3; j <= primes[0]; j++) 
		{
			flag = i%primes[j];
			temp = primes[j] * i;//6k-1
			if (temp > n)break;
			if (flag1)
			{
				primes[temp] = 1;
				flag1 = (flag != 0);
			}
			if (flag2)
			{
				temp2 = temp + (primes[j] <<1);//6k+1
				if (temp2 <= n)
				{
					primes[temp2] = 1;
					flag2 = (flag + 2 != primes[j]);
				}
				else
					flag2 = 0;
			}
				
			if(!(flag1 || flag2))break;
		}
	}
	for (; i <= n; i += 6)
	{
		if (!primes[i])primes[++primes[0]] = i;//6k-1
		if (!primes[i + 2])primes[++primes[0]] = i + 2;//6k+1
	}
}
/*2和3的倍数不考虑,节省空间*/
void getPrime4(int *primes, int n)
{
	//memset(primes, 0, n*sizeof(int));
	int i,k, temp, temp2;
	int flag, flag1, flag2;

	primes[0] = 2;
	primes[1] = 2;
	primes[2] = 3;
	/*primes[3] = 5;
	primes[4] = 7;
	primes[5] = 11;
	primes[6] = 13;
	primes[7] = 17;
	primes[8] = 19;
	primes[9] = 23;
	primes[10] = 25;
	*/

	k = 3;
	for (i = 5; i <= n / 5; i += 6,k+=2)
	{
		if (!primes[k])primes[++primes[0]] = i;//6k-1
		if (!primes[k + 1])primes[++primes[0]] = i + 2;//6k+1
		flag1 = flag2 = 1;
		for (int j = 3; j <= primes[0]; j++)
		{
			flag = i%primes[j];
			temp = primes[j] * i;//6k-1
			if (temp > n)break;
			

			if (flag1)
			{
				primes[temp/3+2] = 1;
				flag1 = (flag != 0);
			}
			if (flag2)
			{
				temp2 = temp + (primes[j] << 1);//6k+1
				if (temp2 <= n)
				{
					primes[temp2/3+2] = 1;
					flag2 = (flag + 2 != primes[j]);
				}
				else
					flag2 = 0;
			}

			if (!(flag1 || flag2))break;
		}
	}
	for (; i <= n; i += 6)
	{
		if (!primes[k])primes[++primes[0]] = i;//6k-1
		if (!primes[k + 1])primes[++primes[0]] = i + 2;//6k+1
		k += 2;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	clock_t t1, t2;
	int count=0;
	int *primes=(int *)calloc(MAXN,sizeof(int));
	//printf("del=%lf\n", det(0));
	t1 = clock();
	//count=MakePrimeList(100000000,primes);
	getPrime4(primes, 100000000);
	count = primes[0];
	t2 = clock();
	printf("total %u primes,time=%u ms\n", count,t2 - t1);
	for (int i = 1; i < 100; i++)
		printf("prime[%u]=%u ", i, primes[i]);
	free(primes);
	getchar();
	return 0;
}

