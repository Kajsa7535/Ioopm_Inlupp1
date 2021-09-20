#include<stdio.h>
#include<stdlib.h>


// ANVÄNDER SIG AV HEAPEN
/*
int *heap_int(int i)
{
  int *result = o
  *result = i;
  return result;
}

int *fib(int *n)
{
  if (*n == 0)
    {
      return heap_int(0);
    }
  else
    {
      if (*n == 1)
  {
    return heap_int(1);
  }
      else
  {
    return heap_int(*fib(heap_int(*n-1)) + *fib(heap_int(*n-2)));
  }
    }
}*/


// ANVÄNDER SIG AV STACKEN
int fib(int num)
{
    if (num == 0)
    {
        return 0;
    }
    else if (num == 1 )
    {
        return 1; 
    }
    else
    {
        int result = fib(num-1) + fib(num-2);
        return result;
    }
}


// OBSERVERA!
// Detta program läcker minne som ett såll -- det frigör inte några allokerade pekare
int main(int argc, char *argv[])
{
  int arr[4] = {1,2,3,4};
  if (argc < 2)
    {
      puts("Usage: ./fib-rec 5");
    }
  else
    {
      // STACK  
      int n = atoi(argv[1]);
      printf("fib(%d) = %d\n", n, fib(n));
      int test1 = arr[2];
      int test2 = 2[arr];
      printf("1:(%d), 2:(%d)", test1, test2);
    // HEAP
    // int *n = heap_int(atoi(*(argv+1)));
     //printf("fib(%d) = %d\n", *n, *fib(n));
    }
  return 0;
}