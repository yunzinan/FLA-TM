; this program is designed for the string pattern, w = ucu, where u \in \{a, b\}^*
; @author: yunzinan
; @date: 2023/12/5
; Input: a string of a's, b's and c's, e.g. 'abbbacabbba'
; Sketch: 
; 1. find the only c, make sure it is the only c in input string
; 2. copy the substring in the right of c to the second tape
; 3. move the first tape to c, and clear sym on tape1, tape2 stay still at the rightmost place
; 4. start comparing the first and the second tape from the right to the left, clear them in the meantime
; 5.1 if difference found, then goto reject(continues to go left, clear in the meantime, then write false)
; 5.2 if no diff found, then goto accept(write true, since now the two tapes are already cleared)

; the finite set of states
#Q = {check0,check1,1movl,r1,r2,r3,r4,r5,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {a,b,c}

; the complete set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = check0 

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions


; State check0: move right to find c
check0 a_ a_ r* check0
check0 b_ b_ r* check0
check0 c_ c_ r* check1
check0 __ __ l* reject2 ; no c found till end of string 

; State check1: c found once, copy the right part to the second tape, in the meantime, check if there is another c
check1 a_ aa rr check1
check1 b_ bb rr check1
check1 c_ c_ ** reject ; another c found
check1 __ __ ll 1movl ; tape2 stay at rigthmost, tape1 move left to until c, and clear in the way  

; State reject: first go to the rightmost on the tape1, then clear to the left, then write 'false'
reject ** ** r* reject
reject *_ *c r* reject
reject _* _* l* reject2
reject __ __ l* reject2 



; State reject2: clear to the left, and then write false
reject2 ** _* l* reject2
reject2 *_ _c l* reject2
reject2 _* _* ** r1
reject2 __ __ ** r1 

; State 1movl: tape1 from the rightmost to c, and clear the right part, tape2 not moving 
1movl a* _* l* 1movl
1movl b* _* l* 1movl
1movl c* _* l* cmp
1movl a_ __ l* 1movl
1movl b_ __ l* 1movl
1movl c_ __ l* cmp

; State cmp: compare two strings
cmp aa __ ll cmp
cmp bb __ ll cmp
cmp ab __ ll reject2 ; clear to the left, and then write false
cmp a_ __ l* reject2
cmp ba __ ll reject2
cmp b_ __ l* reject2
cmp _a __ ** reject2
cmp _b __ ** reject2
cmp __ __ ** accept


; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State r*: write 'false' on 1st tape
r1 _* f* r* r2
r2 _* a* r* r3
r3 _* l* r* r4
r4 _* s* r* r5
r5 _* e* ** halt_reject
r1 __ f_ r* r2
r2 __ a_ r* r3
r3 __ l_ r* r4
r4 __ s_ r* r5
r5 __ e_ ** halt_reject