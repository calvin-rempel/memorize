#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "general_funcs.h"
#include "errors.h"
#include "ui.h"
#include "loadPassage.h"


int main(){
  bookListItem bookList[BOOKS_IN_BIBLE] = {};
  chapterItem chapterList[MAX_CHAPTERS] = {};
  int selectedBook = 0;
  int i;

  initscr();
  cbreak();
  noecho();
  colorInit();
  curs_set(0);

  //Load Book List
  if(loadBookList(bookList)){
    drawBookList(bookList, selectedBook);
    selectedBook = selectBook(bookList);

    if(!selectedBook){
        //quit subroutine and return to main program
        mvprintw(0,0, "No Book Selected");
        getch();
    } else {
      //Book has been selected

      loadChapterList(bookList[selectedBook].fileName, chapterList);

      //DRAW CHAPTER LIST
      wipeScreen();
      mvprintw(0,0,"");
      for(i = 0; chapterList[i].fileName[0] != '\0'; ++i){
        printw("%d: %s\n", i + 1, chapterList[i].fileName);
      }
      getch();
    }
  } else {
    //Error Loading Book List.
  }
  //User Chooses Book

  //Load Chapter List

  //User Chooses Chapter

  //Load Chapter into memory

  //Save Currently Selected Chapter to state file for next program load

  endwin();
  return 0;
}
