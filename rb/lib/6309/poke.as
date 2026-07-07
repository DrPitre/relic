* POKE routine
              section code

poke
poke export
              pshs   x,a
              ldx    poke.p0,u
              lda    poke.p1,u
              sta    ,x
              puls   x,a,pc

              endsect

