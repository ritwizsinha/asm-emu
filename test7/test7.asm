; test07.asm
; program first stored a(3) into B and b(2) into A (after first two instuctions), then performs A := B << A;
; Final Result 2<<3 = 16
ldc a
ldc b	
shl
HALT
a: SET 3
b: SET 2