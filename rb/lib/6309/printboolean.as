              section code

* Print a Boolean
*
* Entry:
*   A = path to write
*   B = boolean vale
printboolean
printboolean export
              lda     printboolean.p0,u
              tst     printboolean.p1,u
              beq     printfalse
              leax    _true,pcr
              bra     write
printfalse    leax    _false,pcr
write         lbra    FPUTS

_true         fcc     /True/
              fcb     0
_false        fcc     /False/
              fcb     0

              endsect
