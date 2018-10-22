CPU P4 
BITS 32
ideal
%include 'nasm_dxm.inc'
%include "W_CON.INC"
%include 'nasm_macros.inc'
%include 'nasm_dxc.inc'
%include 'nasm_dxs.inc'
%include 'nasm_externs.inc'
%include 'nasm_procs.inc'
global window_proc
;;;;;;;;;; CONSTS
debug equ 1
%if debug=0
wid equ 320
hei equ 240
%endif
%if debug=1
wid equ 1024
hei equ 768
%endif
IB_SIZE equ 2*10000h
CUR_FVF equ D3DFVF_XYZ+D3DFVF_NORMAL+D3DFVF_TEX1  
SZ_VX equ 3+3+2
SZ_VX_ equ SZ_VX*4
N_TREUG equ 100
TIMEOUT equ 3000
N_MAX_OBJ equ 100h
;;;;;;;;;;;;;;;;
[segment code public use32 class='CODE']
..start: ;Точка входа
;int 3
call app_init
jc exitf
p0
p TIMEOUT
inc eax
p eax
p dwo [hwnd]
cal  SetTimer  
test eax,eax
jz exitf
mov dwo [htim],eax
go1:
mov ebp,msg_s
p PM_NOREMOVE
p0
p eax
p eax
p ebp
cal PeekMessage 
test eax,eax
jz go1_render
dec eax
p eax
p eax
p eax
p ebp
cal GetMessage 
test eax,eax
jz exitt
p ebp
cal TranslateMessage 
p ebp
cal DispatchMessage 
jmp go1
go1_render:
cmp by [i_act],0
jz go1
call render_screen
jmp go1
exitt:
call destroy
exitf:
p0
cal ExitProcess
aiexf:
stc 
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;DRAW
draw:
p0
pf 1.0
p 11223344h; dwo [cnt1]
p D3DCLEAR_TARGET+D3DCLEAR_ZBUFFER
p eax
p eax
ca Clear,hi2 
ca BeginScene,hi2
;;;VVVVVVVVVVVVVVVVVVVVVVVVVV
p CUR_FVF
ca SetVertexShader,hi2
p SZ_VX*4
p dwo [hi3]
p eax
ca SetStreamSource,hi2
;jmp yind
mov eax, ID_CUBE 
;mov eax,ID_TREUG 
call select_obj_draw
;mov eax,ID_DRAWPRIM_SINCOS
;call gen_drawprim
p dwo [n_draw]
p dwo [fst_vx_draw]
p  D3DPT_TRIANGLESTRIP
ca DrawPrimitive,hi2
;jmp noind
yind:
int3
mov eax,ID_SETKA 
mov eax,ID_PRLPD
mov ebp,ID_SETKA 
ebp2:
mov eax,ebp
call select_obj_draw
p dwo [fst_vx_draw]
p dwo [hi4]
ca SetIndices,hi2
mov ecx,dwo [n_draw]
p ecx
p esi  ;<--from select_obj (StartIndex)
lea ecx,[ecx+ecx*2]
p ecx
p 0; esi; 0 ;minIndex
p D3DPT_TRIANGLELIST
ca DrawIndexedPrimitive,hi2
inc ebp
cmp ebp,ID_PRLPD+1
jne ebp2
noind:
mov dwo [n_obj_prim],0
;;;^^^^^^^^^^^^^^^^^^^^^
ca EndScene,hi2
p0
p eax
p eax
p eax
ca Present,hi2
mov by [iflags],0
ret
;;;;;;;;;;;;;;;;;;;

