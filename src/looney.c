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
  while(1) {
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
      clean_input();
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
  int i;  
  
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
      for (i = initial; i < input_length; i++) {
        substr[(i - initial)] = input[i];
      }
      substr[test_len] = 0x00;
      
      valid = valid_checksum(substr);
      if (valid == 0) {
        pan_detected = 1;
        pan_start = initial;
        pan_length = test_len;
      }
    }
  }
}

int valid_checksum(char* tmp_input)
{
  int len = strlen(tmp_input);
  int total = 0;
    
  if (len >= MINCARDLENGTH && len <= MAXCARDLENGTH) {
    int i;
    int odd = 0;
    for (i = (len - 1); i >= 0; i--) {
      int x = tmp_input[i] - '0';
  
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
  } else {
    total = 11;
  }
  
  return (total % 10);
}

void clean_input()
{
  int char_count = 0;
  char clean_input[input_length];
  
  // build a new array with only clean characters in it
  int i;
  for (i = 0; i < input_length; i++) {
    if (input[i] != '-' && input[i] != ' ') {
      clean_input[char_count] = input[i];
      char_count++;
    }
  }
  
  // release the old input and make a new one that is only as long as we need
  // and transfer the contents of clean_input to the input var
  free(input);
  input = malloc(char_count * sizeof(char));
  for (i = 0; i < char_count; i++) {
    input[i] = clean_input[i];
  }
  // input_length may have changed, so reset it here.
  input_length = char_count;
}

void mask_output()
{
  char ch = 'X';
  int i;
  
  // printf("%i\n%i\n%i\n", pan_detected, pan_start, pan_length);
  
  if (pan_detected == 1) {
    for (i = 0; i < input_length; i++) {
      if (i >= pan_start && i < (pan_length + pan_start)) {
        input[i] = ch;
      } 
    }
  }
}

void reset_state() {
  possible_card = 0;
  pan_detected = 0;
  pan_start = 0;
  pan_length = 0;
  input_length = 0;
}