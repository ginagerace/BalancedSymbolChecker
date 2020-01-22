#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int DebugMode;

typedef struct stackStruct{
    char* darr;         //pointer to dynamic array
    int allocated;      //amount of space allocated
    int inUse;          //top of stack indicator
} Stack;

//function to initialize the stack
void init(Stack* s) {
    s->allocated = 2;
    s->darr = (char*) malloc (sizeof (char) * s->allocated);
    s->inUse = 0;
}

//function to check if stack is empty
int isEmpty(Stack* s) {
    if(s->inUse == 0)
        return TRUE;
    else
        return FALSE;
}

//function to grow the stack by 2
void grow(Stack* s) {
    //print debugging statements
    if(DebugMode==TRUE) {
        printf("\nGrowing stack from size %d to size %d", s->allocated, s->allocated + 2);
        printf("\nValues copied to new array: %d", s->inUse);
    }
    //allocate two more spaces for the array
    char *temp = s->darr;
    s->darr = (char *) malloc ( (s->allocated + 2) * sizeof(char) );
    int i;
    for ( i = 0 ; i < s->allocated ; i++)
        s->darr[i] = temp[i];
    free (temp);
    s->allocated = s->allocated + 2;
}

//function for pushing/adding element to stack
void push(Stack* s, char val) {
    //print debugging statements
    if(DebugMode==TRUE)
        printf("\nPushing %c to stack ", val);
    //check if stack has room
    if(s->inUse >= s->allocated)
        grow(s);
    //add value to stack
    s->darr[s->inUse] = val;
    s->inUse = s->inUse + 1;
}

//function for accessing/returning top element of stack
char top(Stack* s) {
    return ( s->darr[s->inUse - 1] );
}

//function for popping/removing element from stack
void pop(Stack* s) {
    //print debugging info
    if(DebugMode==TRUE)
        printf("\nPopping %c from stack ", top(s));
    //decrease inUse value by 1
    s->inUse = s->inUse - 1;
}

//function to clear stack and make it ready to use again
void clear(Stack* s) {
    free(s->darr);
    init(s);
}

//function to check if input is balanced, returns the number of the error that occurred or zero if no error
int check(Stack* s, char *input, int *index) {
    char comp = '\0';
    char temp;
    int error = 0;
    int i = 0;

    //continue while line is not finished
    while(input[i] != '\0') {
        comp = input[i];

        //if input is an opening symbol, push it to stack
        if(comp=='(' || comp=='{' || comp=='[' || comp=='<')
            push(s, comp);

        //check if input is a closing symbol
        else if(comp==')' || comp=='}' || comp==']' || comp=='>') {
            //if stack is empty, then there's a missing opening symbol (error #2)
            if(isEmpty(s)) {
                error = 2;
                break;
            }
            temp = top(s);
            //if closing symbol matches opening symbol at top, pop the opening symbol
            if((temp=='(' && comp==')') || (temp=='{' && comp=='}') ||
               (temp=='[' && comp==']') || (temp=='<' && comp=='>'))
                {pop(s);}
            //if closing symbol doesn't match, then a different closing symbol was expected (error #1)
            if(!((temp=='(' && comp==')') || (temp=='{' && comp=='}') ||
               (temp=='[' && comp==']') || (temp=='<' && comp=='>'))) {
               error = 1;
               break;
               }
        }
        i++;
    }
    //if stack isn't empty, then it is missing a closing a symbol (error #3)
    if(error==0 && isEmpty(s)==FALSE)
        error = 3;
    //update index of where the error was found
    if(error!=0 && index!=NULL)
        *index = i + 1;
    return error;
}

//function to find expected character for error #1 or missing character for error #3
char error1or3(Stack *s){
    char hold = ' ';
    char comp = top(s);
    if(comp == '(')
        hold = ')';
    else if(comp == '{')
        hold = '}';
    else if(comp == '[')
        hold = ']';
    else if(comp == '<')
        hold = '>';

    //return the char that expression is missing/expecting
    return hold;
}

//function to find missing character for error #2
char error2(Stack *s, char *input, int index){
    char hold = ' ';
    char comp = input[index-1];
    if(comp == ')')
        hold = '(';
    else if(comp == '}')
        hold = '{';
    else if(comp == ']')
        hold = '[';
    else if(comp == '>')
        hold = '<';

    //return the char that expression is missing/expecting
    return hold;
}

int main (int argc, char** argv)
{
 Stack s;               //declare stack
 init(&s);              //initialize stack
 char input[301];
 DebugMode = FALSE;
 int error;
 int index;
 int i;

 //check if debugMode was selected
 for(i=0; i<argc; i++)
       if(strcmp(argv[i], "-d") == 0) {
           DebugMode = TRUE;
           printf("Debugging Information\n");
       }

 /* set up an infinite loop */
 while (1)
 {
   /* get line of input from standard input */
   printf ("\nEnter input to check or q to quit\n");
   fgets(input, 300, stdin);

   /*remove the newline character from the input */
   i = 0;
   while (input[i] != '\n' && input[i] != '\0')
   {
      i++;
   }
   input[i] = '\0';

   /* check if user enter q or Q to quit program */
   if ( (strcmp (input, "q") == 0) || (strcmp (input, "Q") == 0) ) {
       printf ("\nGoodbye\n");
       exit(0);
   }

   //reprint the input
   printf ("%s\n", input);

   /* run the algorithm to determine is input is balanced */
   error = check(&s, input, &index);
   char hold;

   //print the line again is in debug mode
   if(DebugMode==TRUE)
        printf("\n\n%s\n", input);

   //if there are no errors
   if(error == 0){
    printf("Expression is balanced\n");
   }
   //if there is an error
   else {
    int j;
    for(j=1; j<index; j++)
        printf(" ");
    //if error is one (expecting a different closing symbol)
    if(error == 1) {
        hold = error1or3(&s);
        printf("^ expecting %c\n", hold);
    }
    //if error is two (missing an opening symbol)
    else if(error == 2) {
        hold = error2(&s, input, index);
        printf("^ missing %c\n", hold);
    }
    //if error is 3 (missing a closing symbol)
    else {
        hold = error1or3(&s);
        printf("^ missing %c\n", hold);
    }
   }
   //clear the stack to get ready for a new line of input
   clear(&s);
 }

 printf ("\nGoodbye\n");
 return 0;
}
