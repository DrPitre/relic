PROCEDURE main
DIM command : STRING

command = "procs"
PRINT "Executing ";command
SHELL command

command = "mdir -e"
PRINT "Executing ";command
SHELL command
