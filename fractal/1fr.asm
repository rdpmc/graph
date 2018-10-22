CPU P4 
BITS 32
ideal
%include "W_CON.INC"
%include 'nasm_externs.inc'
%include 'nasm_macros.inc'

K equ 256;N colors
M equ 100
NDLTS equ 0
[segment code public use32 class='CODE']
..start:
cld
p SM_CXSCREEN
cal GetSystemMetrics
mov dword [dibh],eax
inc eax
mov dword [a],eax
p SM_CYSCREEN
cal GetSystemMetrics
mov dword [dibw],eax
inc eax
mov dword [b],eax

;
p0
p eax
p eax ;nmdev
p nmdrv
cal CreateDCA
mov [hdc],eax
mov dword [ndlts],NDLTS
jmp jul0
p eax ;0
cal CreateCompatibleDC
mov dword [hcdc],eax

p 0
p 0
p pvbits
p DIB_RGB_COLORS
p dib
p 0
cal CreateDIBSection
mov dword [hbitmap],eax
p eax
p dword [hcdc]
cal SelectObject
mov ebp,eax
mov edi,[pvbits]
xor eax,eax
mov ecx,100000h
lll:
stosd
add eax,4
loop lll

p SRCCOPY
p 0
p 0
p dword [hcdc]
p dword [b]
p dword [a]
p 0
p 0
p dword [hdc]
cal BitBlt
p ebp
p dword [hcdc]
cal SelectObject


jmp exit

mov ebp,16		;!!!!!!!!!!!!
jmp jul
mov word [pal],300h
mov word [paln],256
mov edi,palen
mov eax,2000000h;PC_EXPLICIT
mov ecx,0ffh
fillp:
stosd
;inc ah
loop fillp
p pal
cal CreatePalette
mov [hpal],eax
p 0
p eax
p dword [hdc]
cal SelectPalette 
push dword [hdc]
cal  RealizePalette
;			jmp jul
nopal:
xor edi,edi
xor esi,esi
xor ebx,ebx
lpix:

p ebx
p edi
p esi
p dword [hdc]
cal SetPixel
inc ebx
cmp bl,0
jne bl1
xor ebx,ebx
bl1:
inc esi
cmp esi,[a]
jne lpix
inc edi
xor esi,esi
cmp edi,[b]
jne lpix

;;;;;;;;;;;;;;;;;
exit:

p dword [hdc]
cal  DeleteDC
p dword [hpal]
cal DeleteObject
p0
cal ExitProcess
;;;;;;;;;;
magnet_x:










;;;;;;;;;*******************************************************************************************
jul0:
mov ebp,N_JU
jul:
xor eax,eax
mov eax,[a]
dec eax
finit
fld dword [xmin +4] ;xmax
fld dword [xmin]
fsubp st1
p eax
fild dword [esp]
pop eax
fdivp st1
fstp dword [dx_]
;
mov eax,[b]
dec eax
fld dword [xmin+4+4+4] ;ymax
fld dword [xmin+4+4] ;ymin
fsubp st1
p eax
fild dword [esp]
pop eax
fdivp st1
fstp dword [dy_]
mov ecx,[a]
dec ecx ;np
jl0:
p ecx
mov ecx,[b]
dec ecx ;nq
jl1:
p ecx
;;;;;;;;;;;;;;;1
mov eax,[a]
dec eax
sub eax,[esp+4]
mov ebx,[b]
dec ebx
sub ebx,[esp]
p eax
p ebx
fild dword [esp+4]
fld dword [dx_]
fmulp st1
fld dword [xmin]
faddp st1
fstp dword [xk]
;
fild dword [esp]
fld dword [dy_]
fmulp st1
fld dword [xmin+8] ;qmin
faddp st1
fstp dword [yk]
pop eax
pop eax
;
xor eax,eax
mov dword [k],eax
;;;;;;;;;;;;;;;;2
;x(k+1):
jm2:
fld dword [pq]
fld dword [xk]
fld st0
fmulp st1
fld dword [yk]
fld st0
fmulp st1
fsubp st1
faddp st1
p eax
fstp dword [esp]
;y(k+1)
fld dword [pq+4]
fld dword [xk]
fld dword [yk]
fmulp st1
fld st0
faddp st1
faddp st1
pop dword [xk]
fstp dword [yk]
;k++
inc dword [k]
;;;;;;;;;;;;;;;;;;3
fld dword [xk]
fld dword [yk]
fld st0
fmulp st1
fld st1
fmulp st2
faddp st1 ;r
p M
fild dword [esp]
fxch
fcompp ;r ? M
fstsw ax 
pop ecx

