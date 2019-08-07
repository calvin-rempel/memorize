#ifndef LOADPASSAGE_H
#define LOADPASSAGE_H

struct listItem{
  char itemName[MAX_ITEMNAME_LENGTH];
  char fileName[MAX_FILENAME_LENGTH];
};

typedef struct listItem listItem;

struct chapterItem{
  char fileName[MAX_FILENAME_LENGTH];
};

typedef struct chapterItem chapterItem;

bool loadItemList(char* listFileName, listItem * itemList){
  bool returnValue = TRUE;
  FILE * fp;
  char input[255];
  char fileName[MAX_FILENAME_LENGTH];
  char itemName[MAX_ITEMNAME_LENGTH];
  int itemCount;
  int i;
  int j;

  char directoryName[64] = "./bibleFiles/";


  fp = fopen(strcat(directoryName, listFileName), "r");
  if(fp != NULL){
    for(itemCount = 0; !feof(fp); ++itemCount){
      //bookList[bookCount] = malloc(sizeof(listItem));
      if(fgets(input, 254, fp)){
        //Get Filename
        for(i = 0; i < strlen(input) && i < MAX_FILENAME_LENGTH; ++i){
          if(input[i] == ','){
            itemList[itemCount].fileName[i] = '\0';
            break;
          } else {
            itemList[itemCount].fileName[i] = input[i];
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

        //Get itemName
        if(returnValue){
          for(j = 0; (i + j) < strlen(input) && j < MAX_ITEMNAME_LENGTH; ++j){
            if(input[i + j] == '\n' || input[i + j] == '\r' || feof(fp)){
              itemList[itemCount].itemName[j] = '\0';
              break;
            } else {
              itemList[itemCount].itemName[j] = input[i + j];
            }
          }
        }


        /*ERROR*/
        if(j > MAX_ITEMNAME_LENGTH){
          //ERROR!
          printError("Item Name Error:",
                     "Max itemName Length Exceeded in index.list");
          returnValue = FALSE;
        }
      }
    }

    fclose(fp);

  } else {
    printError("Load Error", "Unable to open list file");
    returnValue = FALSE;
  }

  return returnValue;
} /*=== End of function loadBookList() =======================================*/

void drawItemList(listItem * itemList, int selectedItem, int arrayLength){
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
  for(i = selectedItem; y >= 0 && i >= 0; --i){
    if(itemList[i].itemName[0] != '\0' || itemList[i].fileName[0] != '\0'){
      x = vertiCenter - ( strlen(itemList[i].itemName) / 2 );
      if(y == horizCenter){
        attron(A_STANDOUT);
      }
      mvprintw(y, x, "%s", itemList[i].itemName);
      if(y == horizCenter){
        attroff(A_STANDOUT);
      }
      --y;
    }
  }

  //print forwards starting after selected book
  y = horizCenter + 1;
  for(i = selectedItem + 1; y < COLS && i < arrayLength; ++i){

    if(itemList[i].itemName[0] != '\0' || itemList[i].fileName[0] != '\0'){
      x = vertiCenter - ( strlen(itemList[i].itemName) / 2 );
      mvprintw(y, x, "%s", itemList[i].itemName);

      ++y;
    }
  }
  return;
}/*=== End function drawList() ===============================================*/

/*=== Begin Function selectBook() ============================================*/
int selectItem(listItem * itemList, int arrayLength, int selectedItem){
  int input;
  int oldSelectedItem = 0;

  //Get User Input
  while(1){
    input = getch();

    if(input == UP_KEY){
      if(selectedItem - 1 >= 0){
        --selectedItem;
        drawItemList(itemList, selectedItem, arrayLength);
      }
    } else if(input == DOWN_KEY){
      if(selectedItem + 1 < arrayLength){
        oldSelectedItem = selectedItem;
        ++selectedItem;
        //attempt to go to next book if current one is invalid
        while(itemList[selectedItem].itemName[0] == '\0' &&
              selectedItem + 1 < arrayLength){
              ++selectedItem;
        }
        //if the last book we landed on was invalid, return to the original
        //valid book
        if(itemList[selectedItem].itemName[0] == '\0'){
          selectedItem = oldSelectedItem;
        } else {
          drawItemList(itemList, selectedItem, arrayLength);
        }
      }
    } else if(input == ENTER_KEY){
      break;
    } else if(input == BACKSPACE_KEY || input == B_KEY){
      selectedItem = -1; //indicate no item was selected
      break;
    }
  }

  return selectedItem;
}/*=== End Function selectBook() =============================================*/


bool selectPassage(char * fileName){
  bool returnValue = TRUE;
  listItem bookList[BOOKS_IN_BIBLE] = {};
  listItem chapterList[MAX_CHAPTERS] = {};
  int selectedBook = 0;
  int selectedChapter = 0;
  int i;

  //Load Book List
  if(loadItemList("index.list", bookList)){
    while(1){
      drawItemList(bookList, selectedBook, BOOKS_IN_BIBLE);
      selectedBook = selectItem(bookList, BOOKS_IN_BIBLE, selectedBook);

      if(selectedBook < 0){
          //quit subroutine and return to main program
          fileName[0] = '\0';
          break;
      } else {
        //Book has been selected

        if(loadItemList(bookList[selectedBook].fileName, chapterList)){
          drawItemList(chapterList, selectedChapter, MAX_CHAPTERS);
          selectedChapter = selectItem(chapterList, MAX_CHAPTERS,
                                       selectedChapter);

          if(selectedChapter >= 0){
            //Chapter has been selected. set the filename so the verse
            //loader routine can get that and load the appropriate verse.
            strcpy(fileName, chapterList[selectedChapter].fileName);
            break;
          } else {
            //did not select a chapter but went back to book selection
            selectedChapter = 0; //reset for the next go-around
          }

        } else {
          //ERROR Loading Chapter List
          printError("Chapter List Loading Error",
                     "Unable to load chapter list");
          returnValue = FALSE;
          break;
        }
      }

    }
  } else {
    //Error Loading Book List.
    printError("Book List Loading Error", "Unable to load book list");
    returnValue = FALSE;
  }


  return returnValue;
}
#endif
