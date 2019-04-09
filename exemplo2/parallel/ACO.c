#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define output stdout

#ifdef DEBUG
void print_v_f(int n, float vetor[n])
{
  for(int i=0;i<n;i++) {
    fprintf(output, "%.2f ", vetor[i]);
  }
  fprintf(output, "\n");
}

void print_v_i(int n, int vetor[n])
{
  for(int i=0;i<n;i++) {
    fprintf(output, "%d ", vetor[i]);
  }
  fprintf(output, "\n");
}

void print_m_f(int n, int m, float vetor[][n])
{
  for(int i=0;i<n;i++) {
    for(int j=0;j<m;j++) {
      fprintf(output, "%.2f ", vetor[i][j]);
    }
    fprintf(output, "\n");
  }
}

void print_m_i(int n, int m, int vetor[][n])
{
  for(int i=0;i<n;i++) {
    for(int j=0;j<m;j++) {
      fprintf(output, "%d ", vetor[i][j]);
    }
    fprintf(output, "\n");
  }
}
#endif

// Matriz de caminhos
int size = 0;
int **paths;

void alloc_read_paths()
{
  int i,j;
  fscanf(stdin, "%d", &size);
  paths = (int**) malloc(sizeof(int*)*size);
  if(paths==NULL) exit(1);
  for(i=0;i<size;i++) {
    paths[i] = (int*)malloc(sizeof(int)*size);
    if(paths[i]==NULL) exit(1);
    for(j=0;j<size;j++) {
      fscanf(stdin, "%d", &paths[i][j]);
    }
  }
}

void free_paths()
{
  int i;
  for(i=0;i<size;i++) {
    free(paths[i]);
  }
  free(paths);
}

float* alloc_f_v(int size)
{
  float *mem;
  mem = (float*)calloc(size, sizeof(float));
  if(mem == NULL) exit(1);
  return mem;
}

int* alloc_i_v(int size)
{
  int *mem;
  mem = (int*)calloc(size, sizeof(int));
  if(mem == NULL) exit(1);
  return mem;
}

float **alloc_f_m(int size)
{
  float **mem;
  int i;
  mem = (float**)calloc(size, sizeof(float**));
  if(mem == NULL) exit(1);
  for(i=0;i<size;i++) {
    mem[i] = (float*)calloc(size, sizeof(float));
    if(mem[i]==NULL) exit(1);
  }
  return mem;
}

void free_f_m(float **mem, int size)
{
  int i;
  for(i=0;i<size;i++) {
    free(mem[i]);
  }
  free(mem);
}

int **alloc_i_m(int l, int c)
{
  int **mem;
  int i;
  mem = (int**)calloc(l, sizeof(int**));
  if(mem == NULL) exit(1);
  for(i=0;i<l;i++) {
    mem[i] = (int*)calloc(c, sizeof(int));
    if(mem[i]==NULL) exit(1);
  }
  return mem;
}

void free_i_m(int **mem, int size)
{
  int i;
  for(i=0;i<size;i++) {
    free(mem[i]);
  }
  free(mem);
}

int my_choice(int m, float probability[m], float total)
{
  float n = ((float)rand()/(float)(RAND_MAX)) * total;
  float sum=0;
  for(int i=0;i<m;i++) {
    if(n < sum+probability[i]) {
      return i;
    }
    sum += probability[i];
  }
  return m-1;
}

void ACO(int max_it, float p, float Q, int **paths, int N)
{
  int M = size;
  float** t = alloc_f_m(M);
  int* ants = alloc_i_v(N);
  float* best_paths = alloc_f_v(N);
  int** ant_paths = alloc_i_m(N, M+1);
  int** best_ant_paths = alloc_i_m(N, M+1);
  float* current_path = alloc_f_v(N);
  float* probability = alloc_f_v(M);
  int current_node, next_node;

  for(int i=0;i<M;i++) {
    for(int j=0;j<M;j++) {
      t[i][j] = 1.0;
    }
    t[i][i] = 0;
  }

  srand(M);
  for(int i=0;i<N;i++) {
    ants[i] = rand()%M;
  }

  for(int i=0;i<N;i++) {
    best_paths[i] = INFINITY;
  }

  for(int i=0;i<N;i++) {
    for(int j=0;j<M+1;j++) {
      ant_paths[i][j]=0;
      best_ant_paths[i][j]=0;
    }
  }

  for(int it=0;it<max_it;it++) {
    for(int i=0;i<N;i++) {
      current_path[i]=0;
    }
    for(int i=0;i<N;i++) {
      ant_paths[i][0] = ants[i];
      current_node = ants[i];

      int e = 1;
      while(e < M) {
        float this_t = 0;
        for(int j=0;j<M;j++) {
          this_t += t[current_node][j];
        }
        for(int j=0;j<M;j++) {
          probability[j] = t[current_node][j] / this_t;
        }
        next_node = my_choice(M, probability, this_t);
        int found = 0;
        for(int j=0;j<e && !found;j++) {
          if(ant_paths[i][j] == next_node) {
            found=1;
          }
        }
        if(!found) {
          ant_paths[i][e] = next_node;
          e++;
          current_node = next_node;
        }
      }
      ant_paths[i][M] = ant_paths[i][0];
      for(int j=0;j<M;j++) {
        current_node = ant_paths[i][j];
        next_node = ant_paths[i][j+1];
        current_path[i] += paths[current_node][next_node];
      }
    }

    for(int i=0;i<M;i++) {
      for(int j=0;j<M;j++) {
        t[i][j] *= (1.0-p);
      }
    }
    for(int i=0;i<N;i++) {
      if(current_path[i] < best_paths[i]) {
        best_paths[i] = current_path[i];
        for(int j=0;j<M+1;j++) {
          best_ant_paths[i][j] = ant_paths[i][j];
        }
        for(int j=0;j<M-1;j++) {
          current_node = ant_paths[i][j];
          next_node = ant_paths[i][j+1];
          t[current_node][next_node] += Q/paths[current_node][next_node];
        }
      }
    }

#ifdef DEBUG
    fprintf(stdout, "It:%d\n", it);
    int min=0;
    for(int i=1;i<N;i++) {
      if(best_paths[i]<best_paths[min]) {
        min=i;
      }
    }
    fprintf(output, "Minimo: [%d]%.2f\nPath: ", min, best_paths[min]);
    for(int i=0;i<M;i++) {
      fprintf(output, "%d ", best_ant_paths[min][i]);
    }
    fprintf(output, "\n");
#endif
  }

  int min=0;
  for(int i=1;i<N;i++) {
    if(best_paths[i]<best_paths[min]) {
      min=i;
    }
  }
  fprintf(output, "Minimo: [%d]%.2f\nPath: ", min, best_paths[min]);
  for(int i=0;i<M;i++) {
    fprintf(output, "%d ", best_ant_paths[min][i]);
  }
  fprintf(output, "\n");
}

int main(int argc, char* argv[])
{
  // iteracoes
  int max_it = 10;
  // numero de ants
  int N = 1000;
  // probabilidade de algum caminho
  float p = 0.1;
  // um parametro arbitrario definido pelo usuario
  int Q = 10;

  alloc_read_paths();

  ACO(max_it, p, Q, paths, N);

  free_paths();

  return EXIT_SUCCESS;
}