app_init:
call window_init
jz aiexf
p SW_SHOWNORMAL	
p dword [hwnd]
cal ShowWindow
p dword [hwnd]
cal UpdateWindow
p 78h ;1fh
cal  Direct3DCreate8
mov [hi1],eax
p D3DDISPLAYMODE_
p D3DADAPTER_DEFAULT 
ca GetAdapterDisplayMode ,hi1
p dwo [D3DDISPLAYMODE_+Format]
pop dwo [D3DPRESENT_PARAMETERS_+BackBufferFormat ]
mov ebp,D3DFMT_D16 
cd1:
mov dwo [D3DPRESENT_PARAMETERS_+EnableAutoDepthStencil],1
mov dwo [D3DPRESENT_PARAMETERS_+AutoDepthStencilFormat ],ebp 
p hi2
p D3DPRESENT_PARAMETERS_
p D3DCREATE_SOFTWARE_VERTEXPROCESSING
p dwo [hwnd]
p D3DDEVTYPE_HAL
p D3DADAPTER_DEFAULT
int3
ca CreateDevice, hi1 
inc ebp
test eax,eax
jnz cd1
int3
call matrix_init
call vb_init
call mtr_init
call lit_init
call set_device_params
aiext:
clc
ret
;;;;;;;;;;;;;;;
window_init:
mov ebp,esp
p0
p nm1
p eax
p BLACK_BRUSH
cal GetStockObject
p eax
p0
cal GetModuleHandleA
mov [hinst],eax
p IDC_ARROW
p eax
cal LoadCursor
p eax
p IDC_ICON
p dwo [hinst]
cal LoadIcon
p eax
p dwo [hinst]
p0
p eax
p window_proc
p CS_HREDRAW + CS_VREDRAW
p 12*4
p esp
cal RegisterClassEx
p0
p dwo [hinst]
p eax
p eax
p hei
p wid
p eax
;p 80000000h
p eax
p WS_OVERLAPPEDWINDOW
p nm1
p dwo [esp]
p WS_EX_TOPMOST
cal CreateWindowEx
mov [hwnd],eax
mov esp,ebp
test eax,eax
ret
;************************
window_proc:
%arg hwnd_:dword,ms:dword,wp:dword,lp:dword
;db 0cch
p ebp
mov ebp,esp
mov eax,[ebp+0ch]
mov eax,dwo [ms]
cmp eax,WM_TIMER
je tim
cmp eax,WM_CREATE
jnz wp1
jmp retze
tim:
call onTimer
jmp retze
wp1:
cmp eax,WM_ACTIVATE
jz onWM_ACTIVATE
cmp eax,WM_DESTROY
je onWM_DESTROY
;cmp eax,WM_SETCURSOR
;jz onWM_SETCURSOR
cmp eax,WM_KEYDOWN 
jne jf
mov ecx,dwo [wp]
cmp ecx,VK_ESCAPE 
je onWM_DESTROY
jf:
p dwo [lp]
p dwo  [wp]
p dwo [ms]
p  dwo [hwnd_]
cal DefWindowProc
jmp retze1
ret
onWM_ACTIVATE:
mov eax,[wp]
mov [i_act],al
retze:
xor eax,eax
retze1:
pop ebp
ret
onWM_DESTROY:
p0
cal PostQuitMessage 
jmp retze
onWM_SETCURSOR:
p0
cal SetCursor 
jmp retze
;********************
render_screen:
ca TestCooperativeLevel, hi2
test eax,eax
jz rscont
cmp eax, D3DERR_DEVICELOST 
jne rs1
ret
rs1:
p D3DPRESENT_PARAMETERS_
ca Reset,hi2
call set_device_params
rscont:
call transform_3d ;<<<<<<
call transform_lit
call mtr_init
call draw
ret
;;;;;;;;;
destroy:
ca Release,hi5
ca Release,hi4
ca Release,hi3
ca Release,hi2
ca Release,hi1
p dwo [htim]
p dwo [hwnd]
cal  KillTimer
ret
;;;;;;;;;;;;;;;;;;;;;;;;;
tex_init:
jmp no11
mov ebx,0
pf 0.4
pop ebp
call set_vx_tex
no11:
p dwo [hi5]
p0
ca SetTexture,hi2
;p D3DTSS_TCI_CAMERASPACENORMAL 
;p D3DTSS_TCI_CAMERASPACEPOSITION 
p D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR 
p D3DTSS_TEXCOORDINDEX 
p D3DTA_TEXTURE
p D3DTSS_COLORARG1
p D3DTOP_SELECTARG1
p D3DTSS_COLOROP
p D3DTOP_MODULATE
p D3DTSS_COLOROP
mov ecx,4
mov edi,SetTextureStageState 
xor ebx,ebx
inc ebx
xor edx,edx
call call_opt1
ret
;;;;;;;;;;
lit_init:
int3
mov edi,lit
p edi
mov ecx,4+3*4+2*3+7
xor eax,eax
rep stosd
pop edi
mov dwo [edi],D3DLIGHT_DIRECTIONAL
pf 1.0
pop eax
cld
scasd
times  3 stosd
add edi,4+2*4*4+3*4
p edi
p eax
xor eax,eax
stosd
stosd
pop eax
stosd
int3
pop eax
p eax
p eax
cal D3DXVec3Normalize 
pf 1000.0
pop dwo [edi]
p lit
p0
ca SetLight,hi2 
inc eax
p eax
dec eax
p eax
ca LightEnable,hi2 
ret
;;;;;;;;;;;;;;;;;;;;;
mtr_init: ;dase{rgba},p
cld
finit
p eax
fld dwo [cnt2]
pop eax
int3
mov edi,mtr
xor eax,eax
mov ecx,4
mtrl1:
stosd
fsin
fst dwo [edi] ;g
fcos
fst dwo [edi+4] ;blue
fld1
fstp dwo [edi+8] ;alpha
add edi,8+4
loop mtrl1
;mov ecx,2*4
;rep stosd
fstp dwo [edi] 
p mtr
ca SetMaterial,hi2
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
vb_init:
;int3
pf 0.9
call get_avg1
xor ebp,ebp
mov ecx,N_TREUG
mov dwo [of_thisobj],obj
;jmp paral0
p eax
;;;#1
vbl1:
p eax
p ecx
sub eax,ebp
mov ebx,eax
shl eax,16
add ebx,eax
mov bh,bl
shr bh,1
mov dl,bh
shr edx,1
add bh,dl
mov eax,ID_TREUG
mov ecx,ebp ;zyxh
mov edx,1234567h ;color
p ebp
;int3
call cre_obj
finit
pop eax
mov ebp,eax
mov ecx,3
vbl2:
mov eax,ebp
and eax,0ffh
p eax
fild dwo [esp]
fld st0
fld st0
cmp cl,1
je vbl2a
fsin
jmp vbl2b
vbl2a:
fcos
vbl2b:
fmulp st1,st0
faddp st1,st0 ;sin(x)*x+x
frndint
fistp dwo [esp]
pop eax
and eax,0ffh
add ebp,eax
ror ebp,8
loop vbl2
rol ebp,8
add ebp,010101h
pop ecx
pop eax
loop vbl1
int3
call reg_obj
;;;;;#2
pop ebx
p ebx
;p dwo [of_free]
;pop dwo [of_thisobj]
xor ecx,ecx
;dec ecx
lo1:
mov eax,ID_CUBE
call cre_obj
;sub ebx,3
add ecx,ebx
test cl,cl
jns lo1
call reg_obj
;;;;;;;#3
pop ebx ;сторона квадрата сетки
paral0:
mov ebx,20
pf -100.0
pop dwo [vx]
mov dwo [vy],0
mov dwo [vz],0
mov edx,30 ;число квадратов по x
mov ecx,40 ;z
mov eax,ID_SETKA
int3
or by [iflags],1
call cre_obj
call reg_obj
;;;;;;;;;;;;;;;;;;;;#4
paral:
int3
mov edi,pr
mov eax,6
stosd
mf 100.0
stosd
stosd
stosd
mf 5.0
stosd
stosd
stosd
or by [iflags],1
mov eax,ID_PRLPD
call cre_obj
call reg_obj
;;;;;;;;;;;;;;;;;;;;;;;;
p  hi5
p  nm_tex1
p dwo [hi2]
cal D3DXCreateTextureFromFileA 
call tex_init
;;;;;;;;;;;;
noc1:
p dwo [n_prim]
pop dwo [n_draw]
mov dwo [fst_vx_draw],0
mov ecx,dwo [of_free]
sub ecx,obj
p ecx
p hi3
p D3DPOOL_MANAGED 
p CUR_FVF; 
p0
p ecx
;int3
ca CreateVertexBuffer,hi2
p eax
p hvb
p eax ;SZ_VX
p eax
ca IDirect3DVertexBuffer8_Lock, hi3
pop ecx
shr ecx,2
mov esi,obj
mov edi,[hvb]
rep movsd
ca IDirect3DVertexBuffer8_Unlock,hi3
p hi4
p D3DPOOL_MANAGED
p D3DFMT_INDEX16
p0
p IB_SIZE
ca CreateIndexBuffer,hi2  
p eax
p hib
p eax
p eax
ca IDirect3DIndexBuffer8_Lock,hi4
mov esi,obj_ind
mov edi,[hib]
mov ecx,dwo [of_free_ind]
sub ecx,esi
shr ecx,2
rep movsd
ca IDirect3DIndexBuffer8_Unlock,hi4
ret
;;;;;;;;;;;;;;;;;;;
matrix_init:
p0 ;radians
p mxw
cal D3DXMatrixRotationY
p vx_lat_up
p vx_lat_tag
p vx_lat_eye
p dwo [eye]
pop dwo [vx_lat_eye+2*4]
p mxv
cal D3DXMatrixLookAtLH 
;int3
pf 10000.0 ;far_view_plane
pf 0.0 ;near_view_plane (1.0)
finit
p wid
fild dwo [esp]
pop eax
p hei
fild dwo [esp]
fdivp st1,st0
fstp dwo [esp]
pop dwo [aspect]
p dwo [aspect] ;aspect_ratio
p dwo [fov] ;3.14/4=fov
p mxp
cal D3DXMatrixPerspectiveFovLH
finit
pf 2.0
fld dwo [fov]
fld dwo [esp]
fdivp st1
fsincos
fdivp st1
fld dwo [eye]
fmulp st1
frndint
fist dwo [miny]
fchs
frndint
fist dwo [maxy]
fld dwo [aspect]
fmulp st1
frndint
fist dwo [maxx]
fchs
frndint
fistp dwo [minx]
pop eax
ret
;;;;;;;;;;;;;;;;;;;;;;;;;
set_device_params:
p mxw
p D3DTS_WORLD
ca SetTransform,hi2
p mxv
p D3DTS_VIEW
ca SetTransform,hi2
p mxp
p D3DTS_PROJECTION
ca SetTransform,hi2
p D3DCULL_NONE
p D3DRS_CULLMODE
p 1
p D3DRS_LIGHTING
p 00202020h ;цвет воздуха
p D3DRS_AMBIENT
p D3DZB_TRUE
p D3DRS_ZENABLE
mov edi,SetRenderState
mov ecx,4
xor ebx,ebx
call call_opt1
inc eax
p eax
dec eax
p eax
ca LightEnable,hi2 
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;
transform_3d:
;#1
p vx_lat_up
p vx_lat_tag
p vx_lat_eye
p mxv
cal D3DXMatrixLookAtLH 
p eax
p D3DTS_VIEW
ca SetTransform,hi2
jmp noy
sub esp,100h
int3
mov eax,esp
p dwo [cnt2]
fld dwo [esp]
fsin
pf 0.01
fld st0
frndint
p eax
fistp dwo [esp]
pop ebx
dec ebx
jnz jz0
fchs
jz0:
fadd dwo [esp]
;fabs
pop ebx
fstp dwo [cnt1]
p eax
cal D3DXMatrixRotationX
p eax
p mxw
p dwo [esp]
cal D3DXMatrixMultiply
add esp,100h
p mxw
p D3DTS_WORLD
ca SetTransform,hi2
noy:
call chg_cnt2
call chg_eye
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;
transform_lit:
mov edi,lit+(4+3*4+3)*4-10h
finit
fld dwo [cnt2]
fld st0
p dwo [eye]
fld dwo [esp]
fabs
pop eax
fxch st2
fsin
fmul st2
fstp dwo [edi]
fcos
fmulp st1,st0
fst dwo [edi+4]
fchs
fstp dwo [edi+8]
;jmp jj2
p edi
p edi
cal D3DXVec3Normalize 
jj2:
p lit
p0
ca SetLight,hi2
ret
;;;;;;;;;;
call_opt1: ;ecx=число вызовов;edi=of_func
;ebx=1=>авто кладет edx
pop ebp
mov dwo [dum1],edx
mov esi,ecx
co1:
test ebx,ebx
jz co2
p dwo [dum1]
co2:
ca edi,hi2
dec esi
jnz co1
xor eax,eax
jmp ebp
;;;;;;;;;;;;
onTimer:
ret
;;;;;;;;;;;;
set_vx_tex: ;ebx=#_obj ;ebp=1.0 - образец u-v
int3
p ebp
fld dwo [esp]
fsub dwo [cnt2]
fstp dwo [esp]
pop ebp
imul ebx,ebx, SZ_OBJINFO_EL
lea esi,[obj_info+ebx]
lodsd
mov edi,eax
lodsd
mov ecx,eax
svt0:
xor edx,edx
xor ebx,ebx
svtbk1:
jcxz svt2
xchg ebx,edx
add ebx,edx
add edi,6*4
mov eax,edx
;mf 0.5
stosd
mov eax,ebx
;mf 0.6
stosd
sub ecx,8*4
cmp ebx,edx
jne svtbk1
test ebx,ebx
jnz svt0
svt1:
mov edx,ebp
jmp svtbk1
svt2:
ret








