#define SCREEN_RESIZE 410

#define HORIZ_BORDER_THRESHOLD 17
#define HORIZ_ABBRV_THRESHOLD 46
#define VERT_BORDER_THRESHOLD 53

#define MIN_LINES 16
#define MIN_COLS 39

bool HAS_COLOR;

/**
Function: drawUpperCtrl()
Purpose: to draw the upper set of controls and the title bar.
Preconditions: Ncurses must be initialized. A window must be initialized.
Postconditions: Upper Control Window will be drawn.
Side Effects: None.
Inputs:
  WINDOW * upperCtrl - the window.
  bool redraw - if true, redraw the entire window. Used for first draw or screen
                resize. if false, only update book and chapter info.
  char * book - the name of the book of the bible
  int chapter - the chapter number
  int verse - the verse number
Ouputs: None.
**/
void drawUpperCtrl(WINDOW * upperCtrl, bool redraw, char * book, int chapter,
                   int verse);

/**
Function: drawFullTextCtrl()
Purpose: Draw the Control Window for the Full Text Display.
Preconditions: Ncurses mode must be initialized. A Window must be initialized.
Postconditions: Control window will be drawn.
Side Effects: None.
Inputs:
  WINDOW * win - the NCURSES window
  bool redraw - if true, the entire window needs to be redrawn - either because
                this is the first time drawing the screen or because the screen
                has been resized.
                If false, then we only need to redraw the "textHidden"
                indicator.
  bool textHidden - if true, the "textHidden" indicator is set to [Hidden].
                  - if false, the "textHidden" indicator is set to [UNHIDDEN].
Outputs: None.
**/
void drawFullTextCtrl(WINDOW * win, bool redraw, bool textHidden);

/**
Function: drawAbrvTextCtrl()
Purpose: Draw the Control Window for the Abbreviated Text Display.
Preconitions: Ncurses mode must be initialized. A window must be initialized.
Postconditions: Control window will be drawn.
Side Effects: None.
Inputs:
  WINDOW * win - the NCURSES window.
  bool redraw - if true, the entire window needs to be redrawn. Used when
                drawing for the first time or when resizing the screen.
  int mask - size of the mask that the abbreviated text window will be using.
Outputs: None.
**/
void drawAbrvTextCtrl(WINDOW * win, bool redraw, int mask);

/**
Function: screenResize()
Purpose: To handle resizing, repositioning and redrawing the window elements on
         screen when the terminal window is resized.
Preconditions: Ncurses must be initialized. The windows must be initialized.
Postconditions: Windows will be resized and repositioned to fit onto the new
                screen.
Side Effects: If the screen is too small, the text windows will become unusable.
Inputs: int upperCtrlHeight - the height of the upperCtrlWindow
        int fullTextCtrlHeight - the height of the fullTextCtrl window.
        int abrvTextCtrlHeight - the height of the abbreivated text control
                                 window.
        int *fullTextHeight - pointer to fullTextHeight for the full text
                              window. Done this way to allow it to be updated
                              for main().
        int *abrvTextHeight - pointer to the abrvTextHeight for the abreviated
                              text windows. Done this way to allow it to be
                              updated for main().
        WINDOW * upperCtrl - the upper control window.
        WINDOW * fullTextCtrl - the full text control window.
        WINDOW * fullText - the full text window.
        WINDOW * abrvText - the abbreviated text window.
        char * book - the name of the book of the Bible.
        int chapter - the chapter number.
        int verse - the verse number.
        int mask - the number on the abrv mask.
Outputs: None.
 **/
void screenResize(int upperCtrlHeight, int fullTextCtrlHeight, int abrvTextCtrlHeight,
                  int* fullTextHeight, int* abrvTextHeight,
                  WINDOW * upperCtrl, WINDOW * fullTextCtrl,
                  WINDOW * abrvTextCtrl, WINDOW * fullText, WINDOW * abrvText,
                  char * book, int chapter, int verse, int mask, int textHidden);

