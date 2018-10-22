CPU P4 
BITS 32
get_avg1:
;дать среднюю длину элемента объекта-->eax (int)<0ffh
pop eax
finit
fld dwo [esp]
pop ebx
p eax
mov ebx,dwo [maxx]
add ebx,dwo [maxy]
shr ebx,1
p ebx
fild dwo [esp]
fmulp st1,st0
frndint
fistp dwo [esp]
pop eax
and eax,0ffh
ret
;;;;;;;;;;;;;;;;;;;; 
cre_obj: ;for D3DFVF_XYZ 
;создает объект типа eax, длины сторон в ebx,
;координаты=есх, цвет=edx
ID_TREUG equ 0 ;равнобедр треуг
ID_CUBE equ 1
ID_SETKA equ 2
ID_PRLPD equ 3 ;прямоуг паралелепипед
;
mov ebp,CUR_FVF
mov dwo [fvf],ebp
cmp ebp,D3DFVF_XYZ+ D3DFVF_NORMAL+D3DFVF_TEX1
je on1
ret
on1:
mov edi,dwo [of_free]
cld
p ebx
p ecx
p edx
mov esi,pr
call dwo [cre_obj_xref+eax*4]
pop edx
pop ecx
pop ebx
ret
cre_obj_xref:
 dd onID_TREUG
 dd onID_CUBE
 dd onID_SETKA
 dd onID_PRLPD
onID_TREUG:
call put_fst_vx
mov eax,0ff0000h ;{0,0,-1}
call cre_normal
mov dl,bl
fld dwo [esi]
p edx
fild dwo [esp]
pop edx
fadd st1
fstp dwo [edi]
shr edx,1
p edx
fild dwo [esp]
pop edx
faddp st1,st0 
p edi
cmpsd
movsd ;y1=y0
movsd ;z1=z0
mov eax,000ff00h 
call cre_normal
pop esi ;x1
mov dl,bh
fst dwo [edi] ;x2
p edx
fild dwo [esp] ;st0=a
pop edx
fld st0
fmulp st1,st0
;a2,x2
fxch
fld dwo [esi]
fsubp st1
fld st0
fmulp st1
fsubp st1,st0
fabs
fsqrt
fadd dwo [esi+4] ;тр смотрит  вверх; fsub - вниз
fstp dwo [edi+4]
add esi,8
add edi,8
movsd
mov eax,0ffh 
call cre_normal
mov dwo [of_free],edi
inc dwo [n_prim]
inc dwo [n_obj_prim]
ret
;;;;;;;;;;;;;;;
onID_CUBE: ;edx=side
call put_fst_vx
mov edx,ebx
mov esi,cube1
mov ecx,cube1_len/4
fld dwo [edi-04h]
fld dwo [edi-08h]
fld dwo [edi-0ch] ;st1=x,2,3
p edi
p ecx
movsd
movsd
movsd
sub ecx,3
oc_1:
mov eax,dwo [fvf]
and eax,D3DFVF_TEX1
jz notx
xor eax,eax
stosd
stosd
notx:
p ecx
mov ecx,6
rep movsd
pop ecx
sub ecx,6
jcxz oc_
jmp oc_1
oc_:
stosd
stosd
pop ecx
pop esi
add esi,3*4+2*4
sub ecx,3
xor ebx,ebx
dec ebx
p edx
fild dwo [esp]
pop eax
oncul1:
lodsd
cf 10.0
jne cu2
fld st1
jmp cu4_
cu2:
cf 11.0
jne cu3
fld st2
jmp cu4_
cu3:
cf 12.0
jne cu4
fld st3
cu4_:
fadd st0,st1
fstp dwo [esi-4]
jmp cuinc
cu4:
test ebx,ebx
js cu4s
jz cu4z
dec ebx
jz cu4nz
cubk1:
inc ebx
jmp cuinc
cu4nz:
fxch st3
fst dwo [esi-4]
fxch st3
jmp cubk1
cuinc:
inc ebx
cmp bl,5
jne cul1
add esi,8
xor ebx,ebx
dec ebx
cul1:
loop oncul1
mov dwo [of_free],edi
add dwo [n_prim],8
add dwo [n_obj_prim],8
ret 
cu4s:
fxch st1
fst dwo [esi-4]
fxch st1
jmp cuinc
cu4z:
fxch st2
fst dwo [esi-4]
fxch st2
jmp cuinc
cube1:
dd 0,0,-1.0
dd 0,11.0,0, 0,0,-1.0
dd 10.0,0,0, 0,0,-1.0
dd 10.0,11.0,0, 0,0,-1.0 ;front

