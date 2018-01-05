#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAX_BUF_LEN 327670
char buffer1[MAX_BUF_LEN], subject1[1024];
char buffer2[MAX_BUF_LEN], subject2[1024];

int   get_subject(char *src, char *subject);

int main(int argc, char *argv[] )
{
  FILE *input, *output;
  // char *desc_ptr;
  int   counter, merge_count, reorder_count;
  int   len_subject;
  int   cmp;

  if (argc < 3) {
    printf("\t\tCheck & Sort Duplicated Dictionary Data.\n\t\t\t-by ccash.\n\nUSAGE: check_dup [clipped-html-file] [target-file]\n\n");
    return -1;
  }

  input = fopen(argv[1], "rt");
  if (input==NULL) {
    printf("Error open Input file (%s)\n", argv[1] );
    return -1;
  }
  output = fopen (argv[2], "wt");
  if (output==NULL) {
    printf("Error open Output file (%s)\n", argv[2] );
    return -1;
  }

  printf("Start !!\n");

  counter = 0;  merge_count = 0;  reorder_count = 0;    //desc_ptr=NULL;
  len_subject = 0;
  while (len_subject <= 0) {
    fgets(buffer1, MAX_BUF_LEN, input);   // 처음 한 줄을 읽어와서  subject (표제어) 를 취득해 둔다.
    len_subject= get_subject(buffer1, subject1);
    printf("length=%d, subject=%s\n", len_subject, subject1 );
  }

  while( !feof(input) ) {
    if (fgets(buffer2, MAX_BUF_LEN, input)==NULL) {  // 다음 한 줄을 읽어 와서,
      break;    // 더 읽을 것이 없다면 끝.
    }
    counter++;
    len_subject= get_subject(buffer2, subject2);
    if (len_subject <= 0) {   // 못 찾으면 아무런 변화가 없음. 다음 행을 읽어서 처리.
      continue;
    }

    cmp = strcmp(subject1, subject2);
    if ( cmp == 0 )  // 두 표제어가 동일한 문자열 이라면,
    {
      if (buffer1[strlen(buffer1)-1] == '\n')
        buffer1[strlen(buffer1)-1] = '\0';
      strcat(buffer1, "<hr>");    // 가로줄 한 줄을 넣고 ,
      strcat(buffer1, (buffer2+len_subject) );    // 두개의 행을 합친다.
      merge_count++;
      printf("count=%d,                subject=%s is merged.\n", counter, subject2 );
    }
    else
    if ( cmp > 0) {
      printf("change order : count=%d, subject=%s is possible to duplicated.\n", counter, subject2 );
      // if (buffer2[0]=='-')
      // { printf("\t\tmaybe it's ok to do nothing.\n" );
      // } else {
        fputs(buffer2, output);
        reorder_count++;
      // }
    }
    else {  // 표제어가 다르다면, 이전 표제어를 파일로 기록하고, 비교 대상을 새롭게 한다.
      printf("count=%d, length=%d, subject=%s\r", counter, len_subject, subject2 );
      if (subject1[0] != '\0' ) {
        fputs(buffer1, output);
      }
//      printf("\t%s\n", buffer1);
      strncpy(buffer1, buffer2, MAX_BUF_LEN);
      strncpy(subject1, subject2, 1024);
    }

  } // end of while.
  fputs(buffer1, output);   // 마지막 행도 파일로 기록해 주고,
  fclose(input);
  fclose(output);

  printf(".. DONE. merge_count = %d, reorder_count=%d\n", merge_count, reorder_count );

}


// ret = subject_length.
int   get_subject(char *src, char *subject) {
  char *ptr, *tag_pos;
  int   len;
  ptr = strstr(src, "\t" );
  if (ptr != NULL) {
    len = (ptr-src);
    strncpy( subject, src, len );   subject[len] = '\0';
    tag_pos = strstr(subject, "<");
    if ( tag_pos != 0) {   // 표제어 속에 TAG 가 있다면, TAG 이후는 무시.
      tag_pos[0] = '\0';   // TAG 자리에서 잘라 준다.
    }
  }
  return  len;
}
