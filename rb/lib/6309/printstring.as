              section code

* Print a String
*
* Entry:
*   printstring.p0,u - path
*   printstring.p1,u - address of string to print
printstring
              leax    printstring.p1,u
              lbsr    STRLEN
              tfr     d,y
              lda     printstring.p0,u
              os9     I$Write
              rts

              endsect

printstring export