/**
Function: textBoxSizer
Purpose: To determine the height of the text boxes based on the dimensions of
         the console and of the other windows. Specifically, determines the
         height of the full text box and abrv text box.
Preconditions: Ncurses must be initialied. control box heights must have been
               initialized.
Postconditions: None.
Side Effects: abrvHeight and fullTextHeight will have values set.
Inputs: int upperCtrlHeight - the height of the upper control window.
        int fullTextCtrlHeight - the height of the full control text window.
        int abrvTextCtrlHeight - the height of the abbreviated control text
                                 window.
        int * abrvTextHeight - a pointer to the height of the abbreviated text
                              window.
        int * fullTextHeight - a pointer to the height of the full text window.
Outputs: None.
**/
void textBoxSizer(int upperCtrlHeight, int fullTextCtrlHeight,
                    int abrvTextCtrlHeight, int *abrvTextHeight,
                    int *fullTextHeight);



void colorInit(){
  start_color();
  if(has_colors()){
    HAS_COLOR = TRUE;
    assume_default_colors(COLOR_WHITE, COLOR_BLUE);
  }
  return;
}

void wipeScreen(){
  int y, x;

  for(y = 0; y < LINES; ++y){
    for(x = 0; x < COLS; ++x){
      mvprintw(y, x, " ");
    }
  }

  return;
}


/*=== Begin Function drawAbrvTextCtrl() ======================================*/
void drawAbrvTextCtrl(WINDOW * win, bool redraw, int mask){
  static char* textLabelLong = "Abbreviated Text:";
  static char* textLabelShort = "Abrv:";
  static char* maskCtrl = "(+)Increase Mask  (-)Decrease Mask";
  static char* maskLabel = "Mask: ";
  char* textLabel;

  wattron(win, A_BOLD);
  if(redraw){
    if(COLS < HORIZ_ABBRV_THRESHOLD){
      textLabel = textLabelShort;
    } else {
      textLabel = textLabelLong;
    }

    werase(win);
    wborder(win, ' ', ' ', ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE,
            ACS_HLINE);

    mvwprintw(win, 1, 0, "%s", textLabel);
    mvwprintw(win, 1, COLS - strlen(maskCtrl), "%s", maskCtrl);
  }
  mvwprintw(win, 1, COLS / 2 - (strlen(maskLabel) + 3), "%s%3d",
            maskLabel, mask);
  wattroff(win, A_BOLD);

  wrefresh(win);
  return;
}/*=== End Function drawAbrvTextCtrl() =======================================*/

/*=== Begin Function drawFullTextCtrl() ======================================*/
void drawFullTextCtrl(WINDOW * win, bool redraw, bool textHidden){
  static char* textLabelLong = "Full Text:";
  static char* textLabelShort = "Text:";
  static char* hide = "(H)ide/Unhide Text";
  static char* statHidden = "[HIDDEN]  ";
  static char* statUnhidden = "[UNHIDDEN]";
  char* textLabel;
  char* hideIndic;

  wattron(win, A_BOLD);

  if(redraw){
    werase(win);
    wborder(win, ' ', ' ', ' ', ACS_HLINE, ' ', ' ', ACS_HLINE, ACS_HLINE);

    if(COLS < HORIZ_ABBRV_THRESHOLD){
      textLabel = textLabelShort;
    } else {
      textLabel = textLabelLong;
    }

    mvwprintw(win, 0, 0, "%s", textLabel);
    mvwprintw(win, 0, COLS - strlen(hide), "%s", hide);
  }

  if(textHidden){
    hideIndic = statHidden;
  } else {
    hideIndic = statUnhidden;
  }
  mvwprintw(win, 0, COLS/2 - strlen(hideIndic)/2, "%s", hideIndic);

  wattroff(win, A_BOLD);
  wrefresh(win);

  return;
}/*=== End of Function drawFullTextCtrl() ====================================*/

