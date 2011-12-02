#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 80 characters should be enough for a single PAN */
#define MAXBUFFERSIZE 80
#define MINCARDLENGTH 14
#define MAXCARDLENGTH 16

int valid_checksum(char* tmp_input);
void mask_output();
void clean_input();
void detect_pan();
void reset_state();

char* input;
int input_length = 0;
int possible_card = 0;

int pan_start = 0;
int pan_length = 0;
int pan_detected = 0;

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
    int i;
    for (i = 0; i < len; i++) {
      input[i] = in[i];
    }
    input_length = char_count;
    
    if (input_length >= MINCARDLENGTH) {
      possible_card = 1;
    }
    
    if (possible_card == 1) {
      detect_pan();
      mask_output();
    }
    
    if (pan_detected == 1) {
      printf("%s\n", input);
    } else {
      printf("%s\n", in);
    }
  
    fflush(stdout);
  }
  
  free(input);
  return 0;
}

void detect_pan() {
  int valid = 1;
  
  int test_len;
  for (test_len = 16; test_len >= MINCARDLENGTH; test_len--) {
    if (pan_detected == 1) {
      break;
    }
    
    int initial;
    for (initial = input_length - test_len; initial >= 0 ; initial--) {
      if (pan_detected == 1) {
        break;
      }
      
      char substr [test_len];
      int substring_index = 0;
      int input_index = initial;
      int char_count = 0;
      while (char_count < test_len) {
        char ch = input[input_index++];
        substr[substring_index++] = ch;
        if (ch >= '0' && ch <= '9') {
          char_count++;
        }
      }
      substr[test_len] = 0x00;
      
      // printf("%s -- %i -- %i -- %c\n", substr, (int)strlen(substr), test_len, substr[test_len - 1]);
      
      valid = valid_checksum(substr);
      if (valid == 0) {
        pan_detected = 1;
        pan_start = initial;
        pan_length = (int)strlen(substr);
      }
    }
  }
}

int valid_checksum(char* tmp_input)
{
  int len = strlen(tmp_input);
  int total = 0;
    
  int i;
  int odd = 0;
  for (i = (len - 1); i >= 0; i--) {
    int x = tmp_input[i] - '0';
    
    if (x >= '0' || x <= '9') {
      if (odd == 1) {
        x = x * 2;
        if (x >= 10) {
          x = (x % 10) + (x / 10);
        }
      }

      total += x;

      if (odd == 0) {
        odd = 1;
      } else {
        odd = 0;
      }
    }
  }
  
  return (total % 10);
}

void mask_output()
{
  char ch = 'X';
  int i = 0;
  
  if (pan_detected == 1) {
    int num_chars_xed = 0;
    
    while (i < input_length) {
      if (num_chars_xed < pan_length && i >= pan_start && input[i] != ' ' && input[i] != '-') {
        input[i] = ch;
        num_chars_xed++;
      }
      i++;
    }
    
    input[input_length] = 0x00;
  }
}

void reset_state() {
  possible_card = 0;
  pan_detected = 0;
  pan_start = 0;
  pan_length = 0;
  input_length = 0;
}