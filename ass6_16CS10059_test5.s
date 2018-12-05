	.file	"output.s"

.STR0:	.string ""
.STR1:	.string "--------------QUICKSORT TEST FILE---------------"
.STR2:	.string "Here we sort an array of elements"
.STR3:	.string "using recursive approach:"
.STR4:	.string "Enter the 10 elements of array "
.STR5:	.string "The sorted array (in ascending order) follows :"
.STR6:	.string ""
	.text
	.globl	quickSort
	.type	quickSort, @function
quickSort:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$264, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movq	%rdx, -16(%rbp)
# 0:res = i arg1 = lo 
	movl	-8(%rbp), %eax
	movl	%eax, -24(%rbp)
# 1:res = j arg1 = hi 
	movl	-4(%rbp), %eax
	movl	%eax, -28(%rbp)
# 2:res = t000 arg1 = lo arg2 = hi 
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -44(%rbp)
# 3:res = r arg1 = t000 
	movl	-44(%rbp), %eax
	movl	%eax, -20(%rbp)
# 4:res = t001 arg1 = t000 
	movl	-44(%rbp), %eax
	movl	%eax, -48(%rbp)
# 5:res = t002 
	movl	$2, -52(%rbp)
# 6:res = t003 arg1 = r arg2 = t002 
	movl	-20(%rbp), %eax
	cltd
	idivl	-52(%rbp), %eax
	movl	%eax, -56(%rbp)
# 7:res = r arg1 = t003 
	movl	-56(%rbp), %eax
	movl	%eax, -20(%rbp)
# 8:res = t004 arg1 = t003 
	movl	-56(%rbp), %eax
	movl	%eax, -60(%rbp)
# 9:res = t005 
	movl	$0, -64(%rbp)