dd 10.0,0,12.0, 1.0,0,0 ;r
dd 10.0,11.0,12.0, 1.0,0,0 ;r


dd 0,0,12.0, 0,0,1.0 ;b
dd 0,11.0,12.0, 0,0,1.0 ;b


dd 0,0,0, -1.0,0,0 ;l
dd 0,11.0,0, -1.0,0,0 ;l
cube1_len equ $-cube1
;;;;;;;;;;;;;;;;;
onID_SETKA:
finit
p ebx
fild dwo [esp]
pop ebx
fld dwo [vz]
fld dwo [vy]
fld dwo [vx]
dec ecx
dec edx
p ecx
p edx
inc ecx
inc edx
sel2:
p ecx ;20
mov ecx,edx
sel1:
fst dwo [edi]
fxch st1
fst dwo [edi+4]
fxch st1
fxch st2
fst dwo [edi+8]
fxch st2
add edi,3*4
mov eax,000100h
call cre_normal
sub edi,8
;;tex
fld st0
fdiv st4
fsin
fstp dwo [edi]
fld st1
fdiv st4
fcos
fstp dwo [edi+4]
add edi,8
;;;y_nx
;3.0f*(cos((float)iy/3.0f)+cos((float)ix/3.0f));
fld st0 ;x
pf 13.67
;p dwo [dum2]
fld dwo [esp]
;fld1
;faddp st1,st0
;fst dwo [dum2]
fdiv to st1
fxch st1
fcos
fxch st1
fld st3 ;y
fdiv st1
fcos
faddp st2
fmulp st1 
fxch st2
fstp dwo [esp]
pop eax
;;;x_nx
fadd st0,st3
loop sel1
fxch st2
fadd st3
fxch st2
p eax
fstp dwo [esp]
pop eax
fld dwo [vx]
pop ecx
dec ecx
jnz sel2
finit
pop eax
pop ebx
p eax
p ebx
imul ebx
shl eax,1 ;по 2 prim в квадрате
add dwo [n_prim],eax
add dwo [n_obj_prim],eax
mov dwo [of_free],edi
lea edx,[eax+eax*2]
xor eax,eax
mov al,by [iflags]
test al,1
jz seex1
;;;регистрация индексного объекта
call reg_ind_obj
;;;;fill in indexes
;[esp]=z
;[esp+4]=x
xor ebx,ebx ;x
xor ecx,ecx ;z
mov ebp,dwo [esp+4]
inc ebp ;n_x_points
sej1:
mov eax,ecx
imul ebp
mov esi,eax  ;(iy+0)*(FLAG_SIZE+1);
inc ecx
mov eax,ecx
dec ecx
imul ebp   
mov edx,eax	;(iy+1)*(FLAG_SIZE+1)
lea eax,[ebx+edx]
stosw
lea eax,[ebx+esi]
stosw
lea eax,[ebx+esi+1]
stosw
lea eax,[ebx+edx]
stosw
lea eax,[ebx+esi+1]
stosw
lea eax,[ebx+edx+1]
stosw
inc ebx
cmp ebx,dwo [esp+4]
jb sej1
xor ebx,ebx
inc ecx
cmp ecx,dwo [esp]
jb sej1
seex1:
pop eax
pop eax
ret
;;;;;;;;;;;;;;;;;;;;
onID_PRLPD: ;{dz,dy,dx,z,y,x}
lodsd
mov ecx,eax
finit
ip1:
fld dwo [esi]
lodsd
loop ip1
;0,1,2=x,y,z ;3,4,5=dx,dy,dz
fadd to st3
fxch st1
fadd to st4
fxch st1
fxch st2
fadd to st5
fxch st2
p edi
mov ecx,2
ipl1a:
p ecx
mov ecx,4
ipl1:
fst dwo [edi]
add edi,SZ_VX_
loop ipl1
pop ecx
fxch st3
loop ipl1a
pop edi
scasd ;y
fxch st1
ipl2:
fst dwo [edi] ;y1
fst dwo [edi+3*SZ_VX_] ;y4
fst dwo [edi+4*SZ_VX_] ;y5
fst dwo [edi+7*SZ_VX_] ;y5
fxch st4
fst dwo [edi+SZ_VX_] ;y1
fst dwo [edi+2*SZ_VX_] ;y4
fst dwo [edi+5*SZ_VX_] ;y5
fst dwo [edi+6*SZ_VX_] ;y5
scasd ;z
p edi
fxch st2
mov ecx,2
ipl3:
fst dwo [edi] ;z1
fst dwo [edi+SZ_VX_] ;z2
fst dwo [edi+4*SZ_VX_] ;z5
fst dwo [edi+5*SZ_VX_] ;z6
fxch st5
add edi,2*SZ_VX_
loop ipl3
pop edi
scasd ;norm
mov ecx,8
ipl4:
mov eax,1
call cre_normal
add edi,3*4
loop ipl4
;sub edi,3*4
mov eax,6*2
sub edi,eax
add dwo [n_prim],eax
add dwo [n_obj_prim],eax
mov dwo [of_free],edi
lea edx,[eax+eax*2]
xor eax,eax
mov al,by [iflags]
test al,1
jz noind2
;;;регистрация индексного объекта
call reg_ind_obj
call nxparal
db 1,22h,30h,23h,72h,67h,56h
db 75h,47h,1,50h,45h,3,70h,47h
db 12h,61h,56h
nxparal:
pop esi
filn1:
xor eax,eax
lodsb
shl eax,16-4
shr ax,16-4
rol eax,16
stosd
dec ecx
loop filn1
noind2:
ret

