ldr r0, =0x20200004
ldr r2, =0x00040000
str r2, [r0]
ldr r0, =0x20200028
ldr r1, =0x2020001C
ldr r2, =0x00010000
str r2, [r0]
loop: 
str r2, [r1]
ldr r3, =0x01000000
wait_to_clear:
sub r3, r3, #1
cmp r3, #0
bne wait_to_clear
str r2, [r0]
ldr r3, =0x01000000
wait_to_set:
sub r3, r3, #1
cmp r3, #0
bne wait_to_set
b loop
end_loop:
andeq r0, r0, r0
