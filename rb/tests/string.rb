PROCEDURE main
DIM b1, b2 : BYTE
DIM i1, i2 : INTEGER
DIM r1, r2 : REAL
DIM s1, s2 : STRING
DIM s3 : STRING[66]

b1 = 3 \ REM BYTE = INTEGER
i1 = 1168
r1 = 22.4 \ REM REAL = REAL
PRINT r1

(* BYTE addition
b2 = b1 + 3 \ REM BYTE = BYTE + INTEGER
(*b1 = b1 + 3.3 \ REM BYTE = BYTE + REAL
PRINT b1

(* INTEGER addition
i2 = i1 + b1 \ REM INTEGER = INTEGER + BYTE
i2 = i1 + i2 \ REM INTEGER = INTEGER + INTEGER
i2 = i1 + r1 \ REM INTEGER = INTEGER + REAL
i2 = 4320-(3+21)
PRINT i2

(* REAL addition
r2 = r1 + b1 \ REM REAL = REAL + BYTE
r2 = r1 + i2 \ REM REAL = REAL + INTEGER
r2 = r1 + r1 \ REM REAL = REAL + REAL
PRINT r2

(* STRING addition
(*s1 = s2 + b1 \ REM REAL = REAL + BYTE
(*s1 = s2 + i2 \ REM REAL = REAL + INTEGER
(*s1 = s2 + r1 \ REM REAL = REAL + REAL
s1 = "Darth Vader+"
s2 = "Addition Tests"
s3 = s1 + s2
PRINT s1
PRINT s2
PRINT s3
PRINT s3 + s3