;;;;;;;;;;;;;;
reg_ind_obj:
;edx=N_INDEXES;
;-->edi=off_in_IB ;ecx=n_vx
mov edi,[n_indobj]
imul edi,edi,SZ_OBJINDINFO_EL 
add edi,obj_ind_info
mov eax,dwo [of_free_ind]
stosd ;смещ. масива индексов объекта в obj_ind
mov edi,eax
shl edx,1
add eax,edx
mov dwo [of_free_ind],eax
mov ecx,edx
shr ecx,1
ret
;;;;;;;;;;;;;;
select_obj_draw:
;eax=#obj
mov ecx,eax
inc ecx
imul eax,eax,SZ_OBJINFO_EL
lea esi,[obj_info+eax]
lodsd
lodsd
lodsd
mov dwo [n_obj_prim],eax
mov dwo [n_draw],eax
lodsd
mov dwo [fst_vx_draw],eax
lodsb
mov by [iflags],al
test al,1
jz sobj1
xor edx,edx
mov esi,obj_info+4*4
sol1:
lodsb
test al,1
jz so2
inc edx
so2:
add esi,4*4
loop sol1
dec edx
mov esi,edx
imul esi,esi,SZ_OBJINDINFO_EL 
add esi,obj_ind_info
lodsd
sub eax,obj_ind
mov esi,eax ;;--->esi=of_in_IB
shr esi,1
sobj1:
ret
;;;;;;;;;;;;;;;;
gen_drawprim:
;генерирует по закону eax начало и размер массива
;рисуемых примит (треуг/линия)
ID_DRAWPRIM_SINCOS equ 0
jmp dwo [gen_drawprim_xref+eax*4]
gen_drawprim_xref dd onID_DRAWPRIM_SINCOS
onID_DRAWPRIM_SINCOS:
finit
fild dwo [fst_vx_draw]
mov edx,dwo [fst_vx_draw]
fld st0
fld dwo [cnt2]
fsin ;<<<<<<<<
fmulp st1
fabs
pf 2.0;<<<
fld dwo [esp]
fdivp st1
faddp st1
frndint
fistp dwo [esp]
pop eax
mov dwo [fst_vx_draw],eax
mov ebx,dwo [n_obj_prim]
mov ecx,ebx
add ebx,edx
sub ebx,eax
p ebx
fild dwo [esp]
pop ebx
fld st0
fld dwo [cnt2]
fcos ;<<<<<<<<
fmulp st1
fabs
pf 1.3;<<<
fld dwo [esp]
fdivp st1
fsubp st1
fabs
frndint
fistp dwo [esp]
pop dwo [n_draw]
cmp dwo [n_draw],ecx
jbe gd1
mov dwo [n_draw],ecx
gd1:
ret