xor edx,edx
mov ebx,dword [k]
cmp ebx,K
je jm4
mov edx,ebx
sahf
jc jm2;r<=M
nop
jm4:
mov ecx,dword [a]
dec ecx
sub ecx,[esp+4]
mov ebx,[b]
dec ebx
sub ebx,[esp]
;edx=color
;test edx,edx
;jz noch
jmp jnoch
mov eax,[esp]
and eax,0ffh
add edx,eax
mov eax,[esp+4]
and eax,0ffh
add edx,eax
jnoch:
call putpix
pop ecx
dec ecx
jnz near jl1
pop ecx
dec ecx
jnz near jl0
;jmp exit
mov eax,ebp
dec eax
shl eax,3
mov edi,pq
lea esi,[eax+edi]
;int3
p edi
movsd
movsd
pop edi
;call do_rnd
dec ebp
jz fr0
;p 5000
;cal Sleep
jmp jul



fr0:

mov ebp,N_FR

;;;;;;;;;*******************************************************************************************
fr:
xor eax,eax
mov eax,[a]
dec eax
finit
fld dword [pmin +4] ;pmax
fld dword [pmin]
fsubp st1
p eax
fild dword [esp]
pop eax
fdivp st1
fstp dword [dp_]
;
mov eax,[b]
dec eax
fld dword [pmin+4+4+4] ;qmax
fld dword [pmin+4+4] ;qmin
fsubp st1
p eax
fild dword [esp]
pop eax
fdivp st1
fstp dword [dq_]
;
mov ecx,[a]
dec ecx ;np
l0:
p ecx
mov ecx,[b]
dec ecx ;nq
l1:
p ecx
;;; 1
mov eax,[a]
dec eax
sub eax,[esp+4]
mov ebx,[b]
dec ebx
sub ebx,[esp]
p eax
p ebx
fild dword [esp+4]
fld dword [dp_]
fmulp st1
fld dword [pmin]
faddp st1
fstp dword [p_]
;
fild dword [esp]
fld dword [dq_]
fmulp st1
fld dword [pmin+8] ;qmin
faddp st1
fstp dword [q_]
pop eax
pop eax
;
xor eax,eax
mov dword [k],eax
mov dword [xk],eax
mov dword [yk],eax
;;; 2
;x(k+1):
m2:
fld dword [p_]
fld dword [xk]
fld st0
fmulp st1
fld dword [yk]
fld st0
fmulp st1
fsubp st1
faddp st1
p eax
fstp dword [esp]
;y(k+1)
fld dword [q_]
fld dword [xk]
fld dword [yk]

fmulp st1

fld st0
faddp st1
faddp st1
pop dword [xk]
fstp dword [yk]
;k++
inc dword [k]
;;; 3
fld dword [xk]
fld dword [yk]
fld st0
fmulp st1
fld st1
fmulp st2
faddp st1 ;r
p M
fild dword [esp]
fxch
fcompp ;r ? M
fstsw ax 
pop ecx

xor edx,edx
mov ebx,dword [k]
cmp ebx,K
je m4
mov edx,ebx
sahf
jc m2;r<=M
nop
m4:
mov ecx,dword [a]
dec ecx
sub ecx,[esp+4]
mov ebx,[b]
dec ebx
sub ebx,[esp]
;edx=color
test edx,edx
jz noch
jmp noch
mov eax,[esp]
and eax,0ffh
add edx,eax
mov eax,[esp+4]
and eax,0ffh
add edx,eax
noch:
call putpix
pop ecx
dec ecx
jnz near l1
pop ecx
dec ecx
jnz near l0
;jmp exit
cmp dword [ndlts],0
je nd1
;int3
jmp nomul
fld dword [dltmul]
fld dword [pmin]
fld dword [pmin+4]
fld dword [pmin+8]
fld dword [pmin+0ch]
fmul st4
fstp dword [pmin+0ch]
fmul st3
fstp dword [pmin+8]
fmul st2
fstp dword [pmin+4]
fmulp st1
fstp dword [pmin]
dec dword [ndlts]
jmp fr
nomul:

