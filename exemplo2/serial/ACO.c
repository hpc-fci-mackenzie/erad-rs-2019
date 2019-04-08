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
#ifdef TEST
#define size 5
int paths[][size] = { { 0, 11, 10, 12, 4 },
                      { 2,  0,  6,  3, 5 },
                      { 3, 12,  0, 14, 6 },
                      { 6, 14,  4,  0, 7 },
                      { 7,  9,  8, 12, 0 } };

#else
#define size 20
int paths[][size] = { { 0,648,2625,549,2185,1898,1458,1752,1963,427,1743,1817,1899,1060,1148,2084,732,1095,1725,2524 },
                      { 648,0,2363,481,2129,2030,1641,1594,1638,557,1214,1492,1710,1126,825,1861,811,1195,1375,2262 },
                      { 2625,2362,0,1965,669,1274,1541,920,744,2172,1623,875,720,1595,3085,543,3113,1734,1111,103 },
                      { 549,481,1965,0,1667,1605,1194,1132,1242,431,963,1096,1280,664,1249,1464,1276,799,979,1866 },
                      { 2185,2129,669,1667,0,621,906,541,643,1733,1504,733,459,1187,2880,479,2791,1169,932,566 },
                      { 1898,2030,1274,1605,621,0,443,662,978,1482,1669,925,839,1007,2855,929,2541,843,1107,1172 },
                      { 1458,1641,1541,1194,906,443,0,754,1106,1074,1447,976,968,638,2477,1148,2132,435,1027,1442 },
                      { 1752,1594,920,1132,541,662,754,0,347,1293,1015,261,209,724,2319,389,2346,841,443,818 },
                      { 1963,1638,744,1242,643,978,1106,347,0,1511,961,170,183,970,2361,287,2389,1124,388,688 },
                      { 427,557,2172,431,1733,1482,1074,1293,1511,0,1318,1363,1447,585,1378,1631,1063,641,1275,2071 },
                      { 1743,1214,1623,963,1504,1669,1447,1015,961,1318,0,813,1078,918,1571,1182,1882,1147,582,1583 },
                      { 1817,1492,875,1096,733,925,976,261,170,1363,813,0,271,826,2215,373,2242,979,241,774 },
                      { 1899,1710,720,1280,459,839,968,209,183,1447,1078,271,0,882,2453,192,2408,1049,504,621 },
                      { 1060,1126,1595,664,1187,1007,638,724,970,585,918,826,882,0,1891,1066,1667,251,798,1495 },
                      { 1148,825,3085,1249,2880,2855,2477,2319,2361,1378,1571,2215,2453,1891,0,2583,559,2042,2108,2984 },
                      { 2084,1861,543,1464,479,929,1148,389,287,1631,1182,373,192,1066,2583,0,2612,1220,611,442 },
                      { 732,811,3113,1276,2791,2541,2132,2346,2389,1063,1882,2242,2408,1667,559,2612,0,1701,2125,3012 },
                      { 1095,1195,1734,799,1169,843,435,841,1124,641,1147,979,1049,251,2042,1220,1701,0,977,1634 },
                      { 1725,1375,1111,979,932,1107,1027,443,388,1275,582,241,504,798,2108,611,2125,977,0,1011 },
                      { 2524,2262,103,1866,566,1172,1442,818,688,2071,1583,774,621,1495,2984,442,3012,1634,1011,0 } };
#endif

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

void ACO(int max_it, float p, float Q, int paths[][size], int N)
{
  int M = size;
  float t[M][M];
  int ants[N];
  float best_paths[N];
  int ant_paths[N][M+1];
  int best_ant_paths[N][M+1];
  float current_path[N];
  float probability[M];
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
    fprintf(output, "Minimo: %d:%.2f\nPath: ", min, best_paths[min]);
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
  fprintf(output, "Minimo: %d:%.2f\nPath: ", min, best_paths[min]);
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

  ACO(max_it, p, Q, paths, N);

  return EXIT_SUCCESS;
}

