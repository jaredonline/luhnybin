#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 1000 characters should be enough for a single PAN */
#define MAXBUFFERSIZE 1000
#define MINCARDLENGTH 14
#define MAXCARDLENGTH 16

int char_count;
char* input;
char* output;
char ch;

typedef struct {
  int start;
  int type;
  int count;
  int total;
  int skip;
} pan_tracker;

pan_tracker pans [MAXBUFFERSIZE * 3];
int pan_tracker_count;

int main(int argc, char* argv[])
{    
  char_count = pan_tracker_count = 0;
  
  input = malloc(MAXBUFFERSIZE * sizeof(char));
  output = malloc(MAXBUFFERSIZE * sizeof(char));
  
  while( (ch = getchar()) != EOF && char_count <= MAXBUFFERSIZE) {
    
    if (ch == '\n') {
      output[char_count] = 0x00;
      int i;
      for (i = 0; i < char_count; i++) putchar(output[i]);
      printf("\n");
      fflush(stdout);
      
      for (i = 0; i < pan_tracker_count; i++) {
        pan_tracker pan = {0, 0, 0, 1, 1};
        pans[i] = pan;
      }
      
      input = malloc(MAXBUFFERSIZE * sizeof(char));
      output = malloc(MAXBUFFERSIZE * sizeof(char));
      char_count = pan_tracker_count = 0;
      continue;
    }  
    
    input[char_count] = output[char_count] = ch;
    
    int type;
    for (type = MINCARDLENGTH; type <= MAXCARDLENGTH; type++) {
      pan_tracker pan = {char_count, type, 0, 0, 0};
      pans[pan_tracker_count++] = pan;
    }
    
    int x = ch - '0';
    int i;
    for (i = 0; i < pan_tracker_count; i++) {
      if (x >= 0 && x <= 9) {
        pans[i].total += ((pans[i].count % 2) == (pans[i].type == 15 ? 1 : 0)) ? ((x * 2) % 10) + ((x * 2) / 10): x;
        pans[i].count++;
      }
      
      if (pans[i].count == pans[i].type && pans[i].total % 10 == 0 && pans[i].skip == 0) {
        int a = pans[i].start;
        int x_count = 0;
        while (x_count < pans[i].type) {
          if (input[a] >= '0' && input[a] <= '9') {
            output[a++] = 'X';
            x_count++;
          } else {
            a++;
          }
          pans[i].skip = 1;
        }
      }
    }
    
    char_count++;  
  }
  
  return 0;
}