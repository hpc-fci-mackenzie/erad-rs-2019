#include <stdlib.h>
#include <stdio.h>

int partition(int* A, int len)
{
  int pivot = A[len / 2];
 
  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (A[i] < pivot) i++;
    while (A[j] > pivot) j--;
 
    if (i >= j) break;
 
    int temp = A[i];
    A[i]     = A[j];
    A[j]     = temp;
  }

  return i;
}
 
void quicksort(int* A, int len)
{
  if (len < 2) return;

  int pivot = partition(A, len);

  quicksort(A, pivot);
  quicksort(A + pivot, len - pivot);
 
}

int main (void)
{
  int *a;
  int n;
 
  int i;

  fscanf(stdin, "%d", &n);
  a = (int*) malloc(sizeof(int) * n);
  for (i = 0; i < n; i++) {
    fscanf(stdin, "%d", a+i);
  }
 
  quicksort(a, n);
 
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
 
  return 0;
}