;;;;;;;;;;;;;;;;;;;;;;;;;
[segment data public use32 class='CODE']
;;;;;;;;;
iflags db 0 ;0(=1):use indexed_vertises
dum2 dd 10.0 
;;;;;;;
i_act db 0
nm_tex1 db 'tex.jpg',0
nm1 db 'xxx',0
cnt1 dd 0 ;Clearr
cnt2 dd 0 ;LookAt LH
eye dd -500.0
fov dd 0.785
aspect dd 1.333333
n_prim dd 0 ;общее число треуг
n_obj_prim: dd 0 ;--//-- для данного объекта
n_obj dd 0 ;общ число объектов
n_indobj dd 0
of_free dd obj
of_free_ind dd obj_ind
;;;;;;;;;;;STRUC
D3DDISPLAYMODE_: istruc D3DDISPLAYMODE
at Width,dd 0
at Height,dd 0
at RefreshRate,dd 0
at Format,dd 0
iend
;;
D3DPRESENT_PARAMETERS_: istruc  D3DPRESENT_PARAMETERS
at  BackBufferWidth,dd wid
at   BackBufferHeight ,dd hei
at   BackBufferFormat ,dd 0
at   BackBufferCount ,dd 0
at    MultiSampleType,dd 0
at    SwapEffect,dd  D3DSWAPEFFECT_DISCARD
at    hDeviceWindow,dd 0
at    Windowed ,dd 1
at    EnableAutoDepthStencil ,dd 0
at    AutoDepthStencilFormat ,dd 0
at    Flags3d ,dd 0
at    FullScreen_RefreshRateInHz,dd 0
at    FullScreen_PresentationInterval,dd 0
iend
;;;;;;;;;;;;DATA
vx_lat_up dd 0,1.0,0
vx_lat_tag dd 0,0,0
vx_lat_eye dd 0,0,-500.0


