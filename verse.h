#ifndef VERSE_H
#define VERSE_H
const char * DEFAULT_PASSAGE_FILE = "002_GEN_01";

struct verse {
  char *sText;
  int sLength;
  int sMaxLength;

  byte vNum;
  byte vMask;

  struct verse *vPrev;
  struct verse *vNext;
};

typedef struct verse verse;

/**
Function: initVerse()
Purpose: To initialize a verse and allocate memory for the character array.
Preconditions: None.
Postconditions: Character array is initialized, variables for tracking length
                of string and current maximum size of string are initialized,
                and character array has a '\0' character appended to ensure that
                it will print correctly (e.g. printf("%s", sourceString.sText);)
Side Effects: In the event of insufficient memory being made available to the
              program, it will generate an error.
Inputs: verse *sourceVerse - the verse to be initialized.
        byte vNum - the number of the verse.
Outputs: None, barring errors.
Return: TRUE on success, FALSE on Failure.
**/
bool initVerse(verse * sourceVerse, byte vNum);

/**
Function: addVerse()
Purpose: To add a verse to the linked list of verses and update the main pointer
         to point to the new verse.
Preconditions: First verse must have been initialized. Also, the main function
               needs to have a pointer to the initial verse - ideally it should
               have been initialied via malloc rather than statically.
Postconditions: A second verse will be initialized and linked to the first
                verse.
Side Effects: May print an error to the screenin the event of a memory
              allocation error.
Inputs: verse * oldVerse - the address of the old verse.
        verse ** versePointer - the address of the pointer to the current verse.
Outputs: On success, this function returns true. On failure, it returns false.
**/
bool addVerse(verse * oldVerse, verse ** versePointer);

/**
Function: destroyVerse()
Purpose: To release memory allocated to a verse once we are no longer using
         it.
Preconditions: String should have been initialized with initVerse(), but
               I don't think this is strictly necessary. Mind you, there would
               not be a lot of pointto destroy an uninitialized string.
Postconditions: Memory allocated for the vstrings character array will be freed
                and the string will have been de-initialized.
Side Effects: None.
Inputs: vstring *sourceString - the string to be destroyed.
Outputs: none.
**/
void destroyVerse(verse * sourceVerse);

/**
Function: resizeVerse()
Purpose: To resize the string in a verse.
Preconditions: Verse must be initialized via initVerse()
PostConditions: Memory allocated for string character array will be doubled and
                String int indicating sMaxLength will have been updated.
Side Effects: If the string length is expanded beyond 2^7 (32000 approx) chars,
               It will print out an error and not expand the string.
Inputs: vstring *sourceString - the sourceString to have its size doubled.
Outputs: On success, it will return TRUE. On failure it will return FALSE.
**/
bool resizeVerse(verse * sourceVerse);

/**
Function: appendString()
Purpose: To append a string onto the existing string within a verse.
Preconditions: The verse must have been initialized.
Postconditions: The sourceString will have been appended to the end of the verse
                string.
Side Effects: Verse String may be expanded to fit the added string. May also
              print error messages if a malloc error occurs.
Inputs: char *sourceString - the string to be appended to the verse.
        verse *sourceVerse - the verse the sourceString is to be appended to.
Outputs: bool - on success TRUE. on failure FALSE.
**/
bool appendString(char *sourceString, verse *sourceVerse);

/**
 Function: printVerse()

**/
void printVerse(WINDOW * win, verse *sourceVerse, bool firstLetter, int mask);

/**
 Function: nextVerse()
 Purpose: To advence to the next verse in the linked list.
 Preconditions: currentVerse must have been initialized.
 Postconditions: if there is subsequent verse in the list, versePointer will be
                 updated to point at it.
 Side Effects: None.
 Inputs: verse *sourceVerse - the address of the current verse
         verse **versePointer - the address of the pointer to the currentVerse
 Outputs: if it successfully jumps to another verse, it returns TRUE.
          if there is no other verse in the chain, it returns FALSE.
 **/
bool nextVerse(verse *sourceVerse, verse **versePointer);

/**
 Function: prevVerse()
 Purpose: To move to the previous verse in the linked list.
 Preconditions: currentVerse must have been initialized.
 Postconditions: if there is previous verse in the list, versePointer will be
                 updated to point at it.
 Side Effects: None.
 Inputs: verse *sourceVerse - the address of the current verse
         verse **versePointer - the address of the pointer to the currentVerse
 Outputs: if it successfully jumps to another verse, it returns TRUE.
          if there is no other verse in the chain, it returns FALSE.
 **/
bool prevVerse(verse *sourceVerse, verse **versePointer);

/**
Function: rewindVerse()
Purpose: to "rewind" the verse back to the first verse in the series.
Preconditions: the verse must be properly initialized.
Postconditions: the versePointer will be pointing to the first verse in the
                list of verses.
Side Effects: None.
Inputs: verse *sourceVerse - the address current verse
        verse ** versePointer - the address of the pointer to the currentVerse
Outputs: None.
**/
void rewindVerse(verse *sourceVerse, verse **versePointer);

