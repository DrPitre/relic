PROCEDURE main
DIM bo, bo1, bo2 : BOOLEAN
DIM b1, b2 : BYTE
DIM i1, i2 : INTEGER
DIM r1, r2 : REAL

PRINT "(***** BOOLEAN X BOOLEAN RELATION *****)"
bo1 = FALSE \ bo2 = TRUE
PRINT "bo1(";bo1;") =  bo2(";bo2;") : ";bo1 = bo2
PRINT "bo1(";bo1;") >  bo2(";bo2;") : ";bo1 > bo2
PRINT "bo1(";bo1;") <  bo2(";bo2;") : ";bo1 < bo2
PRINT "bo1(";bo1;") <= bo2(";bo2;") : ";bo1 <= bo2
PRINT "bo1(";bo1;") >= bo2(";bo2;") : ";bo1 >= bo2
PRINT "bo1(";bo1;") <> bo2(";bo2;") : ";bo1 <> bo2

PRINT "(***** BYTE X BYTE RELATION *****)"
b1 = 3 \ b2 = 3
PRINT "b1(";b1;") =  b2(";b2;") : ";b1 = b2
PRINT "b1(";b1;") >  b2(";b2;") : ";b1 > b2
PRINT "b1(";b1;") <  b2(";b2;") : ";b1 < b2
PRINT "b1(";b1;") <= b2(";b2;") : ";b1 <= b2
PRINT "b1(";b1;") >= b2(";b2;") : ";b1 >= b2
PRINT "b1(";b1;") <> b2(";b2;") : ";b1 <> b2

PRINT "(***** INTEGER X INTEGER RELATION *****)"
i1 = 6 \ i2 = -3
PRINT "i1(";i1;") =  i2(";i2;") : ";i1 = i2
PRINT "i1(";i1;") >  i2(";i2;") : ";i1 > i2
PRINT "i1(";i1;") <  i2(";i2;") : ";i1 < i2
PRINT "i1(";i1;") <= i2(";i2;") : ";i1 <= i2
PRINT "i1(";i1;") >= i2(";i2;") : ";i1 >= i2
PRINT "i1(";i1;") <> i2(";i2;") : ";i1 <> i2

PRINT "(***** REAL X REAL RELATION *****)"
r1 = 66.2 \ r2 = 3.4
PRINT "r1(";r1;") =  r2(";r2;") : ";r1 = r2
PRINT "r1(";r1;") >  r2(";r2;") : ";r1 > r2
PRINT "r1(";r1;") <  r2(";r2;") : ";r1 < r2
PRINT "r1(";r1;") <= r2(";r2;") : ";r1 <= r2
PRINT "r1(";r1;") >= r2(";r2;") : ";r1 >= r2
PRINT "r1(";r1;") <> r2(";r2;") : ";r1 <> r2

