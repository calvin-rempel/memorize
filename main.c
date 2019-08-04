#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "general_funcs.h"
#include "errors.h"
#include "verse.h"
#include "ui.h"

int main(int argc, char *argv){
  verse *currentVerse;
  int input;
  char book[25];
  int chapter;
  int verse = 1;
  int mask = 0;
  bool hideText = FALSE;

  static int upperCtrlHeight = 5;
  static int fullTextCtrlHeight = 2;
  static int abrvTextCtrlHeight = 3;
  int fullTextHeight;
  int abrvTextHeight;

  //load passage file
  int i = 0;
  currentVerse = malloc(sizeof(verse));

  initscr();
  cbreak();
  noecho();
  colorInit();
  curs_set(0);

  //Init Windows
  textBoxSizer(upperCtrlHeight, fullTextCtrlHeight, abrvTextCtrlHeight,
               &abrvTextHeight, &fullTextHeight);

  WINDOW *upperCtrl = newwin(upperCtrlHeight, COLS, 0, 0);
  WINDOW *fullTextCtrl = newwin(fullTextCtrlHeight, COLS, upperCtrlHeight, 0);
  WINDOW *abrvTextCtrl = newwin(abrvTextCtrlHeight, COLS, (upperCtrlHeight +
                                fullTextCtrlHeight + fullTextHeight), 0);
  WINDOW *fullText = newwin(fullTextHeight, COLS, upperCtrlHeight +
                            fullTextCtrlHeight, 0);
  WINDOW *abrvText = newwin(abrvTextHeight, COLS, upperCtrlHeight +
                            fullTextCtrlHeight + fullTextHeight +
                            abrvTextCtrlHeight, 0);

  //wipeScreen();
  refresh();

  //LOAD FILES
  loadPassage("passage.txt", currentVerse, book, &chapter);

  //Do the initial round of drawing windows
  drawUpperCtrl(upperCtrl, TRUE, book, chapter, verse);
  drawFullTextCtrl(fullTextCtrl, TRUE, hideText);
  drawAbrvTextCtrl(abrvTextCtrl, TRUE, mask);
  printVerse(fullText, currentVerse, FALSE, 0);
  printVerse(abrvText, currentVerse, TRUE, mask);
  //COMMAND INTERPRETER
  //Change the loop to using a constant that is changed when Q is pressed.
  while(1){
    input = getch();
    if(input == SCREEN_RESIZE){
      screenResize(upperCtrlHeight, fullTextCtrlHeight,abrvTextCtrlHeight, &fullTextHeight,
                   &abrvTextHeight, upperCtrl, fullTextCtrl, abrvTextCtrl, fullText,
                   abrvText, book, chapter, verse, mask, hideText);

      //Print out full text and abrvtext
      if(!hideText){
        printVerse(fullText, currentVerse, FALSE, 0);
      }
      printVerse(abrvText, currentVerse, TRUE, mask);

    } else if(input == (int)',' || input == (int)'<'){
      //Previous Verse
      if(verse > 1){
        --verse;
        prevVerse(currentVerse, &currentVerse);
        if(!hideText){
          printVerse(fullText, currentVerse, FALSE, 0);
        }
        printVerse(abrvText, currentVerse, TRUE, mask);
        drawUpperCtrl(upperCtrl, FALSE, book, chapter, verse);

      }
    } else if(input == (int)'.' || input == (int)'>'){
      //Next Verse
      if(verse + 1 > 0){ //check for overflow
        ++verse;
        nextVerse(currentVerse, &currentVerse);
        if(!hideText){
          printVerse(fullText, currentVerse, FALSE, 0);
        }
        printVerse(abrvText, currentVerse, TRUE, mask);
        drawUpperCtrl(upperCtrl, FALSE, book, chapter, verse);
      }
    } else if(input == (int)'p' || input == (int)'P'){
      //Previous Chapter
      if(chapter > 1){
        --chapter;
        drawUpperCtrl(upperCtrl, FALSE, book, chapter, verse);
      }
    } else if(input == (int)'n' || input == (int)'N'){
      //Next Chapter
      if(chapter + 1 > 0){
        ++chapter;
        drawUpperCtrl(upperCtrl, FALSE, book, chapter, verse);
      }
    } else if(input == (int)'l' || input == (int)'L'){
      //Load Passage
    } else if(input == (int)'=' || input == (int)'+'){
      //increase mask
      if(mask + 1 > 0){
        ++mask;
        drawAbrvTextCtrl(abrvTextCtrl, TRUE, mask);
        printVerse(abrvText, currentVerse, TRUE, mask);
      }
    } else if(input == (int)'-' || input == (int)'_'){
      //decrease mask
      if(mask > 0){
        --mask;
        drawAbrvTextCtrl(abrvTextCtrl, TRUE, mask);
        printVerse(abrvText, currentVerse, TRUE, mask);
      }
    } else if(input == (int)'h' || input == (int)'H'){
      if(hideText){
        hideText = FALSE;
        printVerse(fullText, currentVerse, FALSE, 0);
      } else {
        hideText = TRUE;
        werase(fullText);
        wrefresh(fullText);
      }

      drawFullTextCtrl(fullTextCtrl, FALSE, hideText);
    }

  }

  rewindVerse(currentVerse, &currentVerse);
  destroyVerse(currentVerse);
  endwin();
  return 0;
}