/**
Function: loadVerse()
Purpose: to load a verse in from file.
Preconditions: sourceVerse must be initialied. file pointer fp must be
               initialied to a file opened for reading.
Postconditions: sourceVerse will have a verse loaded from file into it.
Side Effects: May have errors print if there are memory allocation issues.
Inputs: verse *sourceVerse - the address of the current verse
Outputs: boolean - TRUE on success, FAIL on failure.
**/
bool loadVerse(verse *sourceVerse, FILE *fp);



/*=== Begin function initVerse() =============================================*/
bool initVerse(verse * sourceVerse, byte vNum){
  bool returnValue = TRUE;
  sourceVerse->sMaxLength = 32;
  sourceVerse->sLength = 1;
  sourceVerse->vNum = vNum;
  sourceVerse->vPrev = NULL;
  sourceVerse->vNext = NULL;
  sourceVerse->sText = (char*) malloc(sourceVerse->sMaxLength + 1);
  if(!sourceVerse->sText){
    memAllocError("initVerse()");
    returnValue = FALSE;
  } else {
    sourceVerse->sText[0] = '\0';
  }

  return returnValue;
}/*=== End function initVerse() =============================================*/

bool addVerse(verse * oldVerse, verse ** versePointer){
  bool returnValue = TRUE;
  verse *newVerse = NULL;

  //allocate memory for new verse
  newVerse = (verse*)malloc(sizeof(struct verse));

  if(newVerse){
    initVerse(newVerse, oldVerse->vNum + 1);

    //link verses together
    oldVerse->vNext = newVerse;
    newVerse->vPrev = oldVerse;

    //update the verse versePointer
    *versePointer = newVerse;
  } else { //Error condition
    returnValue = FALSE;
    memAllocError("addVerse()");
  }

  return returnValue;
}

/*=== Begin Function destroyVerse() ===========================================*/
void destroyVerse(verse * sourceVerse){
  //recursively go to the end of the linked list and free up all of the memory
  //allocated for the verses and strings, then work our way backwards.
  if(sourceVerse->vNext != NULL){
    destroyVerse(sourceVerse->vNext);
    //free(sourceVerse->vNext);
  }

  free(sourceVerse->sText);
  free(sourceVerse);
  sourceVerse = NULL;
  //sourceVerse->sMaxLength = 0;
  //sourceVerse->sLength = 0;

  return;
}/*===End function destroyVerse() ===========================================*/

/*=== Beginning of function resizeVerse() ===================================*/
bool resizeVerse(verse * sourceVerse){
  //char *temp;
  bool returnValue = TRUE;
  int memRequired = sourceVerse->sMaxLength * 2 + 1;

  //Copy old string contents to new, bigger string
  if(memRequired <= 0){
    //if doubling sMaxLength gives us an overflow error, then don't expand the
    //string size.
    returnValue = FALSE;
    printf("Error: Max vstring Size has already been reached.\n");
  } else {
    sourceVerse->sText = realloc(sourceVerse->sText, memRequired);
    if(sourceVerse->sText == NULL) {
      memAllocError("resizeVerse()");
      returnValue = FALSE;
    } else {
      sourceVerse->sMaxLength *= 2;
    }
  }

  return returnValue;
}/*=== End Function resizeVerse =============================================*/

/*=== Begin Function appendString() ==========================================*/
bool appendString(char *sourceString, verse *sourceVerse){
  bool returnValue = TRUE;
  short totalLength = (short)strlen(sourceString) + sourceVerse->sLength;

  //Ensure string in verse is large enough for the string we are about to insert
  while(totalLength > sourceVerse->sMaxLength){
    returnValue = resizeVerse(sourceVerse);
    //error check
    if(!returnValue){
      break;
    }
  }

  if(returnValue){
    //append source string onto existing string verse string.
    strcat(sourceVerse->sText, sourceString);
    sourceVerse->sLength = totalLength;
  }else {
    //print more messages for a malloc error that caused resizeString
    //to return "FALSE"
    printf("resizeVerse(0 called from appendString()\n");
  }

  return returnValue;
}/*=== End Function appendString() ===========================================*/

void printVerse(WINDOW * win, verse *sourceVerse, bool firstLetter, int mask){
  int i;
  int wordsPrinted = 0;
  bool newWord = TRUE;
  werase(win);

  mvwprintw(win, 0, 0, "%d. ", sourceVerse->vNum);

  if(!firstLetter){
    //print the full text of the verse
    mvwprintw(win, 0, 0, "%s", sourceVerse->sText);
  } else{
    //print the shortened first letter only version of the verse
    for(i = 0; i < sourceVerse->sLength; ++i){
      if((sourceVerse->sText[i] <= 'Z' && sourceVerse->sText[i] >= 'A') ||
         (sourceVerse->sText[i] <= 'z' && sourceVerse->sText[i] >= 'a')){
           if(newWord){
               if(wordsPrinted < mask){
                 wprintw(win, "*");
               } else {
                 wprintw(win, "%c",sourceVerse->sText[i]);
               }
               ++wordsPrinted;
             newWord = FALSE;
           }
         } else {
           if(sourceVerse->sText[i] == ' ' || sourceVerse->sText[i] == '\n' ||
              sourceVerse->sText[i] == '\r' || sourceVerse->sText[i] == '\t'){
             newWord = TRUE;
           } else {
              if(wordsPrinted < mask){
                wprintw(win, "*");
              } else {
                wprintw(win, "%c", sourceVerse->sText[i]);
              }
              ++wordsPrinted;

           }
         }
    }
  }

  wrefresh(win);
  return;
}

