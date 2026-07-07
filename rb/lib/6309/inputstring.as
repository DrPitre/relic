              section code
* Input a String
*
* Entry:
*   inputstring.p0,u - path
*   inputstring.p1,u - area where string will be stored
*   inputstring.p2,u - maximum input length
inputstring 
inputstring  export
              lda     inputstring.p0,u
              leax    inputstring.p1,u
              ldy     inputstring.p2,u
              lbsr    FGETS_NOCR
              rts

              endsect
