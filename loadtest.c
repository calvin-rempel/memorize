#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loadPassage.h"

#define MAX_FILENAME_LENGTH 12
#define MAX_BOOKNAME_LENGTH 25
#define BOOKS_IN_BIBLE 66

struct bookListItem{
  char bookName[MAX_BOOKNAME_LENGTH];
  char fileName[MAX_FILENAME_LENGTH];
};

typedef struct bookListItem bookListItem;

int main(){
  FILE * fp;
  char input[255];
  char fileName[MAX_FILENAME_LENGTH];
  char bookName[MAX_BOOKNAME_LENGTH];
  int bookCount;
  int i;
  int j;

  bookListItem bookList[BOOKS_IN_BIBLE];


  fp = fopen("./bibleFiles/index.list", "r");
  for(bookCount = 0; !feof(fp) && bookCount < BOOKS_IN_BIBLE; ++bookCount){
    //bookList[bookCount] = malloc(sizeof(bookListItem));
    if(fgets(input, 254, fp)){
      //Get Filename
      for(i = 0; i < strlen(input) && i < MAX_FILENAME_LENGTH; ++i){
        if(input[i] == ','){
          bookList[bookCount].fileName[i] = '\0';
          break;
        } else {
          bookList[bookCount].fileName[i] = input[i];
        }
      }

      /*ERROR*/
      if(i > MAX_FILENAME_LENGTH){
        //ERROR!
        printf("Error: Max FileName Length Exceeded in index.list");
        return -1;
      }

      //Move onto next letter after above error check.
      ++i;

      //Get Bookname
      for(j = 0; (i + j) < strlen(input) && j < MAX_BOOKNAME_LENGTH; ++j){
        if(input[i + j] == '\n' || input[i + j] == '\r' || feof(fp)){
          bookList[bookCount].bookName[j] = '\0';
          break;
        } else {
          bookList[bookCount].bookName[j] = input[i + j];
        }
      }

      /*ERROR*/
      if(j > MAX_BOOKNAME_LENGTH){
        //ERROR!
        printf("Error: Max Bookname Length Exceeded in index.list");
        return -1;
      }
    }
  }

  fclose(fp);

  //Display Book List
  for(bookCount = 0; bookCount < BOOKS_IN_BIBLE; ++bookCount){
    printf("Book Name: %s\t", bookList[bookCount].bookName);
    printf("File Name: %s\n", bookList[bookCount].fileName);
  }

  //Choose Book


  return 0;
}
