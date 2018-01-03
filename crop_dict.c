#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAX_BUF_LEN 32767
char buffer[MAX_BUF_LEN];
// char title[1024];


char *replaceAll(char *s, const char *olds, const char *news);
char *removeButtonTag(char *s);
char *removeAnchorTag(char *s);


int main(int argc, char *argv[] )
{
  FILE *input, *output;
  char *temporary_buf, *target;

  if (argc < 3) {
    printf("\t\tDummy-HTML tag Remover for stardict-editor.\n\t\t\t-by ccash.\n\nUSAGE: crop_dict [clipped-html-file] [target-file]\n\n");
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

  while( !feof(input) ) {
    fgets(buffer, MAX_BUF_LEN, input);
    temporary_buf = replaceAll(buffer, "<span class=\"blind\"> </span>", "");
    if (temporary_buf == NULL)
    {
      target = buffer;
    }
    else
    {
      target = temporary_buf;     // free(temporary_buf);
    }
    printf("STEP_1");

    temporary_buf = replaceAll(target, "<button type=\"button\" title=\"발음 재생\" class=\"play\" data-tts=\"jp\" onclick=\"clickcr(this, 'bdy.exmtts','','',event);\"" , "<button ");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_2");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, ">      <h5>", "><h5>");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_3");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "</h5>        ", "</h5>");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_4");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "     <div", "<div");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_5");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "         <li class=\"inner_lst\">     ", "<li class=\"inner_lst\">");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_6");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "        </ul>              ", "</ul>");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_7");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "       </div>   ", "</div>");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_8");
      free (target);
      target = temporary_buf;
    }

    // Remove <!-- 연세 어학당 XXXX --> tag.
    temporary_buf = replaceAll(target, "<!-- 연세어학당 출처 여기서 노출 -->", "");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_9");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "<!-- //연세어학당 출처 여기서 노출 -->", "");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_9");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "<!-- 연세어학당 이미지 추가 -->", "");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_9");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "<!-- //연세어학당 이미지 추가 -->", "");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_9");
      free (target);
      target = temporary_buf;
    }
    temporary_buf = replaceAll(target, "                            </div>", "</div>");
    if (temporary_buf != NULL)
    {
      printf("-> STEP_9");
      free (target);
      target = temporary_buf;
    }


    // Remove <button> tag.
    temporary_buf = removeButtonTag(target);
    if (temporary_buf != NULL)
    {
      printf("-> STEP_a");
      free (target);
      target = temporary_buf;
    }
    // Remove <a href> tag.
    temporary_buf = removeAnchorTag(target);
    if (temporary_buf != NULL)
    {
      printf("-> STEP_B\n");
      free (target);
      target = temporary_buf;
    }

    fputs(target, output);
    free(target);
  }

  fclose(input);
  fclose(output);
}



char *replaceAll(char *s, const char *olds, const char *news) {
  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);


  if (newlen != oldlen) {
    for (i = 0; s[i] != '\0';) {
      if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
      else i++;
    }
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
    if (memcmp(s, olds, oldlen) == 0) {
      memcpy(sr, news, newlen);
      sr += newlen;
      s  += oldlen;
    } else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}



char *removeButtonTag(char *s) {
  char *result, *sr;
  size_t i, count = 0;

  sr = strstr(s, "<button");
  if (sr==NULL)     // 해당 TAG가 없으므로 그냥 종료. 끝.
    return NULL;

  // 원본 문자열 만큼의 버퍼를 확보해 놓고,
  result = (char *) malloc( strlen(s) );
  if (result == NULL) return NULL;
  sr = result;    // 처음 부터 시작.
  while ( *s )    // 입력 문자열의 끝 까지.
  {
    if (memcmp( s, "<button", 7) == 0 )   // TAG를 발견하면,
    {
      while ( memcmp(s, "</button>", 9) != 0 )  // TAG 의 끝을 발견할 때 까지.
        s++;
      s+=9;   // TAG 종료될 때 까지 jump
    }
    else
    {
      *sr = *s;   // 문자열 복사.
      sr++; s++;
    }
  }
  *sr = '\0';

  return result;
}


char *removeAnchorTag(char *s) {
  char *result, *sr;
  size_t i, count = 0;

  sr = strstr(s, "<a ");
  if (sr==NULL)     // 해당 TAG가 없으므로 그냥 종료. 끝.
    return NULL;

  // 원본 문자열 만큼의 버퍼를 확보해 놓고,
  result = (char *) malloc( strlen(s) );
  if (result == NULL) return NULL;
  sr = result;    // 처음 부터 시작.
  while ( *s )    // 입력 문자열의 끝 까지.
  {
    if (memcmp( s, "<a ", 3) == 0 )   // TAG를 발견하면,
    {
      while ( *s != '>' )  // 우선 TAG 열림의 끝까지 건너 뜀.
        s++;
      s++;    // '>' 이것 자체도 건너 뜀.
    }
    else if (memcmp( s, "</a>", 4) == 0 )   // TAG를 발견하면,
    {
      s+=4;    // '</a>' 이것 자체도 건너 뜀.
    }
    else
    {
      *sr = *s;   // 문자열 복사.
      sr++; s++;
    }
  }
  *sr = '\0';

  return result;
}
