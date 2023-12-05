; this TM program is used to make a simple multiplication function
; Input: a string of a's and b's, e.g. 'aaabb'

; the finite set of states
#Q = {check0,check1,check2,check3,mhc,start,mov2b,writec,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,halt_reject,halt_accept}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {c,I,l,e,g,a,_,n,p,u,t} ; cccccc or Illegal_Input

; the start state
#q0 = check0 

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

check0 a_ aa rr check1 ; check1: the first sym must be a
check0 b_ b_ ** reject ; reject state: will first move to the left and then clear the whole tape 
check0 __ __ ** reject

; check1 
check1 a_ aa rr check1
check1 b_ bb rr check2
check1 __ __ l* reject ; so no b

; check2: once see a b, no more a 
check2 b_ bb rr check2
check2 a_ a_ ** reject
check2 __ __ ll mhc ; valid, so move to the leftmost and start fxcking multiplying


; State mhc: mov&clear: the 2 heads to the left most, and clear the tape1
mhc ** _* ll mhc
mhc __ __ rr start

; State start: change the tape2's current a into _ 
start _a __ *r mov2b
start _b _b l* halt_accept 

; State mov2b: move to the first b
mov2b _a _a *r mov2b
mov2b _b _b ** writec

; State writec: write c on tape1
writec _b cb rr writec
writec __ __ *l mh ; so finished one round, go to leftmost

; State mh: move the second tape to the leftmost
mh _* _* *l mh
mh __ __ *r start


; State reject: move to the leftmost and clear the 1st tape
reject ** ** l* reject
reject *_ *_ l* reject
reject _* _* r* reject1
reject __ __ r* reject1

; State reject1: clear the whole tape 
reject1 ** _* r* reject1
reject1 _* _* ** r1
reject1 *_ __ r* reject1
reject1 __ _c ** r1

; State r*: write 'Illegal_Input' on 1st tape
r1 _* I* r* r2
r2 _* l* r* r3
r3 _* l* r* r4
r4 _* e* r* r5
r5 _* g* r* r6
r6 _* a* r* r7
r7 _* l* r* r8
r8 _* _* r* r9
r9 _* I* r* r10
r10 _* n* r* r11
r11 _* p* r* r12
r12 _* u* r* r13
r13 _* t* ** halt_reject
