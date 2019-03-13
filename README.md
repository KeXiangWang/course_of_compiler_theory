# Project written for the course of compiler theory.

#### Introduction
It is a C0 compiler for the course of compiler theory written in object-oriented C++. The grammar of C0 is given by the course group. The project was stored on gitee firstly and then transfered to github.

#### Programming Language and Environment
Written in object-oriented C++ 11.
Written with Visual Studio 2017 under Windows 10.

#### Grammar
```html
＜plusMinus＞ 	::= +｜-
＜multDiv＞  	::= *｜/
＜relationOption＞  ::=  <｜<=｜>｜>=｜!=｜==
＜letter＞   	::= ＿｜a｜．．．｜z｜A｜．．．｜Z
＜number＞   	::= ０｜＜notZeroInt＞
＜notZeroInt＞  	::= １｜．．．｜９
＜char＞    		::=  '＜plusMinus＞'｜'＜multDiv＞'｜'＜letter＞'｜'＜number＞'
＜string＞   	::=  "｛ASCII char, of which decimal presentation pertain to 32,33,35-126｝"                              
＜program＞    		::= ［＜constDeclaration＞］［＜varDeclaration＞］{＜retFuncDefination＞|＜voidFuncDefination＞}＜mainFunc＞
＜constDeclaration＞	::=  const＜constDefination＞;{ const＜constDefination＞;}
＜constDefination＞	::=   int＜ident＞＝＜integer＞{,＜ident＞＝＜integer＞}|char＜ident＞＝＜char＞{,＜ident＞＝＜char＞}
＜unsignedNum＞	::= ＜notZeroInt＞｛＜number＞｝｜０
＜integer＞		::= ［＋｜－］＜unsignedNum＞
＜ident＞		::=  ＜letter＞｛＜letter＞｜＜number＞｝
＜headDeclaration＞		::=  int＜ident＞|char＜ident＞
＜varDeclaration＞ 	::= ＜varDefination＞;{＜varDefination＞;}
＜varDefination＞	::= ＜type＞(＜ident＞|＜ident＞'['＜unsignedNum＞']'){,(＜ident＞|＜ident＞'['＜unsignedNum＞']') }  // ＜unsignedNum＞ shows the amount of element of array, which should bigger than 0.
＜type＞      	::=  int | char
＜retFuncDefination＞	::=  ＜headDeclaration＞'('＜paraList＞')' '{'＜compoundStatement＞'}'
＜voidFuncDefination＞	::= void＜ident＞'('＜paraList＞')''{'＜compoundStatement＞'}'
＜compoundStatement＞	::=  ［＜constDeclaration＞］［＜varDeclaration＞］＜stmtSequence＞
＜paraList＞ 		::=  ＜type＞＜ident＞{,＜type＞＜ident＞}| ＜NULL＞
＜mainFunc＞ 		::= void main'('')' '{'＜compoundStatement＞'}'
＜expression＞ 	::= ［＋｜－］＜term＞{＜plusMinus＞＜term＞}   //[+|-]only serve the first <term>
＜term＞ 		::= ＜factor＞{＜multDiv＞＜factor＞}
＜factor＞ 		::= ＜ident＞｜＜ident＞'['＜expression＞']'｜＜integer＞|＜char＞｜＜retFuncCall＞|'('＜expression＞')'
＜statement＞ 	::= ＜ifStatement＞｜＜loopStatement＞| '{'＜stmtSequence＞'}'｜＜retFuncCall＞;  |＜voidFuncCall＞;｜＜assignStatement＞;｜＜readStatement＞;｜＜writeStatement＞;｜＜NULL＞;｜＜returnStatement＞;
＜assignStatement＞	::=  ＜ident＞＝＜expression＞|＜ident＞'['＜expression＞']'=＜expression＞
＜ifStatement＞  ::=  if '('＜condition＞')'＜statement＞［else＜statement＞］
＜condition＞    ::=  ＜expression＞＜relationOption＞＜expression＞｜＜expression＞  // condition is false if only expression == 0
＜loopStatement＞   ::=  do＜statement＞while '('＜condition＞')' |for'('＜ident＞＝＜expression＞;＜condition＞;＜ident＞＝＜ident＞(+|-)＜step＞')'＜statement＞
＜step＞::= ＜unsignedNum＞  
＜retFuncCall＞ 	::= ＜ident＞'('＜valueParaList＞')'
＜voidFuncCall＞	::= ＜ident＞'('＜valueParaList＞')'
＜valueParaList>	::= ＜expression＞{,＜expression＞}｜＜NULL＞
＜stmtSequence＞	::=｛＜statement＞｝
＜readStatement＞	::=  scanf '('＜ident＞{,＜ident＞}')'
＜writeStatement＞	::=  printf'('＜string＞,＜expression＞')'|printf '('＜string＞')'|printf '('＜expression＞')'
＜returnStatement＞	::=  return['('＜expression＞')']
```

#### Addition Rules
（1）Var or const defined as char should be caculated in integer form that is transformed from ASCII form.
（2）Idents are case-sensitive.
（3）In writeStatements, strings should be outputed directly, vars or consts defined as char should be outputed as char and other expressions should be outputed as integer.
（4）Arrays start at 0.

