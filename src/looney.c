#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 80 characters should be enough for a single PAN */
#define MAXBUFFERSIZE 1001
#define MINCARDLENGTH 14
#define MAXCARDLENGTH 16

int valid_checksum(char* tmp_input);
void mask_output(int pan_start, int pan_length);
void clean_input();
void detect_pan();
void reset_state();

char* input;
char* output;
int input_length = 0;
int possible_card = 0;

int main(int argc, char* argv[])
{
  char ch; /* read one input character at a time */
  char in [MAXBUFFERSIZE]; /* initialize an empty "string" */
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
    while( (ch != '\n') && (char_count < MAXBUFFERSIZE) ) {
      in[char_count++] = ch;
      ch = getchar();
    }
    in[char_count] = 0x00;
    
    int len = strlen(in);
    
    free(input);
    input = malloc(len * sizeof(char));
    output = malloc(len * sizeof(char));
    int i;
    for (i = 0; i < len; i++) {
      input[i] = in[i];
      output[i] = in[i];
    }
    input_length = char_count;
    
    input[input_length] = 0x00;
    output[input_length] = 0x00;
    
    if (input_length >= MINCARDLENGTH) {
      possible_card = 1;
    }
    
    if (possible_card == 1) {
      detect_pan();
    }
    
    printf("%s\n", output);
  
    fflush(stdout);
  }
  
  free(input);
  free(output);
  return 0;
}

void detect_pan() {
  int valid = 1; 
  
  int test_len;
  for (test_len = 16; test_len >= MINCARDLENGTH; test_len--) {
    
    int initial;
    for (initial = 0; initial <= (input_length - test_len); initial++) {
      
      char substr [input_length];
      int substring_index = 0;
      int input_index = initial;
      int char_count = 0;
      while (char_count < test_len) {
        if (input_index < (int)strlen(input)) {
          char ch = input[input_index++];
          substr[substring_index++] = ch;
          if (ch >= '0' && ch <= '9') {
            // printf("%c -- %i\n", ch, char_count);
            char_count++;
          }
        } else {
          break;
        }
      }
      substr[substring_index] = 0x00;
      
      // printf("%s -- %i -- %i -- %c -- %i\n", substr, (int)strlen(substr), test_len, substr[test_len - 1], initial);
      
      valid = valid_checksum(substr);
      if (valid == 0) {
        mask_output(initial, (int)strlen(substr));
      }
    }
  }
}

int valid_checksum(char* tmp_input)
{
  int len = strlen(tmp_input);
  int total = 0;
  int int_count = 0;
    
  int i;
  int odd = 0;
  for (i = (len - 1); i >= 0; i--) {
    int x = tmp_input[i] - '0';
        
    if (x >= 0 && x <= 9) {
      if (odd == 1) {
        x = x * 2;
        if (x >= 10) {
          x = (x % 10) + (x / 10);
        }
      }
      
      total += x;
      int_count++;
      
      if (odd == 0) {
        odd = 1;
      } else {
        odd = 0;
      }
    }
  }
  
  if (int_count < MINCARDLENGTH) {
    return 1;
  } else {
    return (total % 10);
  }
}

void mask_output(int pan_start, int pan_length)
{
  char ch = 'X';
  int i = 0;
  
  // printf("%i\n", pan_start);
  
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