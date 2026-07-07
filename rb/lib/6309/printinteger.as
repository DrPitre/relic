              section code

* Print an INTEGER (SIGNED)
*
* Entry:
*   printinteger.p0,u - path
*   printinteger.p1,u - integer to print
printinteger
printinteger export
              pshs   a,x
              leas   -7,s
              tfr    s,x
              ldd    printinteger.p1,u
              lbsr   BIN_SDEC	convert to signed decimal
              lda    printinteger.p0,u
              lbsr   FPUTS
              leas   7,s
              puls   a,x,pc

              endsect
