#ifndef _DATA_IO_H_
#define _DATA_IO_H_

int get_n_lines(char *FileName)
{
  char temp[64], cmd[512];
  FILE *in;
  strcpy(cmd, "grep -c \">\" ");
  strcat(cmd, FileName);
  in = popen(cmd, "r");
  fgets(temp, 64, in);
  fclose(in);
  return atoi(temp);
}

float* read_csv(char *filename, double **data){
  FILE *file;
  float *items;
  int i = 0, n = 2048, n_items;
  char line[n];

  n_items = get_n_lines(filename);
  items = (float*)malloc(sizeof(float) * n_items);

  file = fopen(filename, "r");
  while (fgets(line, n, file))
  {
    char* tmp = strdup(line);
    int j = 0;
    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, "\n"))
    {
      item[i] = atof(tok);
      printf("%f\t", data[i][j]);
    }
    printf("\n");
    free(tmp);
  }
  fclose(file);
}

#endif