# 10:res = t007 arg1 = r 
	movl	-20(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -72(%rbp)
# 11:res = t006 arg1 = t005 arg2 = t007 
	movl	-64(%rbp), %eax
	movl	-72(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -68(%rbp)
# 12:res = t008 arg1 = a arg2 = t006 
	movq	-16(%rbp), %rdx
	movslq	-68(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -76(%rbp)
# 13:res = x arg1 = t008 
	movl	-76(%rbp), %eax
	movl	%eax, -36(%rbp)
# 14:res = t009 arg1 = t008 
	movl	-76(%rbp), %eax
	movl	%eax, -80(%rbp)
# 15:res = t010 
.L9:
	movl	$0, -84(%rbp)
# 16:res = t012 arg1 = i 
	movl	-24(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -92(%rbp)
# 17:res = t011 arg1 = t010 arg2 = t012 
	movl	-84(%rbp), %eax
	movl	-92(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -88(%rbp)
# 18:res = t013 arg1 = a arg2 = t011 
	movq	-16(%rbp), %rdx
	movslq	-88(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -96(%rbp)
# 19:res = r arg1 = t013 
	movl	-96(%rbp), %eax
	movl	%eax, -20(%rbp)
# 20:res = t014 arg1 = t013 
	movl	-96(%rbp), %eax
	movl	%eax, -100(%rbp)
# 21:arg1 = r arg2 = x 
.L3:
	movl	-20(%rbp), %eax
	movl	-36(%rbp), %edx
	cmpl	%edx, %eax
	jl .L1
# 22:
	jmp .L2
# 23:
	jmp .L2
# 24:res = t015 arg1 = i 
.L1:
	movl	-24(%rbp), %eax
	movl	%eax, -104(%rbp)
# 25:res = i arg1 = i 
	movl	-24(%rbp), %eax
	movl	$1, %edx
	addl	%edx, %eax
	movl	%eax, -24(%rbp)
# 26:res = t016 
	movl	$0, -108(%rbp)
# 27:res = t018 arg1 = i 
	movl	-24(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -116(%rbp)
# 28:res = t017 arg1 = t016 arg2 = t018 
	movl	-108(%rbp), %eax
	movl	-116(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -112(%rbp)
# 29:res = t019 arg1 = a arg2 = t017 
	movq	-16(%rbp), %rdx
	movslq	-112(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -120(%rbp)
# 30:res = r arg1 = t019 
	movl	-120(%rbp), %eax
	movl	%eax, -20(%rbp)
# 31:res = t020 arg1 = t019 
	movl	-120(%rbp), %eax
	movl	%eax, -124(%rbp)
# 32:
	jmp .L3
# 33:res = t021 
.L2:
	movl	$0, -128(%rbp)
# 34:res = t023 arg1 = j 
	movl	-28(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -136(%rbp)
# 35:res = t022 arg1 = t021 arg2 = t023 
	movl	-128(%rbp), %eax
	movl	-136(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -132(%rbp)
# 36:res = t024 arg1 = a arg2 = t022 
	movq	-16(%rbp), %rdx
	movslq	-132(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -140(%rbp)
# 37:res = r arg1 = t024 
	movl	-140(%rbp), %eax
	movl	%eax, -20(%rbp)
# 38:res = t025 arg1 = t024 
	movl	-140(%rbp), %eax
	movl	%eax, -144(%rbp)
# 39:arg1 = r arg2 = x 
.L6:
	movl	-20(%rbp), %eax
	movl	-36(%rbp), %edx
	cmpl	%edx, %eax
	jg .L4
# 40:
	jmp .L5
# 41:
	jmp .L5
# 42:res = t026 arg1 = j 
.L4:
	movl	-28(%rbp), %eax
	movl	%eax, -148(%rbp)
# 43:res = j arg1 = j 
	movl	-28(%rbp), %eax
	movl	$1, %edx
	subl	%edx, %eax
	movl	%eax, -28(%rbp)
# 44:res = t027 
	movl	$0, -152(%rbp)
# 45:res = t029 arg1 = j 
	movl	-28(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -160(%rbp)
# 46:res = t028 arg1 = t027 arg2 = t029 
	movl	-152(%rbp), %eax
	movl	-160(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -156(%rbp)
# 47:res = t030 arg1 = a arg2 = t028 
	movq	-16(%rbp), %rdx
	movslq	-156(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -164(%rbp)
# 48:res = r arg1 = t030 
	movl	-164(%rbp), %eax
	movl	%eax, -20(%rbp)
# 49:res = t031 arg1 = t030 
	movl	-164(%rbp), %eax
	movl	%eax, -168(%rbp)
# 50:
	jmp .L6
# 51:arg1 = i arg2 = j 
.L5:
	movl	-24(%rbp), %eax
	movl	-28(%rbp), %edx
	cmpl	%edx, %eax
	jle .L7
# 52:
	jmp .L8
# 53:
	jmp .L8
# 54:res = t032 
.L7:
	movl	$0, -172(%rbp)
# 55:res = t034 arg1 = i 
	movl	-24(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -180(%rbp)
# 56:res = t033 arg1 = t032 arg2 = t034 
	movl	-172(%rbp), %eax
	movl	-180(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -176(%rbp)
# 57:res = t035 arg1 = a arg2 = t033 
	movq	-16(%rbp), %rdx
	movslq	-176(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -184(%rbp)
# 58:res = h arg1 = t035 
	movl	-184(%rbp), %eax
	movl	%eax, -40(%rbp)
# 59:res = t036 arg1 = t035 
	movl	-184(%rbp), %eax
	movl	%eax, -188(%rbp)
# 60:res = t037 
	movl	$0, -192(%rbp)
# 61:res = t039 arg1 = j 
	movl	-28(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -200(%rbp)
# 62:res = t038 arg1 = t037 arg2 = t039 
	movl	-192(%rbp), %eax
	movl	-200(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -196(%rbp)
# 63:res = t040 arg1 = a arg2 = t038 
	movq	-16(%rbp), %rdx
	movslq	-196(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -204(%rbp)
# 64:res = r arg1 = t040 
	movl	-204(%rbp), %eax
	movl	%eax, -20(%rbp)
# 65:res = t041 arg1 = t040 
	movl	-204(%rbp), %eax
	movl	%eax, -208(%rbp)
# 66:res = t042 
	movl	$0, -212(%rbp)
# 67:res = t044 arg1 = i 
	movl	-24(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -220(%rbp)
# 68:res = t043 arg1 = t042 arg2 = t044 
	movl	-212(%rbp), %eax
	movl	-220(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -216(%rbp)
# 69:res = a arg1 = t043 arg2 = r 
	movq	-16(%rbp), %rdx
	movslq	-216(%rbp), %rax
	addq	%rax, %rdx
	movl	-20(%rbp), %eax
	movl	%eax, (%rdx)
# 70:res = t045 arg1 = r 
	movl	-20(%rbp), %eax
	movl	%eax, -224(%rbp)
# 71:res = t046 
	movl	$0, -228(%rbp)
# 72:res = t048 arg1 = j 
	movl	-28(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -236(%rbp)
# 73:res = t047 arg1 = t046 arg2 = t048 
	movl	-228(%rbp), %eax
	movl	-236(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -232(%rbp)
# 74:res = a arg1 = t047 arg2 = h 
	movq	-16(%rbp), %rdx
	movslq	-232(%rbp), %rax
	addq	%rax, %rdx
	movl	-40(%rbp), %eax
	movl	%eax, (%rdx)
# 75:res = t049 arg1 = h 
	movl	-40(%rbp), %eax
	movl	%eax, -240(%rbp)
# 76:res = t050 arg1 = i 
	movl	-24(%rbp), %eax
	movl	%eax, -244(%rbp)
# 77:res = i arg1 = i 
	movl	-24(%rbp), %eax
	movl	$1, %edx
	addl	%edx, %eax
	movl	%eax, -24(%rbp)
# 78:res = t051 arg1 = j 
	movl	-28(%rbp), %eax
	movl	%eax, -248(%rbp)
# 79:res = j arg1 = j 
	movl	-28(%rbp), %eax
	movl	$1, %edx
	subl	%edx, %eax
	movl	%eax, -28(%rbp)
# 80:
	jmp .L8
# 81:arg1 = i arg2 = j 
.L8:
	movl	-24(%rbp), %eax
	movl	-28(%rbp), %edx
	cmpl	%edx, %eax
	jle .L9
# 82:
	jmp .L10
# 83:
	jmp .L10
# 84:arg1 = lo arg2 = j 
.L10:
	movl	-8(%rbp), %eax
	movl	-28(%rbp), %edx
	cmpl	%edx, %eax
	jl .L11
# 85:
	jmp .L12
# 86:
	jmp .L12
# 87:res = t052 
.L11:
	movl	$0, -252(%rbp)
# 88:res = a 
# 89:res = lo 
# 90:res = j 
# 91:res = t053 
	pushq %rbp
	movl	-28(%rbp) , %edi
	movl	-8(%rbp) , %esi
	movq	-16(%rbp), %rdx
	call	quickSort
	movl	%eax, -256(%rbp)
	addq $0 , %rsp
# 92:
	jmp .L12
# 93:arg1 = i arg2 = hi 
.L12:
	movl	-24(%rbp), %eax
	movl	-4(%rbp), %edx
	cmpl	%edx, %eax
	jl .L13
# 94:
	jmp	.LRT0
# 95:
	jmp	.LRT0
# 96:res = t054 
.L13:
	movl	$0, -260(%rbp)
# 97:res = a 
# 98:res = i 
# 99:res = hi 
# 100:res = t055 
	pushq %rbp
	movl	-4(%rbp) , %edi
	movl	-24(%rbp) , %esi
	movq	-16(%rbp), %rdx
	call	quickSort
	movl	%eax, -264(%rbp)
	addq $0 , %rsp
# 101:
	jmp	.LRT0
.LRT0:
	addq	$-264, %rsp
	movq	%rbp, %rsp
	popq	%rbp
	ret
.LFE0:
	.size	quickSort, .-quickSort
	.globl	main
	.type	main, @function
main:
.LFB1:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$568, %rsp
# 102:res = t056 arg1 = c 
.L14:
	leaq	-4(%rbp), %rax
	movq	%rax, -32(%rbp)
# 103:res = ep arg1 = t056 
	movq	-32(%rbp), %rax
	movq	%rax, -24(%rbp)
# 104:res = t057 arg1 = t056 
	movq	-32(%rbp), %rax
	movq	%rax, -40(%rbp)
# 105:res = t058 
	movl	$100, -444(%rbp)
# 106:
	movq	$.STR0,	%rdi
# 107:res = t059 
	pushq %rbp
	call	printStr
	movl	%eax, -448(%rbp)
	addq $8 , %rsp
# 108:
	movq	$.STR1,	%rdi
# 109:res = t060 
	pushq %rbp
	call	printStr
	movl	%eax, -452(%rbp)
	addq $8 , %rsp
# 110:
	movq	$.STR2,	%rdi
# 111:res = t061 
	pushq %rbp
	call	printStr
	movl	%eax, -456(%rbp)
	addq $8 , %rsp
# 112:
	movq	$.STR3,	%rdi
# 113:res = t062 
	pushq %rbp
	call	printStr
	movl	%eax, -460(%rbp)
	addq $8 , %rsp
# 114:
	movq	$.STR4,	%rdi
# 115:res = t063 
	pushq %rbp
	call	printStr
	movl	%eax, -464(%rbp)
	addq $8 , %rsp
# 116:res = t064 
	movl	$0, -468(%rbp)
# 117:res = i arg1 = t064 
	movl	-468(%rbp), %eax
	movl	%eax, -12(%rbp)
# 118:res = t065 arg1 = t064 
	movl	-468(%rbp), %eax
	movl	%eax, -472(%rbp)
# 119:res = t066 
.L17:
	movl	$10, -476(%rbp)
# 120:arg1 = i arg2 = t066 
	movl	-12(%rbp), %eax
	movl	-476(%rbp), %edx
	cmpl	%edx, %eax
	jl .L15
# 121:
	jmp .L16
# 122:
	jmp .L16
# 123:res = t067 arg1 = i 
.L18:
	movl	-12(%rbp), %eax
	movl	%eax, -480(%rbp)
# 124:res = i arg1 = i 
	movl	-12(%rbp), %eax
	movl	$1, %edx
	addl	%edx, %eax
	movl	%eax, -12(%rbp)
# 125:
	jmp .L17
# 126:res = ep 
.L15:
# 127:res = t068 
	pushq %rbp
	movq	-24(%rbp), %rdi
	call	readInt
	movl	%eax, -484(%rbp)
	addq $0 , %rsp
# 128:res = r arg1 = t068 
	movl	-484(%rbp), %eax
	movl	%eax, -16(%rbp)
# 129:res = t069 arg1 = t068 
	movl	-484(%rbp), %eax
	movl	%eax, -488(%rbp)
# 130:res = t070 
	movl	$0, -492(%rbp)
# 131:res = t072 arg1 = i 
	movl	-12(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -500(%rbp)
# 132:res = t071 arg1 = t070 arg2 = t072 
	movl	-492(%rbp), %eax
	movl	-500(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -496(%rbp)
# 133:res = a arg1 = t071 arg2 = r 
	leaq	-440(%rbp), %rdx
	movslq	-496(%rbp), %rax
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	movl	%eax, (%rdx)
# 134:res = t073 arg1 = r 
	movl	-16(%rbp), %eax
	movl	%eax, -504(%rbp)
# 135:
	jmp .L18
# 136:res = t074 
.L16:
	movl	$0, -508(%rbp)
# 137:res = t075 
	movl	$0, -512(%rbp)
# 138:res = t076 
	movl	$9, -516(%rbp)
# 139:res = a 
# 140:res = t075 
# 141:res = t076 
# 142:res = t077 
	pushq %rbp
	movl	-516(%rbp) , %edi
	movl	-512(%rbp) , %esi
	leaq	-440(%rbp), %rdx
	call	quickSort
	movl	%eax, -520(%rbp)
	addq $0 , %rsp
# 143:
	movq	$.STR5,	%rdi
# 144:res = t078 
	pushq %rbp
	call	printStr
	movl	%eax, -524(%rbp)
	addq $8 , %rsp
# 145:res = t079 
	movl	$0, -528(%rbp)
# 146:res = i arg1 = t079 
	movl	-528(%rbp), %eax
	movl	%eax, -12(%rbp)
# 147:res = t080 arg1 = t079 
	movl	-528(%rbp), %eax
	movl	%eax, -532(%rbp)
# 148:res = t081 
.L21:
	movl	$10, -536(%rbp)
# 149:arg1 = i arg2 = t081 
	movl	-12(%rbp), %eax
	movl	-536(%rbp), %edx
	cmpl	%edx, %eax
	jl .L19
# 150:
	jmp	.LRT1
# 151:
	jmp	.LRT1
# 152:res = t082 arg1 = i 
.L22:
	movl	-12(%rbp), %eax
	movl	%eax, -540(%rbp)
# 153:res = i arg1 = i 
	movl	-12(%rbp), %eax
	movl	$1, %edx
	addl	%edx, %eax
	movl	%eax, -12(%rbp)
# 154:
	jmp .L21
# 155:res = t083 
.L19:
	movl	$0, -544(%rbp)
# 156:res = t085 arg1 = i 
	movl	-12(%rbp), %eax
	movl	$4, %ecx
	imull	%ecx, %eax
	movl	%eax, -552(%rbp)
# 157:res = t084 arg1 = t083 arg2 = t085 
	movl	-544(%rbp), %eax
	movl	-552(%rbp), %edx
	addl	%edx, %eax
	movl	%eax, -548(%rbp)
# 158:res = t086 arg1 = a arg2 = t084 
	leaq	-440(%rbp), %rdx
	movslq	-548(%rbp), %rax
	addq	%rax, %rdx
	movl	(%rdx), %eax
	movl	%eax, -556(%rbp)
# 159:res = r arg1 = t086 
	movl	-556(%rbp), %eax
	movl	%eax, -16(%rbp)
# 160:res = t087 arg1 = t086 
	movl	-556(%rbp), %eax
	movl	%eax, -560(%rbp)
# 161:res = r 
# 162:res = t088 
	pushq %rbp
	movl	-16(%rbp) , %edi
	call	printInt
	movl	%eax, -564(%rbp)
	addq $0 , %rsp
# 163:
	movq	$.STR6,	%rdi
# 164:res = t089 
	pushq %rbp
	call	printStr
	movl	%eax, -568(%rbp)
	addq $8 , %rsp
# 165:
	jmp .L22
.LRT1:
	addq	$-568, %rsp
	movq	%rbp, %rsp
	popq	%rbp
	ret
.LFE1:
	.size	main, .-main
