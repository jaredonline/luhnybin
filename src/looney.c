#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 1000 characters should be enough for a single PAN */
#define MAXBUFFERSIZE 1000
#define MINCARDLENGTH 14
#define MAXCARDLENGTH 16

void mask_output(int pan_start, int pan_length);
void detect_pan();
void reset_state();

char input [MAXBUFFERSIZE];
char output [MAXBUFFERSIZE];
int input_length = 0;
int possible_card = 0;

int main(int argc, char* argv[])
{
  char ch; /* read one input character at a time */
  int char_count; /* our character counter */
  
  /* loop until the test suite force quits us
    Grap each single character input and check it's not a new line, then add it 
    to the string.
    
    Terminate the string in a null character.
    
    Pass the string to the valid_checksum method to see if it's valid, if it is valid, X it out.
  */
  while(ch != EOF) {
    reset_state();
    
    ch = getchar();
    char_count = 0;
    while( (ch != '\n' && ch != EOF) && (char_count < MAXBUFFERSIZE) ) {
      input[char_count] = ch;
      output[char_count] = ch;
      char_count++;
      ch = getchar();
    }
    input[char_count] = 0x00;
    output[char_count] = 0x00;
    input_length = char_count;
    
    if (input_length >= MINCARDLENGTH) {
      possible_card = 1;
    }
    
    if (possible_card == 1) {
      detect_pan();
    }
    
    printf("%s\n", output);
  
    fflush(stdout);
  }
  
  return 0;
}

void detect_pan() {
  int valid = 1; 
  
  int test_len;
  for (test_len = 16; test_len >= MINCARDLENGTH; test_len--) {
    
    int initial;
    for (initial = 0; initial <= (input_length - test_len); initial++) {
      int odd = test_len % 2;
      int total = 0;
      
      int substring_index = 0;
      int input_index = initial;
      int char_count = 0;
      
      while (char_count < test_len) {
        if (input_index < (int)strlen(input)) {
          char ch = input[input_index++];
          substring_index++;
          if (ch >= '0' && ch <= '9') {
            int x = ch - '0';
            
            if (odd == 0) {
              x = x * 2;
              if (x >= 10) {
                x = (x % 10) + (x / 10);
              }
            }
            
            total += x;
            
            if (odd == 1) {
              odd = 0;
            } else {
              odd = 1;
            }
            
            char_count++;
            valid = (char_count < MINCARDLENGTH || char_count > MAXCARDLENGTH) ? 1 : total % 10;
          }
        } else {
          break;
        }
      }
      
      if (valid == 0) {
        mask_output(initial, substring_index);
      }
    }
  }
}

void mask_output(int pan_start, int pan_length)
{
  char ch = 'X';
  int i = 0;
  
  int num_chars_xed = 0;
  
  while (i < input_length) {
    if (num_chars_xed < pan_length && i >= pan_start && (input[i] >= '0' && input[i] <= '9')) {
      output[i] = ch;
      num_chars_xed++;
    }
    i++;
  }
  
  output[input_length] = 0x00;
}

void reset_state() {
  possible_card = 0;
  input_length = 0;
}