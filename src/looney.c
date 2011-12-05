#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 1000 characters should be enough for a single PAN */
#define MAXBUFFERSIZE 1000
#define MINCARDLENGTH 14
#define MAXCARDLENGTH 16

char input [MAXBUFFERSIZE];
char output [MAXBUFFERSIZE];
char ch;

/*
  the pan_tracker type keeps all the information for a single possible PAN:
    start: the index of input that this possible PAN starts at
    type: the type of PAN (14, 15 or 16-digit PAN)
    count: how many digits this pan_tracker has accumulated
    total: the current running total according to the Luhn check
    skip: already ID'ed as a PAN (or not a PAN) so skip it in all actions
*/
typedef struct {
  int start;
  int type;
  int count;
  int total;
  int skip;
} pan_tracker;

pan_tracker pans [MAXBUFFERSIZE * 3];
int pan_tracker_count, char_count;

int main(int argc, char* argv[])
{    
  char_count = pan_tracker_count = 0;
  
  while( (ch = getchar()) != EOF && char_count <= MAXBUFFERSIZE) {
    
    /*
      reset the state on a new line:
       - (set a NULL character for output) and print the output string
       - set all PAN trackers to skip
       - reset counters
    */
    if (ch == '\n') {
      output[char_count] = 0x00;
      printf("%s\n", output);
      fflush(stdout);
      
      int i;
      for (i = 0; i < pan_tracker_count; i++) {
        pans[i].skip = 1;
      }
      
      char_count = pan_tracker_count = 0;
      continue;
    }  
    
    // add new character to the buffers
    input[char_count] = output[char_count] = ch;
    
    // setup the new pan_trackers; one for each type
    int type;
    for (type = MINCARDLENGTH; type <= MAXCARDLENGTH; type++) {
      pan_tracker pan = {char_count, type, 0, 0, 0};
      pans[pan_tracker_count++] = pan;
    }
    
    /*
      update the state of each PAN tracker and mask the
      output if a PAN is detected.  If a PAN tracker goes
      over it's designated length, set it to skip (it's no
      longer a possible PAN).
    */
    int x = ch - '0';
    int i;
    for (i = 0; i < pan_tracker_count; i++) {
      if (x >= 0 && x <= 9 && pans[i].skip == 0) {
        pans[i].total += ((pans[i].count++ % 2) == (pans[i].type == 15 ? 1 : 0)) ? ((x * 2) % 10) + ((x * 2) / 10): x;
        
        if (pans[i].count == pans[i].type && pans[i].total % 10 == 0) {
          int a = pans[i].start;
          int x_count = 0;
          while (x_count < pans[i].type) {
            if (input[a] >= '0' && input[a] <= '9') {
              output[a] = 'X';
              x_count++;
            } 
            a++;
            pans[i].skip = 1;
          }
        } else if (pans[i].count > pans[i].type) {
          pans[i].skip = 1;
        }
        
      }
    }
    
    char_count++;  
  }
  
  return 0;
}