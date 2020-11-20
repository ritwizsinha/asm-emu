; Almost all the errors
ldc loop: ;Error for loop not defined
ldc 0xkkkkkk ;Invalid hex value
ldc090 ; Invalid octal value
label1:
label1: ; Repeated labels
90ax:   ; Bogus label
-3-3-... ; Unrecognized characters
ldc: ldc 200 ; Label name matches with mnemonic
sadjflkalsdjkflasd ; Random string
23123; Random value
ldc ; Without operand
a2sp 10 ; with operand
label3:: ; Two colons

HALT
var2: data 10 
var1: SET var2 ; Label used with set
var3: data var1 ; Label used with data