fld dword [dlt]
fld dword [pmin]
fld dword [pmin+4]
fsub st2
fstp dword [pmin+4]
fsub st1
fstp dword [pmin]
dec dword [ndlts]
ffree st0
jmp fr
nd1:
mov dword [ndlts],NDLTS
mov eax,ebp
dec eax
shl eax,4
mov edi,pmin
lea esi,[eax+edi]
movsd
movsd
movsd
movsd
;
dec ebp
jz exit
jmp fr


;;;;;;;;;*******************************************************************************************

putpix:
;shl edx,3
p edx
p ebx ;y
p ecx ;x
p dword [hdc]
cal SetPixel
ret
;;;;;;;;
do_rnd:
;edi=ptr
rdtsc
xor eax,edx
p eax
cal GetTickCount 
pop ecx
rol eax,cl
xor eax,ecx
mov ebx,eax
and eax,0ffffh
shr ebx,16
and ebx,0ffffh
p eax
p ebx
fild dword [esp]
fild dword [esp+4]
pop eax
pop eax
fcos
fstp dword [edi]
fsin
fstp dword [edi+4]
ret


;;;;;;;;;;;;
patterns:
;
add ebp,edi
sub ebp,esi
lea ecx,[edi+esi]
and ecx,1b
rol ebp,cl ; ror ebp,cl

;

[segment data public use32 class='CODE']
;;;;;;;;;;;JULIA
pq dd -0.74543,0.11301
dd -0.11,0.67
dd -0.12,0.74
dd -0.12375,0.56508
dd -0.481762,-0.531657
dd -0.39054,-0.58679
dd 0.27334,0.00742
dd -1.25,0
dd -0.11,0.6557
dd 0.11031,-0.67037
dd 0,1.0
dd -0.194,0.6557
dd -0.15652,1.03225
dd -0.12375,0.56508
dd -0.39054,-0.58679
dd 0.32,0.043
;dd -0.174543,0.211301

N_JU equ ($-pq)/8


xmin dd -1.5
xmax dd 1.5
ymin dd -1.5
ymax dd 1.5








;;;;;;;;MANDELBROT
;pmin,pmax,qmin,qmax
;pmin dd -0.666 ,-0.667 ,0.666 ,1.6671
pmin  dd -0.713 ,-0.4082 ,0.49216 ,0.71429

dd -0.95,-0.88333,0.23333,0.3


dd -1.781
dd -1.764
dd 0
dd 0.013

dd -0.74758
dd -0.74624
dd 0.10671
dd 0.10779

dd -0.746541
dd -0.746378
dd 0.107574
dd 0.107678


dd -0.74591
dd  -0.74448
dd 0.11196
dd 0.11339

dd -0.745538
dd -0.745054
dd 0.112881
dd 0.113236


dd -0.745468
dd -0.745385
dd 0.112979
dd 0.113039
;;;;;;;;;;;;;;;;;;;;
;dd -0.7454356
;dd -0.7454215
;dd 0.1130037
;dd 0.1130139

;dd -0.7454301
;dd -0.7454289
;dd 0.1130076
;dd 0.1130085
;;;;;;;;;;;;;;
dd -1.254024
dd -1.252861
dd 0.046252
dd 0.047125

;;;;;;;;;;;;;;;;;;;
dd -0.19920
dd -0.12954
dd 1.0148
dd 1.06707

dd -0.75104
dd -0.7408
dd 0.10511
dd 0.11536



dd -2.25
dd 0.75
dd -1.5
dd 1.5

dd -2.305
dd 0.743
dd -1.467
dd 1.233


N_FR equ ($-pmin)/16

dib:
dd LEN_DIB
dibh dd 0
dibw dd 0
dw 1
dw 32
dd BI_RGB	
dd 0
dd a
dd b
dd 0
dd 0
LEN_DIB equ $-dib
dd 0 ;?colors[1]
nmdev db 'fractal_dev1',0
nmdrv db 'DISPLAY',0
hdib dd 0
dlt dd 0.00377
dltmul dd 1.21
[SECTION .bss use32]
ndlts resd 1
pvbits resd 1
hbitmap resd 1
hcdc resd 1
hdc resd 1
hpal resd 1
pdibmas resd 1
p_ resd 1
q_ resd 1

dp_ resd 1
dq_ resd 1

p0 resd 1
q0 resd 1
xk resd 1
yk resd 1
k resd 1
dx_ resd 1
dy_ resd 1

a resd 1 ;X_SCREEN_RESOL
b resd 1
pal resw 1
paln resw 1
palen resd 256