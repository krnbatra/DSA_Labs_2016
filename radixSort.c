#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct pair{
  char str[1000];
  int pgnum;
}pair;

pair* arr[1000];
pair* tempArr[1000];
void radixSort(int numStrings, int maxLength){
  int R = 26;
  int d, i;
  for(d = maxLength-1; d >= 0; d--){
    int count[R+1];
    for(i = 0;i < R+1; i++){
      count[i] = 0;
    }
    for(i = 0;i < numStrings; i++){
      char c[10000];
      strcpy(c, arr[i]->str);
      char ch = c[d];
      if(ch==' '){
        count[0]++;
      }else
        count[ch-'a'+1]++;
    }
    for(i = 0;i < R; i++){
      count[i+1] += count[i];
    }
    for(i = numStrings-1;i >= 0; i--){
      char c[10000];
      strcpy(c, arr[i]->str);
      char ch = c[d];
      int idx;
      if(ch==' '){
        idx = count[0]-1;
        count[0]-=1;
      }else{
        idx = count[ch-'a'+1]-1;
        count[ch-'a'+1]-=1;
      }
      pair* p = (pair*)malloc(sizeof(pair));
      p->pgnum = arr[i]->pgnum;
      strcpy(p->str, arr[i]->str);
      tempArr[idx] = p;
    }
    for(i = 0; i < numStrings; i++){
      pair* p = (pair*)malloc(sizeof(pair));
      strcpy(p->str, tempArr[i]->str);
      p->pgnum = tempArr[i]->pgnum;
      arr[i] = p;
    }
  }
  return;
}

void manipulate(char string[]){
    int len = strlen(string);
    while(string[len-1]==' '){
      string[len-1] = '\0';
      len-=1;
    }
    return;
}
int containsDigitOnly(char string[10000]){
    int i = 0;
    if(string[i]=='\0' || string[i]=='\n')
      return 0;
    while(string[i]!='\0' && string[i]!='\n'){
      if(!isdigit(string[i]))
        return 0;
      i++;
    }
    return 1;
}

int main(){
  // FILE *fp;
  char buff[100000];
  // fp = fopen("/home/karan/fileHandling/file.txt", "r");
  int pageNumber = 0, numStrings = 0;
  int j = 0;
  while(fgets(buff, 10000, stdin)!=NULL){
    //if only a number is stored in buff then this would be my current pagenumber
    // printf("%s", buff);
    if(isdigit(buff[0])){
      pageNumber = atoi(buff);
      fgets(buff, 10000, stdin);
      // printf("%s", buff);
    }
    int i = 0, startIndex = 0, endIndex = 0;
    while(buff[i]!='\0' && buff[i]!='\n'){
      startIndex = i;
      while(buff[startIndex]!=' ' && buff[startIndex]!='\n' && buff[startIndex]!='\0' && !isalpha(buff[startIndex])){
        startIndex+=1;
      }
      i = startIndex;
      while(buff[i]!=' ' && buff[i]!='\0' && buff[i]!='\n' && isalpha(buff[i])){
        i++;
      }
      endIndex = i-1;
      while(endIndex >= 0 && !isalpha(buff[endIndex])){
        endIndex-=1;
      }
      i++;
      if(endIndex < startIndex)
        continue;
      char word[1000];
      int k = 0;
      int l = startIndex;
      while(l <= endIndex){
        if(buff[l]>=65&&buff[l]<=90){
          word[k] = buff[l]+32;
        }else{
          word[k] = buff[l];
        }
        k++;
        l++;
      }
      word[k++] = '\0';
      // printf("%s\n", word);
      pair* p = (pair*)malloc(sizeof(pair));
      strcpy(p->str,  word);
      p->pgnum = pageNumber;
      arr[j] = p;
      j++;
      numStrings++;
    }
    // printf("%d\n", pageNumber);
  }
  int i = 0;
  int maxLength = 0;
  for(i = 0;i < numStrings; i++){
    if(strlen(arr[i]->str) > maxLength)
      maxLength = strlen(arr[i]->str);
  }
  for(i = 0;i < numStrings; i++){
    int currStringLength = strlen(arr[i]->str);
    while(currStringLength < maxLength){
      arr[i]->str[currStringLength] = ' ';
      currStringLength++;
    }
    arr[i]->str[currStringLength] = '\0';
  }
  radixSort(numStrings, maxLength);
  for(i = 0;i < numStrings; i++){
    manipulate(arr[i]->str);
  }
  i = 0;
  while(i < numStrings){
    char currentString[10000];
    int currentPageNumber = arr[i]->pgnum;
    strcpy(currentString, arr[i]->str);
    printf("%s %d", currentString, arr[i]->pgnum);
    i++;
    // if(i >= numStrings){
    //   break;
    // }
    while(i < numStrings && strcmp(arr[i]->str, currentString)==0){
      if(currentPageNumber==arr[i]->pgnum){
        i++;
      }else{
        printf(", %d", arr[i]->pgnum);
        currentPageNumber = arr[i]->pgnum;
        i++;
      }

    }
    printf("\n");
  }
  return 0;
}