;;;;;;;;;;;;;;;;;;;
[SECTION .bss use32]
;;;;;;;;;вертекс координаты для 1-го вершины объекта
vx: resd 1
vy: resd 1
vz: resd 1
;;;;;;;;
dum1: resd 1
fvf: resd 1
iuse_objtime: resb 1
htim: resd 1
hinst: resd 1
hwnd: resd 1
;;;;;;;;;;;;;;
hi1: resd 1 ;3d8
hi2: resd 1 ;Device8
hi3: resd 1 ;VertexBuffer
hi4: resd 1 ;IB
hi5: resd 1 ;Tex8
;;;;;;;;;
msg_s: resd 7 ;hwnd,ms,wp,lp,t,pt
hvb: resd 1
hib: resd 1
mxw: resd 4*4
mxv: resd 4*4
mxp: resd 4*4
maxx: resd 1
minx: resd 1
maxy: resd 1
miny: resd 1
obj: resd 50000h ;
SZ_OBJINFO_EL equ 4*4+1
obj_info: resb N_MAX_OBJ*SZ_OBJINFO_EL ;{of_prims_in_obj,sz_obj (bytes),n_obj_prims,obj_index,flags(db)}
;obj_index - номер 1-й вершины объекта в вертекс.буфере
of_thisobj: resd 1
n_draw: resd 1
fst_vx_draw: resd 1
;;MATERIAL
 ;;;;mtr: difuse,ambient,spec,emessive {r,g,b,a}, power {float}
mtr: resd 4*4+4
;;LIGHTS
;;lit: type(dd),dif,spec,amb{r,g,b,a},
;;pos,dir{x,y,z},range[R],falloff[=1.0],atten0,atten1,atten2[R>0],theta[0,phi],phi[0,Pi]
lit: resd 4+3*4+2*3+7
;;;;;TIMER
;{
;obj_time: resb N_MAX_OBJ*
;{off_in_IB}; число indexes=n_obj_prims*3
SZ_OBJINDINFO_EL equ 4
obj_ind_info: resb N_MAX_OBJ*SZ_OBJINDINFO_EL
obj_ind: resw 20000
pr: resd 1000 ; для передачи параметров в cre_obj;{N_param,p1,..,pN}
