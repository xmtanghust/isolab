/// @file     segmented_sieve.cpp
/// @author   Kim Walisch, <kim.walisch@gmail.com> 
/// @brief    This is a simple implementation of the segmented sieve of
///           Eratosthenes with a few optimizations. It generates the
///           primes below 10^9 in 0.9 seconds (single-threaded) on an
///           Intel Core i7-4770 CPU (3.4 GHz) from 2013.
/// @license  Public domain.
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <stdint.h>
#include <time.h>

/// Set your CPU's L1 data cache size (in bytes) here
const int L1D_CACHE_SIZE = 32768;

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit  Sieve primes <= limit.
///
void segmented_sieve(int *prime, int64_t limit)
{
  int sqrt = (int) std::sqrt(limit);
  int segment_size = std::max(sqrt, L1D_CACHE_SIZE);

  int64_t count = (limit < 2) ? 0 : 1;
  int64_t s = 3;
  int64_t n = 3;

  // generate small primes <= sqrt
  std::vector<char> is_prime(sqrt + 1, 1);
  
  for (int i = 2; i * i <= sqrt; i++)
    if (is_prime[i])
      for (int j = i * i; j <= sqrt; j += i)
        is_prime[j] = 0;
 
  // vector used for sieving
  std::vector<char> sieve(segment_size);
  std::vector<int> primes;
  std::vector<int> next;

  for (int64_t low = 0; low <= limit; low += segment_size)
  {
    std::fill(sieve.begin(), sieve.end(), 1);

    // current segment = interval [low, high]
    int64_t high = std::min(low + segment_size - 1, limit);
	int64_t s2;
    // add new sieving primes <= sqrt(high)
    for (; (s2=s*s) <= high; s += 2)
    {
      if (is_prime[s])
      {
        primes.push_back((int) s);//primes up to sqrt(high)
        next.push_back((int)(s2 - low));//relative next check point in sieve for s
      }
    }

    // sieve the current segment
    for (std::size_t i = 0; i < primes.size(); i++)
    {
      int j = next[i];
      for (int k = (primes[i] <<1); j < segment_size; j += k)//why not j+primes[i]? even number ,directly ignored
        sieve[j] = 0;
      next[i] = j - segment_size;//change for old primes:relative next check point in sieve for s
    }
	
	for (; n <= high;n+=2)
	{
		if (sieve[n - low]) // n is a prime
		{
			count++;
			prime[count] = n;
		}
	}
  }

  //std::cout << count << " primes found." << std::endl;
  prime[0] = count;
  prime[1] = 2;
  
}

/// Usage: ./segmented_sieve n
/// @param n  Sieve the primes up to n.
///
///

int main() {
    clock_t t1, t2;
    int count=0;

    t1 = clock();
    int *primes=(int *)calloc(100000000+5,sizeof(int));
    segmented_sieve(primes,100000000);
    count = primes[0];
    t2 = clock();
    printf("total %u primes,time=%u ms\n", count,t2 - t1);
    for (int i = 1; i < 100; i++)
            printf("prime[%u]=%u ", i, primes[i]);
    free(primes);
    getchar();
    return 0;
}