#### Example
>C0 Code to Compile
```c
const int length = 10, width = +8;
const int firstLowerCase = 97, firstUpperCase = 64;
const int lastLowerCase = 123, lastUpperCase = 90;

int globalInt, globalIntArray[44], globalInt2, globalIntArray2[66], globalInt3;
int globalChar, globalCharArray[44], globalChar2, globalCharArray2[66], globalChar3;

int fib(int n, int k){
	int i;
	int res;
	if(n <= k){
		return (1);
	}
	else{
		res = 0;
		for(i = n - k; i < n; i = i + 1){
			res = res + fib(i, k);
		}
		return (res);
	}
}

void main(){ 
	int i;
	int operation;
	globalInt = 0;
	scanf(operation);
	if(operation == 0){
		printf(fib(5,3));
	}
	else{
		printf("no such option");
	}
}
```

>Mips Code 
```mips
.data
global_length: .word 10
global_width: .word 8
global_firstLowerCase: .word 97
global_firstUpperCase: .word 64
global_lastLowerCase: .word 123
global_lastUpperCase: .word 90
global_globalInt: .word 0
global_globalIntArray: .space 176
global_globalInt2: .word 0
global_globalIntArray2: .space 264
global_globalInt3: .word 0
global_globalChar: .word 0
global_globalCharArray: .space 176
global_globalChar2: .word 0
global_globalCharArray2: .space 264
global_globalChar3: .word 0
string_0: .asciiz  "no such option"
.text
j f_main




f_fib:
addiu $sp $sp -72
sw $ra 68($sp)
sw $fp 64($sp)
sw $s0 60($sp)
sw $s1 56($sp)
sw $s2 52($sp)
sw $s3 48($sp)
sw $s4 44($sp)
sw $s5 40($sp)
sw $s6 36($sp)
sw $s7 32($sp)
sw $a0 104($sp)
sw $a1 108($sp)
label_0:
lw $t0 104($sp)	#n
lw $t1 108($sp)	#k
# writeBack
sw $t0 104($sp)	# n
sw $t1 108($sp)	# k
# end writeBack
bgt $t0 $t1 label_1
# writeBack
# end writeBack
label_2:
li $v0 1	#const_int_5 : 1
j f_fib_return
# writeBack
# end writeBack
# writeBack
# end writeBack
label_1:
li $s1 0	# res
lw $t0 104($sp)	#n
lw $t1 108($sp)	#k
subu $t2 $t0 $t1 	# sub_0
move $s0 $t2	# i
# writeBack
sw $t0 104($sp)	# n
sw $t1 108($sp)	# k
sw $t2 12($sp)	# sub_0
# end writeBack
label_3:
lw $t0 104($sp)	#n
# writeBack
sw $t0 104($sp)	# n
# end writeBack
bge $s0 $t0 label_4
# writeBack
# end writeBack
# call func: fib
addiu $sp $sp -8
move $a0 $s0
lw $a1 116($sp)	#k
addiu $sp $sp -32
jal f_fib
addiu $sp $sp 32
addiu $sp $sp 8
lw $a0 104($sp)
lw $a1 108($sp)
move $t0 $v0
# back from func: fib
addu $t1 $s1 $t0 	# add_1
move $s1 $t1	# res
addiu $t2 $s0 1 	# add_2
move $s0 $t2	# i
# writeBack
sw $t0 8($sp)	# fcall_22
sw $t1 4($sp)	# add_1
sw $t2 0($sp)	# add_2
# end writeBack
j label_3
# writeBack
# end writeBack
label_4:
move $v0 $s1
j f_fib_return
# writeBack
# end writeBack
# writeBack
# end writeBack
label_5:
f_fib_return:
lw $s0 60($sp)
lw $s1 56($sp)
lw $s2 52($sp)
lw $s3 48($sp)
lw $s4 44($sp)
lw $s5 40($sp)
lw $s6 36($sp)
lw $s7 32($sp)
lw $ra 68($sp)
lw $fp 64($sp)
addiu $sp $sp 72
jr $ra


f_main:
addiu $sp $sp -52
label_6:
li $t0 0	#const_int_33 : 0
	# globalInt
sw $t0  global_globalInt	#globalInt
li $v0 5
syscall
move $s0 $v0
li $t1 0	#const_int_38 : 0
# writeBack
# end writeBack
bne $s0 $t1 label_7
# writeBack
# end writeBack
label_8:
# writeBack
# end writeBack
# call func: fib
addiu $sp $sp -8
li $a0 5	#const_int_41 : 5
li $a1 3	#const_int_42 : 3
addiu $sp $sp -32
jal f_fib
addiu $sp $sp 32
addiu $sp $sp 8
move $t0 $v0
# back from func: fib
li $v0 1  # printf int
move $a0 $t0
syscall
# writeBack
sw $t0 0($sp)	# fcall_43
# end writeBack
j label_9
# writeBack
# end writeBack
label_7:
li $v0 4  # printf string
la $a0 string_0
syscall
# writeBack
# end writeBack
label_9:
f_main_return:
```