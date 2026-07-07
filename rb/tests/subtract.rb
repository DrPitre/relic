PROCEDURE main
DIM tn : INTEGER
DIM bo : BOOLEAN; b1, b2, b3 : BYTE
DIM i1, i2, i3 : INTEGER
DIM r1, r2, r3 : REAL

PRINT "(***** SUBTRACTION TESTS *****)"

tn = 1
b1 = 28 \ b2 = 3
b3 = b1 - b2 - 2
PRINT "TEST ";tn; " (BYTE = BYTE - BYTE) ";
IF b3 = 25 - 2 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 25 - 2, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 31 \ i2 = 9
b3 = i1 - i2
PRINT "TEST ";tn; " (BYTE = INTEGER - INTEGER) ";
IF b3 = 22 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 22, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 12.8 \ r2 = 341.8
b3 = r1 - r2
PRINT "TEST ";tn; " (BYTE = REAL - REAL) ";
IF b3 = -329 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -329, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 28 \ b1 = 3
b3 = i1 - b1
PRINT "TEST ";tn; " (BYTE = INTEGER - BYTE) ";
IF b3 = 25 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 25, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 34 \ i1 = 32
b3 = b1 - i1
PRINT "TEST ";tn; " (BYTE = BYTE - INTEGER) ";
IF b3 = 2 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 2, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 3.28 \ b1 = 34
b3 = r1 - b1
PRINT "TEST ";tn; " (BYTE = REAL - BYTE) ";
IF b3 = -30.72 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -30.72, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 992 \ r1 = 32.8
b3 = b1 - r1
PRINT "TEST ";tn; " (BYTE = BYTE - REAL) ";
IF b3 = 961.2 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 961.2, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 1928 \ r1 = 34.11
b3 = i1 - r1
PRINT "TEST ";tn; " (BYTE = INTEGER - REAL) ";
IF b3 = 1893.89 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 1893.89, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 1.928 \ i1 = 3411
b3 = r1 - i1
PRINT "TEST ";tn; " (BYTE = REAL - INTEGER) ";
IF b3 = -3409.072 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -3409.072, Got ";b3;")"
ENDIF

PRINT

tn = tn + 1
b1 = 28 \ b2 = 63
i3 = b1 - b2
PRINT "TEST ";tn; " (INTEGER = BYTE - BYTE) ";
IF i3 = -35 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -35, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ i2 = 663
i3 = i1 - i2
PRINT "TEST ";tn; " (INTEGER = INTEGER - INTEGER) ";
IF i3 = -691 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -691, Got ";i3;")"
ENDIF

tn = tn + 1
r1 = -2.838 \ r2 = 61.63
i3 = r1 - r2
PRINT "TEST ";tn; " (INTEGER = REAL - REAL) ";
IF i3 = -64.498 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -64.498, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ b1 = 3
i2 = i1 - b1
PRINT "TEST ";tn; " (INTEGER = INTEGER - REAL) ";
IF i3 = -31 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -31, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -2822 \ b1 = 11
i2 = b1 - i1
PRINT "TEST ";tn; " (INTEGER = BYTE - INTEGER) ";
IF i2 = 2833 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 2833, Got ";i2;")"
ENDIF

tn = tn + 1
b1 = 4 \ r1 = 3421.18
i2 = r1 - b1
PRINT "TEST ";tn; " (INTEGER = REAL - BYTE) ";
IF i3 = -3417.18 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -3417.18, Got ";i3;")"
ENDIF

tn = tn + 1
b1 = 68 \ r1 = 342.118
i2 = b1 - r1
PRINT "TEST ";tn; " (INTEGER = BYTE - REAL) : ";
IF i3 = -274.118 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -274.118, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 6 \ r1 = 3421.18
i2 = r1 - i1
PRINT "TEST ";tn; " (INTEGER = REAL - INTEGER) : ";
IF i3 = -3415.18 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -3415.18, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 23681 \ r1 = 2.18
i2 = i1 - r1
PRINT "TEST ";tn; " (INTEGER = INTEGER - REAL) : ";
IF i3 = 23678.82 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 23678.82, Got ";i3;")"
ENDIF

PRINT