;;;;;;;;;;;;;;;
reg_obj:  
;для только созданого массива примитивов ;(объекта) сохраняет в obj_info его 
;{offset, size,nprim,obj_index}
;--->eax=[n_obj_prim]
mov edi,dwo [n_obj]
imul edi,edi,SZ_OBJINFO_EL
add edi,obj_info
mov esi,edi
sub esi,4+4+4+1
cmp dwo [n_obj],0
jne ro0
xor ebx,ebx
jmp ro0a
ro0:
lodsd
mov ebx,SZ_VX*4
xor edx,edx
div ebx
test edx,edx
jz ro1
dec eax
ro1:
mov ebx,eax ;N_VX in obj
lodsd
lodsd
add ebx,eax ;new_Index
ro0a:
mov eax,dwo [of_thisobj]
mov ecx,eax
stosd
mov eax,dwo [of_free]
          mov  dwo [of_thisobj],eax
sub eax,ecx
stosd
mov eax,dwo [n_obj_prim]
p eax
stosd
mov eax,ebx
stosd ;obj_index
mov al,by [iflags]
stosb
test al,1
jz ro4
inc dwo [n_indobj]
ro4:
mov by [iflags],0
mov dwo [n_obj_prim],0
inc dwo [n_obj]
pop eax
ret
;;;;;;;;;;;;;;;;;;;;;;
cre_normal: ;создает по edi нормаль eax=0zyxh
;0,1,0ffh=-1
push ecx
mov ecx,3
cnl1:
or al,al
ror eax,8
js cn1
jz cn2
fld1
cnbk:
fstp dwo [edi]
scasd
loop cnl1
call cre_tex
pop ecx
ret
cn1:
fld1
fchs
jmp cnbk
cn2:
fldz
jmp cnbk
;;;;;;;;;;;;;;;;;;;;;
cre_tex:
mov eax,dwo [fvf]
and eax,D3DFVF_TEX1
jz notx1
mf 0
stosd
mf 0
stosd
ret
pushad
rdtsc
xor eax,edx
p eax
p edx
fild dwo [esp]
fcos
fabs
fstp dwo [esp]
pop eax
stosd
fild dwo [esp]
fsin
fabs
fstp dwo [esp]
pop eax
stosd
popad
notx1:
ret

;;;;;;;;;;;;;;;;;;
chg_cnt2:
finit
fld dwo [cnt2]
pf 0.001
fld dwo [esp]
pop eax
faddp st1,st0
fstp dwo [cnt2]
ret
;;;;;;;;;;;;;
chg_eye:
fld dwo [cnt2]
fld st0
fsin
p dwo [eye]
fld dwo [esp]
fchs
pop eax
fmul to st1
fxch
fst dwo [vx_lat_eye]
fstp dwo [vx_lat_eye+4]
fchs
fxch
fcos
fmulp st1,st0
fstp dwo [vx_lat_eye+2*4]
ret
;;;;;;;;;;;;;
put_fst_vx: ;ecx-->esi=vx1_start
finit
cld
mov edx,ecx
mov ebp,edx
mov ecx,3
p edi
pop esi
on1l:
mov edx,ebp
xor eax,eax
and edx,0ffh
test dl,dl
jns psv1
neg dl
inc eax
psv1:
p edx
fild dwo [esp] ;x0
pop edx
dec eax
jnz psv2
fchs
psv2:
fstp dwo [edi]
scasd
shr ebp,8
loop on1l
ret
;;;;;;;;;;;;
fadd_1:
fld dwo [esi]
fadd st0,st1
fstp dwo [edi]
cmpsd
ret
;;;;;
fsub_1:
fld dwo [esi]
fsub st0,st1
fstp dwo [edi]
cmpsd
ret