/*=== Begin function nextVerse() =============================================*/
bool nextVerse(verse *sourceVerse, verse **versePointer){
 bool returnValue = TRUE;

 if(sourceVerse->vNext != NULL){
   *versePointer = sourceVerse->vNext;
 } else {
   returnValue = FALSE;
 }

 return returnValue;
}/*=== End function nextVerse() ==============================================*/

/*=== Begin Function prevVerse() =============================================*/
bool prevVerse(verse *sourceVerse, verse **versePointer){
 bool returnValue = TRUE;

 if(sourceVerse->vPrev){
   *versePointer = sourceVerse->vPrev;
 } else {
   returnValue = FALSE;
 }

 return returnValue;
}/*=== End Function prevVerse() ==============================================*/

/*=== Begin Function rewindVerse() ===========================================*/
void rewindVerse(verse *sourceVerse, verse **versePointer){
  if(sourceVerse->vPrev){
    rewindVerse(sourceVerse->vPrev, versePointer);
  } else {
    *versePointer = sourceVerse;
  }
  return;
}/*=== End Function rewindVerse() ============================================*/

/*=== Begin Function loadVerse() =============================================*/
bool loadVerse(verse *sourceVerse, FILE *fp){
  char temp[33] = {};
  char lastChar;
  bool repeat;
  bool returnValue = TRUE;

  do{
    repeat = TRUE;
    fgets(temp, 32, fp);

    //shove into the string & !!! Begin Error Check !!!
    if(!appendString(temp, sourceVerse)){
      printError("appendString Error", "Error in main loop of loadVerse()");
      returnValue = FALSE;
      break;
    }//!!! End Error Check !!!

    //check to see if we got the entire verse
    lastChar = temp[strlen(temp) - 1];
    if(lastChar == '\n' || lastChar == '\r' || feof(fp)){
      repeat = FALSE;
    }

  } while(repeat);

  return returnValue;
}/*=== End Function loadVerse() ==============================================*/

bool loadPassage(char * fileName, verse *currentVerse, char* book, int * chapter){
  FILE * fp;
  char temp[5];
  bool returnValue = true;
  char directoryName[64] = "./bibleFiles/";
  fp = fopen(strcat(directoryName,fileName),"r");

  if(fp != NULL){
    //if no file, display error and quit.
    initVerse(currentVerse, 1);
    fgets(book, 24, fp);
    book[strlen(book) - 1] = '\0'; //eliminate the newline character.
    fgets(temp, 4, fp);
    *chapter = atoi(temp);

    loadVerse(currentVerse, fp);
    //printVerse(currentVerse, TRUE);

    while(!feof(fp)){
      addVerse(currentVerse, &currentVerse);
      loadVerse(currentVerse, fp);
      //printVerse(currentVerse, FALSE);
      //printVerse(currentVerse, TRUE);
    }

    fclose(fp);
    rewindVerse(currentVerse, &currentVerse);

  } else {
    returnValue = FALSE;
    printError("File Load Error","Unable to load passage file in ./bibleFiles");
  }

  return returnValue;
}

bool saveVerse(char * fileName){
  bool returnValue = TRUE;
  FILE * fp = fopen("./bibleFiles/savedVerse", "w+");
    if(fp != NULL){
      fprintf(fp, "%s", fileName);
      fclose(fp);
    } else {
      returnValue = FALSE;
  }


  return returnValue;
}

bool loadSavedVerse(char * fileName){
  bool returnValue = TRUE;
  char temp[MAX_FILENAME_LENGTH] = "";

  FILE * fp = fopen("./bibleFiles/savedVerse", "r");
  if(fp != NULL){
    fgets(temp, MAX_FILENAME_LENGTH, fp);
    strcpy(fileName, temp);
    fclose(fp);
  } else {
    fp = fopen("./bibleFiles/savedVerse", "w+");
    if(fp != NULL){
      fprintf(fp, "%s", DEFAULT_PASSAGE_FILE);
      fclose(fp);
      strcpy(fileName, DEFAULT_PASSAGE_FILE);
    } else {
      printError("File Error:",
                 "Unable to open ./bibleFiles/savedVerse for read or write");
      returnValue = FALSE;
    }

  }

  return returnValue;
}
#endif