/*=== Begin Function drawUpperCtrl() =========================================*/
void drawUpperCtrl(WINDOW * upperCtrl, bool redraw, char * book, int chapter,
                   int verse){
  static char* pVerseShort = "(<)Prev V.";
  static char* pVerseLong = "(<)Previous Verse";
  static char* nVerseShort = "Next V.(>)";
  static char* nVerseLong = "Next Verse(>)";
  static char* pChapter = "<<(P)rev Chapter";
  static char* nChapter = "(N)ext Chapter>>";
  static char* lPassage = "(L)oad Passage";
  char* pVerse; //previous verse
  char* nVerse; //next verse
  int i;

  if(redraw){
    werase(upperCtrl);
    //If window is too narrow, use abbreviated control names as needed
    if(COLS < HORIZ_ABBRV_THRESHOLD){
      pVerse = pVerseShort;
      nVerse = nVerseShort;
    } else {
      pVerse = pVerseLong;
      nVerse = nVerseLong;
    }

    //Title Bar
    wborder(upperCtrl, ' ', ' ', ACS_CKBOARD, ACS_HLINE, ACS_CKBOARD,
            ACS_CKBOARD, ACS_HLINE, ACS_HLINE);
    wattron(upperCtrl, A_BOLD);
    if(HAS_COLOR){
      init_pair(1, COLOR_WHITE, COLOR_RED);
      wattron(upperCtrl, COLOR_PAIR(1));
      mvwprintw(upperCtrl, 0, 0, "");
      for(i = 0; i < COLS; ++i){
        wprintw(upperCtrl, " ");
      }
    }

    mvwprintw(upperCtrl, 0, 4, " Calvin's Bible Memory Assistant ");

    if(HAS_COLOR){
      wattroff(upperCtrl, COLOR_PAIR(1));
    }

    //First Line of Controls
    mvwprintw(upperCtrl, 1, 0, "%s", pVerse);
    mvwprintw(upperCtrl, 1, COLS / 2 - strlen(lPassage) / 2, "%s", lPassage);
    mvwprintw(upperCtrl, 1, (COLS - strlen(nVerse)), "%s", nVerse);

    //Second Line of Controls
    mvwprintw(upperCtrl, 2, 0, "%s", pChapter);
    mvwprintw(upperCtrl, 2, (COLS - strlen(nChapter)), "%s", nChapter);
  }

  //Book Chapter Verse indicator
  wattroff(upperCtrl, A_BOLD);
  mvwprintw(upperCtrl, 3, 0, "%s %3d:%3d", book, chapter, verse);

  wrefresh(upperCtrl);
  return;
}/*=== End Function drawUpperCtrl() ==========================================*/

/*=== Begin Function textBoxSizer() ==========================================*/
void textBoxSizer(int upperCtrlHeight, int fullTextCtrlHeight,
                      int abrvTextCtrlHeight, int *abrvTextHeight,
                      int *fullTextHeight){
  int multiplier;
  multiplier = (LINES - (upperCtrlHeight + fullTextCtrlHeight +
                abrvTextCtrlHeight)) / 3;
  *fullTextHeight = multiplier * 2;
  *abrvTextHeight = multiplier;

  return;
}/*=== End of Function textBoxSizer() ========================================*/

/*=== Begin Function screenResize() ==========================================*/
void screenResize(int upperCtrlHeight, int fullTextCtrlHeight, int abrvTextCtrlHeight,
                  int* fullTextHeight, int* abrvTextHeight,
                  WINDOW * upperCtrl, WINDOW * fullTextCtrl,
                  WINDOW * abrvTextCtrl, WINDOW * fullText, WINDOW * abrvText,
                  char * book, int chapter, int verse, int mask, int textHidden){
  wipeScreen();
  refresh();

  textBoxSizer(upperCtrlHeight, fullTextCtrlHeight, abrvTextCtrlHeight,
                    abrvTextHeight, fullTextHeight);

  //resize windows
  wresize(upperCtrl, upperCtrlHeight, COLS);
  wresize(fullTextCtrl, fullTextCtrlHeight, COLS);
  wresize(abrvTextCtrl, abrvTextCtrlHeight, COLS);
  wresize(fullText, *fullTextHeight, COLS);
  wresize(abrvText, *abrvTextHeight, COLS);

  //move windows as needed
  mvwin(abrvTextCtrl,(upperCtrlHeight + fullTextCtrlHeight + *fullTextHeight), 0);
  mvwin(abrvText, upperCtrlHeight + fullTextCtrlHeight + *fullTextHeight +
        abrvTextCtrlHeight, 0);

  //redraw windows
  drawUpperCtrl(upperCtrl, TRUE, book, chapter, verse);
  drawFullTextCtrl(fullTextCtrl, TRUE, textHidden);
  drawAbrvTextCtrl(abrvTextCtrl, TRUE, mask);
  return;
}/*=== End Function screenResize() ===========================================*/
