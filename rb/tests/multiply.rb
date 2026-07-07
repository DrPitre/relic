PROCEDURE main
DIM tn : INTEGER
DIM bo : BOOLEAN; b1, b2, b3 : BYTE
DIM i1, i2, i3 : INTEGER
DIM r1, r2, r3 : REAL

PRINT "(***** NEW MULTIPLICATION TESTS *****)"

tn = 1
b1 = 28 \ b2 = 3
b3 = b1 * b2 * 2
PRINT "TEST ";tn; " (BYTE = BYTE * BYTE) ";
IF b3 = 84 * 2 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84 * 2, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 31 \ i2 = 9
b3 = i1 * i2
PRINT "TEST ";tn; " (BYTE = INTEGER * INTEGER) ";
IF b3 = 23 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 23, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 12.8 \ r2 = 341.8
b3 = r1 * r2
PRINT "TEST ";tn; " (BYTE = REAL * REAL) ";
IF b3 = 23 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 23, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 28 \ b1 = 3
b3 = i1 * b1
PRINT "TEST ";tn; " (BYTE = INTEGER * BYTE) ";
IF b3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 34 \ i1 = 328
b3 = b1 * 11
PRINT "TEST ";tn; " (BYTE = BYTE * INTEGER) ";
IF b3 = 118 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 118, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 3.28 \ b1 = 34
b3 = r1 * b1
PRINT "TEST ";tn; " (BYTE = REAL * BYTE) ";
IF b3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 992 \ r1 = 32.8
b3 = b1 * r1
PRINT "TEST ";tn; " (BYTE = BYTE * REAL) ";
IF b3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 1928 \ r1 = 34.11
b3 = i1 * r1
PRINT "TEST ";tn; " (BYTE = INTEGER * REAL) ";
IF b3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 1.928 \ i1 = 3411
b3 = r1 * i1
PRINT "TEST ";tn; " (BYTE = REAL * INTEGER) ";
IF b3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";b3;")"
ENDIF

PRINT

tn = tn + 1
b1 = 28 \ b2 = 63
i3 = b1 * b2
PRINT "TEST ";tn; " (INTEGER = BYTE * BYTE) ";
IF i3 = 1764 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 1764, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ i2 = 663
i3 = i1 * i2
PRINT "TEST ";tn; " (INTEGER = INTEGER * INTEGER) ";
IF i3 = -18564 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -18564, Got ";i3;")"
ENDIF

tn = tn + 1
r1 = -2.838 \ r2 = 61.63
i3 = r1 * r2
PRINT "TEST ";tn; " (INTEGER = REAL * REAL) ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ b1 = 3
i2 = i1 * b1
PRINT "TEST ";tn; " (INTEGER = INTEGER * REAL) ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -2822 \ b1 = 11
i2 = b1 * i1
PRINT "TEST ";tn; " (INTEGER = BYTE * INTEGER) ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
b1 = 4 \ r1 = 3421.18
i2 = r1 * b1
PRINT "TEST ";tn; " (INTEGER = REAL * BYTE) ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
b1 = 68 \ r1 = 342.118
i2 = b1 * r1
PRINT "TEST ";tn; " (INTEGER = BYTE * REAL) : ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 6 \ r1 = 3421.18
i2 = r1 * i1
PRINT "TEST ";tn; " (INTEGER = REAL * INTEGER) : ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 23681 \ r1 = 2.18
i2 = i1 * r1
PRINT "TEST ";tn; " (INTEGER = INTEGER * REAL) : ";
IF i3 = 84 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

PRINT





tn = tn + 1
PRINT "TEST #";tn;": REAL = BYTE * BYTE : ";
b1 = 28 \ b2 = 6
r3 = b1 * b2
PRINT "r3(";r3;") = ";"b1(";b1;") * b2(";b2;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = INTEGER * INTEGER : ";
i1 = -28 \ i2 = -63
r3 = i1 * i2
PRINT "r3(";r3;") = ";"i1(";i1;") * i2(";i2;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = REAL * REAL : ";
r1 = -28.118 \ r2 = 161.63
r3 = r1 * r2
PRINT "r3(";r3;") = ";"r1(";r1;") * r2(";r2;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = INTEGER * BYTE : ";
i1 = -4428 \ b1 = 11
r2 = i1 * b1
PRINT "r2(";r2;") = ";"i1(";i1;") * b1(";b1;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = BYTE * INTEGER : ";
b1 = 21 \ i1 = -222
r2 = b1 * i1
PRINT "r2(";r2;") = ";"b1(";b1;") * i1(";i1;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = REAL * BYTE : ";
r1 = 119.331 \ b1 = 34
r2 = r1 * b1
PRINT "r2(";r2;") = ";"r1(";r1;") * b1(";b1;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = BYTE * REAL : ";
b1 = 68 \ r1 = 342.118
r2 = b1 * r1
PRINT "r2(";r2;") = ";"b1(";b1;") * r1(";r1;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = REAL * INTEGER : ";
r1 = 3311.336 \ i1 = 16
r2 = r1 * i1
PRINT "r2(";r2;") = ";"r1(";r1;") * i1(";i1;")"

tn = tn + 1
PRINT "TEST #";tn;": REAL = INTEGER * REAL : ";
i1 = 238 \ r1 = 152.18
r2 = i1 * r1
PRINT "r2(";r2;") = ";"i1(";i1;") * r1(";r1;")"
PRINT
