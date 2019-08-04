#ifndef ERRORS_H
#define ERRORS_H
/**
Function: memAllocError()
Purpose: To print a generic memory error message on screen, showing in which
         function the error occurred.
Preconditions: None.
Postconditions: None.
Side Effects: Prints text to the screen.
Inputs: character array containing the name of the calling function - for
        debugging purposes.
**/
void memAllocError(char* functionName);

/**
Function: printError()
Purpose: To be a generic error printing function.
Preconditions: None.
Postconditions: None.
Side Effects: Prints text to the screen.
Inputs: char* errorName - a string with the error name.
        char* errorText - text giving more info about the error.
Outputs: Prints text with error info to the screen.
Returns: None.
**/
void printError(char * errorName, char * errorText);


/*=== Begin Function memAllocError() =========================================*/
void memAllocError(char* functionName){
  printf("Memory Allocation Error\n");
  printf("=======================");
  printf("Unable to allocate memory in function %s\n", functionName);
}/*=== End Function memAllocError() ==========================================*/

/*=== Begin Function printError() ============================================*/
void printError(char * errorName, char * errorText){
  byte titleLen = (byte)strlen(errorName) + 7;
  byte i;

  printf("Error: %s\n", errorName);
  for(i = 0; i < titleLen; ++i){
    printf("=");
  }
  printf("\n%s\n", errorText);
  return;
}/*=== End Function printError() =============================================*/
#endif
