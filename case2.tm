; case2: decide L = { ww | w in {0,1}* }
; Tape0: input, later true/false
; Tape1: copy of input; after midpoint scan its head stays at n/2

#Q = {copyStart,copy,rewindL,scanA,scanB,afterB,backT0,cmp,falseEntry,clearEndTrue,clearBackTrue,writeTrueT,writeTrueR,writeTrueU,writeTrueE,clearEndFalse,clearBackFalse,writeFalseF,writeFalseA,writeFalseL,writeFalseS,writeFalseE,over}
#S = {0,1}
#G = {0,1,_,t,r,u,e,f,a,l,s}
#q0 = copyStart
#B = _
#F = {over}
#N = 2

; ---------- copy input to Tape1; empty input is true ----------

copyStart 0_ 00 rr copy
copyStart 1_ 11 rr copy
copyStart __ __ ** clearEndTrue

copy 0_ 00 rr copy
copy 1_ 11 rr copy
copy __ __ ll rewindL

; both tapes move to index -1, then back to 0
rewindL 00 00 ll rewindL
rewindL 11 11 ll rewindL
rewindL __ __ rr scanA

; ---------- find midpoint: Tape0 two steps, Tape1 one step ----------
; invariant before scanA: Tape0 at 2k, Tape1 at k

scanA 0* 0* rr scanB
scanA 1* 1* rr scanB

; Tape0 has moved one step; if it is blank now, length is odd
scanB _0 _0 ** falseEntry
scanB _1 _1 ** falseEntry
scanB __ __ ** falseEntry
scanB 0* 0* r* afterB
scanB 1* 1* r* afterB

; after the second Tape0 step, blank means even length and Tape1 is at n/2
afterB _* _* l* backT0
afterB 0* 0* ** scanA
afterB 1* 1* ** scanA

; ---------- move only Tape0 back to 0; Tape1 stays at midpoint ----------

backT0 0* 0* l* backT0
backT0 1* 1* l* backT0
backT0 _* _* r* cmp

; ---------- compare first half with second half ----------

cmp 00 00 rr cmp
cmp 11 11 rr cmp
cmp 01 01 ** falseEntry
cmp 10 10 ** falseEntry

; Tape1 finished: all characters matched
cmp 0_ 0_ ** clearEndTrue
cmp 1_ 1_ ** clearEndTrue
cmp __ __ ** clearEndTrue

; Tape0 finished before Tape1: length mismatch
cmp _0 _0 ** falseEntry
cmp _1 _1 ** falseEntry

; ---------- false path: park Tape1 on blank first ----------

falseEntry *0 *0 *r falseEntry
falseEntry *1 *1 *r falseEntry
falseEntry _0 _0 *r falseEntry
falseEntry _1 _1 *r falseEntry

falseEntry 0_ 0_ ** clearEndFalse
falseEntry 1_ 1_ ** clearEndFalse
falseEntry __ __ ** clearEndFalse

; ---------- clear Tape0 and write true ----------

clearEndTrue 0_ 0_ r* clearEndTrue
clearEndTrue 1_ 1_ r* clearEndTrue
clearEndTrue __ __ l* clearBackTrue

clearBackTrue 0_ __ l* clearBackTrue
clearBackTrue 1_ __ l* clearBackTrue
clearBackTrue __ __ ** writeTrueT

writeTrueT __ t_ r* writeTrueR
writeTrueR __ r_ r* writeTrueU
writeTrueU __ u_ r* writeTrueE
writeTrueE __ e_ ** over

; ---------- clear Tape0 and write false ----------

clearEndFalse 0_ 0_ r* clearEndFalse
clearEndFalse 1_ 1_ r* clearEndFalse
clearEndFalse __ __ l* clearBackFalse

clearBackFalse 0_ __ l* clearBackFalse
clearBackFalse 1_ __ l* clearBackFalse
clearBackFalse __ __ ** writeFalseF

writeFalseF __ f_ r* writeFalseA
writeFalseA __ a_ r* writeFalseL
writeFalseL __ l_ r* writeFalseS
writeFalseS __ s_ r* writeFalseE
writeFalseE __ e_ ** over
