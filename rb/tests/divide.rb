PROCEDURE main
DIM tn : INTEGER
DIM bo : BOOLEAN; b1, b2, b3 : BYTE
DIM i1, i2, i3 : INTEGER
DIM r1, r2, r3 : REAL

PRINT "(***** NEW DIVISION TESTS *****)"

tn = 1
b1 = 28 \ b2 = 3
b3 = b1 / b2 + 2
PRINT "TEST ";tn; " (BYTE = BYTE / BYTE) ";
IF b3 = 9 + 2 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 9 + 2, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 31 \ i2 = 9
b3 = i1 / i2
PRINT "TEST ";tn; " (BYTE = INTEGER / INTEGER) ";
IF b3 = 3 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 3, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 12.8 \ r2 = 341.8
b3 = r1 / r2
PRINT "TEST ";tn; " (BYTE = REAL / REAL) ";
IF b3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 28 \ b1 = 3
b3 = i1 / b1
PRINT "TEST ";tn; " (BYTE = INTEGER / BYTE) ";
IF b3 = 9 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 9, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 34 \ i1 = 328
b3 = b1 / i1
PRINT "TEST ";tn; " (BYTE = BYTE / INTEGER) ";
IF b3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 3.28 \ b1 = 34
b3 = r1 / b1
PRINT "TEST ";tn; " (BYTE = REAL / BYTE) ";
IF b3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";b3;")"
ENDIF

tn = tn + 1
b1 = 992 \ r1 = 32.8
b3 = b1 / r1
PRINT "TEST ";tn; " (BYTE = BYTE / REAL) ";
IF b3 = 34 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 34, Got ";b3;")"
ENDIF

tn = tn + 1
i1 = 1928 \ r1 = 34.11
b3 = i1 / r1
PRINT "TEST ";tn; " (BYTE = INTEGER / REAL) ";
IF b3 = 56 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 56, Got ";b3;")"
ENDIF

tn = tn + 1
r1 = 1.928 \ i1 = 3411
b3 = r1 / i1
PRINT "TEST ";tn; " (BYTE = REAL / INTEGER) ";
IF b3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";b3;")"
ENDIF

PRINT

tn = tn + 1
b1 = 28 \ b2 = 63
i3 = b1 / b2
PRINT "TEST ";tn; " (INTEGER = BYTE / BYTE) ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ i2 = 663
i3 = i1 / i2
PRINT "TEST ";tn; " (INTEGER = INTEGER / INTEGER) ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";i3;")"
ENDIF

tn = tn + 1
r1 = -2.838 \ r2 = 61.63
i3 = r1 / r2
PRINT "TEST ";tn; " (INTEGER = REAL / REAL) ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -28 \ b1 = 3
i2 = i1 / b1
PRINT "TEST ";tn; " (INTEGER = INTEGER / REAL) ";
IF i3 = -9 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -9, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = -2822 \ b1 = 11
i2 = b1 / i1
PRINT "TEST ";tn; " (INTEGER = BYTE * INTEGER) ";
IF i3 = -256 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected -256, Got ";i3;")"
ENDIF

tn = tn + 1
b1 = 4 \ r1 = 3421.18
i2 = r1 / b1
PRINT "TEST ";tn; " (INTEGER = REAL / BYTE) ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";i3;")"
ENDIF

tn = tn + 1
b1 = 68 \ r1 = 342.118
i2 = b1 / r1
PRINT "TEST ";tn; " (INTEGER = BYTE / REAL) : ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 84, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 6 \ r1 = 3421.18
i2 = r1 / i1
PRINT "TEST ";tn; " (INTEGER = REAL / INTEGER) : ";
IF i3 = 0 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 0, Got ";i3;")"
ENDIF

tn = tn + 1
i1 = 23681 \ r1 = 2.18
i2 = i1 / r1
PRINT "TEST ";tn; " (INTEGER = INTEGER / REAL) : ";
IF i3 = 11840 THEN
PRINT " PASSED!"
ELSE
PRINT " FAILED! (Expected 11840, Got ";i3;")"
ENDIF

PRINT
