; case1: binary addition x#y
; Tape0: input, then result
; Tape1: canonical x (leading zeros removed)
; Tape2: canonical y (leading zeros removed)

#Q = {0,clearXPre0,getX,meetPlus,yStart,clearYPre0,getY,plus,plus1,over,bad,badLeft,errPrint,errU,errP,errN,errI1,errGap,errL1,errA,errG,errE,errL2,errL3,errI2}
#S = {0,1,#}
#G = {0,1,#,_,i,l,e,g,a,n,p,u,t}
#q0 = 0
#B = _
#F = {over}
#N = 3

; ---------- x: validate and canonicalize ----------

0 0__ 0__ *** clearXPre0
0 1__ 1__ *** clearXPre0
0 #__ #__ *** bad
0 ___ ___ *** bad

clearXPre0 0__ ___ r** clearXPre0
clearXPre0 1__ 1__ *** getX
clearXPre0 #__ #0_ *** meetPlus
clearXPre0 ___ ___ *** bad

getX 0__ _0_ rr* getX
getX 1__ _1_ rr* getX
getX #__ #__ *l* meetPlus
getX ___ ___ *** bad

; consume '#', Tape1 head returns to rightmost x digit
meetPlus #*_ _*_ r** yStart

; ---------- y: validate and canonicalize ----------

yStart 0*_ _*_ r** clearYPre0
yStart 1*_ 1*_ *** getY
yStart #*_ #*_ *r* bad
yStart _*_ _*_ *r* bad

clearYPre0 0*_ _*_ r** clearYPre0
clearYPre0 1*_ 1*_ *** getY
clearYPre0 #*_ #*_ *r* bad
clearYPre0 _*_ _*0 *** plus

getY 0*_ _*0 r*r getY
getY 1*_ _*1 r*r getY
getY #*_ #*_ *r* bad
getY _*_ _*_ **l plus

; ---------- addition ----------
; Tape0 is blank and its head moves left while writing result bits.

plus _00 000 lll plus
plus _01 101 lll plus
plus _10 110 lll plus
plus _11 011 lll plus1
plus _0_ 00_ lll plus
plus _1_ 11_ lll plus
plus __0 0_0 lll plus
plus __1 1_1 lll plus
plus ___ ___ *** over

plus1 _00 100 lll plus
plus1 _01 001 lll plus1
plus1 _10 010 lll plus1
plus1 _11 111 lll plus1
plus1 _0_ 10_ lll plus
plus1 _1_ 01_ lll plus1
plus1 __0 1_0 lll plus
plus1 __1 0_1 lll plus1
plus1 ___ 1__ *** over

; ---------- invalid input ----------
; Every bad entry has Tape1 and Tape2 heads on blank cells.

bad 0__ 0__ r** bad
bad 1__ 1__ r** bad
bad #__ #__ r** bad
bad ___ ___ l** badLeft

badLeft 0__ ___ l** badLeft
badLeft 1__ ___ l** badLeft
badLeft #__ ___ l** badLeft
badLeft ___ ___ *** errPrint

; Write illegal_input backwards while moving left:
; t u p n i _ l a g e l l i  =>  illegal_input

errPrint ___ t__ l** errU
errU ___ u__ l** errP
errP ___ p__ l** errN
errN ___ n__ l** errI1
errI1 ___ i__ l** errGap
errGap ___ ___ l** errL1
errL1 ___ l__ l** errA
errA ___ a__ l** errG
errG ___ g__ l** errE
errE ___ e__ l** errL2
errL2 ___ l__ l** errL3
errL3 ___ l__ l** errI2
errI2 ___ i__ *** over
