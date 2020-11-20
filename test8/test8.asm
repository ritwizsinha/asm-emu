;Almost all warnings
ldc label1 ; label1 is a breakpoint not a variable

label1: ldc 10 
br next ; Redundant label
next: 
loop: br loop ;Infinite loop

num1: data 0xffffffffff ; data before halt cause problems


HALT ; Multiple halts
HALT
adc 79274589742359 ; Code after HALT redundant
label389: ;Unused label
num2: data 0777777777777; Overflow
num3: data 89123479810273489; Overflow
num4: data -8479189234719; Overflow
num5: data -1919191919; Overflow
num6: SET 0xffffffffff; Overflow
num7: SET 0777777777777; Overflow
num8: SET 89123479810273489; Overflow
num9: SET -8479189234719; Overflow
num10: SET -1919191919; Overflow