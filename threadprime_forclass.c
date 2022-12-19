//this will call four threads, each of which will find prime numbers in a range
//once all the primes are found, it will print them all out

#include <pthread.h>
#include <stdio.h>

//constants: range of numbers to test if prime
#define START 1000000
#define END 1000100
#define RANGE END-START

//when primes are found they're put in primelist
//primecount will tell how many there are
int primelist[END-START];
int primecount=0;

//returns 1 if num is prime, 0 otherwise
int isprime(int num)
{
	int i;
	for(i=2; i<num-1; i++)
		if(num%i==0)
			return 0;
	return 1;
}

//you'll call this to slow things down
void delay()
{
	int i;
	for(i=0; i<100000; i++) { }
}

//this function is run several times by separate threads
void* findprimes(int start,int end,int threadnumber)
{
	int p;

	printf("Thread %d is about to find primes from %d to %d\n",threadnumber,start,end);

	//go through the range of numbers
	for(p=start; p<end; p++)
	{
		//if number p is prime, then...
		if(isprime(p)==1)
		{
			//put it into the global array
			primelist[primecount]=p;
			primecount++;
		}
	}

	printf("Thread %d is done.\n",threadnumber);
}

//puts the prime list in order
void sort()
{
	int i,j,value;
	for(i=0; i<primecount; i++)
	{
		value=primelist[i];
		for(j=i; j>0 && value<primelist[j-1]; j--)
			primelist[j]=primelist[j-1];
		primelist[j]=value;
	}
}

void* threadfunc1()
{
	findprimes(START,START+RANGE,1);
}

void* threadfunc2()
{
	findprimes(START+RANGE,START+RANGE+RANGE,2);
}

void* threadfunc3()
{
	findprimes(START+RANGE+RANGE,START+RANGE+RANGE+RANGE,3);
}

void* threadfunc4()
{
	findprimes(START+RANGE+RANGE+RANGE,START+RANGE+RANGE+RANGE+RANGE,4);
}

int main()
{

	//thread variables
	pthread_t thread1,thread2,thread3,thread4;

	//make the four threads
	pthread_create(&thread1,NULL,threadfunc1,NULL);
	pthread_create(&thread2,NULL,threadfunc2,NULL);
	pthread_create(&thread3,NULL,threadfunc3,NULL);
	pthread_create(&thread4,NULL,threadfunc4,NULL);


	//make a really really long delay for the threads to finish
//	int j;
//	for(j=0;j<300000000; j++) { }   //commenting this out virtually guarantees the race leads to at least one thread
                                    //finishing too soon, leading to it breaking

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

	//now that we've got the primes, sort them and print them out
	sort();

	int i;
	for(i=0; i<primecount; i++)
		printf("\t%d: %d\n",i,primelist[i]);

	printf("%d primes total\n",primecount);
}