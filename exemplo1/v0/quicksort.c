int partition(int* A, int p, int q) {
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

  return pivot; 
}
 
void quicksort(int* A, int p, int q) {
  if(p < q) {
    int pivot = partition(A, p, q);
 
    quicksort(A, p, pivot - 1);
    quicksort(A, pivot + 1, q);
  }
}
