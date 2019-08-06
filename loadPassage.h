#ifndef LOADPASSAGE_H
#define LOADPASSAGE_H
#define MAX_FILENAME_LENGTH 12
#define MAX_BOOKNAME_LENGTH 25
#define BOOKS_IN_BIBLE 66
#define MAX_CHAPTERS 150

struct bookListItem{
  char bookName[MAX_BOOKNAME_LENGTH];
  char fileName[MAX_FILENAME_LENGTH];
};

typedef struct bookListItem bookListItem;

struct chapterItem{
  char fileName[MAX_FILENAME_LENGTH];
};

typedef struct chapterItem chapterItem;

bool loadBookList(bookListItem * bookList){
  bool returnValue = TRUE;
  FILE * fp;
  char input[255];
  char fileName[MAX_FILENAME_LENGTH];
  char bookName[MAX_BOOKNAME_LENGTH];
  int bookCount;
  int i;
  int j;

  fp = fopen("./bibleFiles/index.list", "r");
  if(fp != NULL){
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
          printError("Filename Error", "Max Filename Length Exceeded in index.list");
          returnValue = FALSE;
        }

        //Move onto next letter after above error check.
        ++i;

        //Get Bookname
        if(returnValue){
          for(j = 0; (i + j) < strlen(input) && j < MAX_BOOKNAME_LENGTH; ++j){
            if(input[i + j] == '\n' || input[i + j] == '\r' || feof(fp)){
              bookList[bookCount].bookName[j] = '\0';
              break;
            } else {
              bookList[bookCount].bookName[j] = input[i + j];
            }
          }
        }


        /*ERROR*/
        if(j > MAX_BOOKNAME_LENGTH){
          //ERROR!
          printError("Book Name Error:",
                     "Max Bookname Length Exceeded in index.list");
          returnValue = FALSE;
        }
      }
    }

    fclose(fp);

  } else {
    printError("Load Error", "Unable to open index.list");
    returnValue = FALSE;
  }

  return returnValue;
} /*=== End of function loadBookList() =======================================*/

void drawBookList(bookListItem * bookList, int selectedBook){
  int horizCenter;
  int vertiCenter;
  int i;
  int x, y;
  char * goBack = "Go (B)ack";

  //Display Book List
  wipeScreen();

  //calculate horizontal centre
  horizCenter = LINES / 2;
  vertiCenter = COLS / 2;
  y = horizCenter;

  //draw control item
  mvprintw(0, COLS - strlen(goBack), "%s", goBack);

  //print backwards started at selected book
  for(i = selectedBook; y >= 0 && i >= 0; --i){
    if(bookList[i].bookName[0] != '\0' || bookList[i].fileName[0] != '\0'){
      x = vertiCenter - ( strlen(bookList[i].bookName) / 2 );
      if(y == horizCenter){
        attron(A_STANDOUT);
      }
      mvprintw(y, x, "%s", bookList[i].bookName);
      if(y == horizCenter){
        attroff(A_STANDOUT);
      }
      --y;
    }
  }

  //print forwards starting after selected book
  y = horizCenter + 1;
  for(i = selectedBook + 1; y < COLS && i < BOOKS_IN_BIBLE; ++i){

    if(bookList[i].bookName[0] != '\0' || bookList[i].fileName[0] != '\0'){
      x = vertiCenter - ( strlen(bookList[i].bookName) / 2 );
      mvprintw(y, x, "%s", bookList[i].bookName);

      ++y;
    }
  }
  return;
}/*=== End function drawList() ===============================================*/

/*=== Begin Function selectBook() ============================================*/
int selectBook(bookListItem * bookList){
  int input;
  int selectedBook = 0;
  int oldSelectedBook = 0;

  //Get User Input
  while(1){
    input = getch();

    if(input == UP_KEY){
      if(selectedBook - 1 >= 0){
        --selectedBook;
        drawList(bookList, selectedBook);
      }
    } else if(input == DOWN_KEY){
      if(selectedBook + 1 < BOOKS_IN_BIBLE){
        oldSelectedBook = selectedBook;
        ++selectedBook;
        //attempt to go to next book if current one is invalid
        while(bookList[selectedBook].bookName[0] == '\0' &&
              selectedBook + 1 < BOOKS_IN_BIBLE){
              ++selectedBook;
        }
        //if the last book we landed on was invalid, return to the original
        //valid book
        if(bookList[selectedBook].bookName[0] == '\0'){
          selectedBook = oldSelectedBook;
        } else {
          drawList(bookList, selectedBook);
        }
      }
    } else if(input == ENTER_KEY){
      break;
    } else if(input == BACKSPACE_KEY || input == B_KEY){
      selectedBook = -1; //indicate no book was selected
      break;
    }
  }

  return selectedBook;
}/*=== End Function selectBook() =============================================*/


/*=== Begin Function loadChapterList =========================================*/
bool loadChapterList(char * fileName, chapterItem * list){
  bool returnValue = TRUE;
  FILE * fp = NULL;
  char fullPath[26] = "./bibleFiles/\0";
  char input[255];
  int numChaps;

  strcat(fullPath, fileName);
  fp = fopen(fullPath, "r");
  if(fp != NULL){
    fgets(input, 254, fp); //throw away first line - it is the book title.

    for(numChaps = 0; !feof(fp) && numChaps <= MAX_CHAPTERS; ++numChaps){
      fgets(input, 254, fp);
      if(!feof(fp)){
        if(strlen(input) >= MAX_FILENAME_LENGTH){
          printError("FileName Length Error", "Filename in Chapter List Too Long");
          returnValue = FALSE;
          break;
        }

        //get rid of newline characters
        if(input[strlen(input) - 1] == '\r' || input[strlen(input) - 1] == '\n'){
          input[strlen(input) - 1] = '\0';
        }
        strcpy(list[numChaps].fileName, input);
      }
    }
    fclose(fp);
  } else {
    printError("File Open Error", "Unable to Open chapterfile");
    returnValue = FALSE;
  }

  return returnValue;
}/*=== End of function loadChapterList() =====================================*/

#endif
