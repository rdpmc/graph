%define qwo qword
%define dwo dword
%define by byte
%define wo word

%macro API 2
	import %1 %2
	extern %1
%endmacro



%macro mi 2
db 6ah
db %2
pop %1
%endmacro


%macro pi 1
db 6ah
db %1
%endmacro

%macro stp 1
p eax
fstp dword [esp]
pop eax
%endmacro

%macro int3 0
db 0cch
%endmacro

%macro p 1 
          push %1 
%endmacro

%macro p0 0
	xor eax,eax
          push eax
%endmacro
 
%macro pf 1
db 68h
dd %1
%endmacro

%macro mf 1
db 0b8h
dd %1
%endmacro


%macro cf 1
db 03dh
dd %1
%endmacro


%macro cal2 1
call dword [%1]
%endmacro

%macro ca 2
mov eax,dword [%2]
mov eax,dword [eax]
p dwo [%2]
call dword [eax+%1*4]
%endmacro



;===================== CALL


%macro CONST 2+  ; macro to insert constant into code
 jmp near %%skip ; jump to macro-local label
 %1 %2  ; mark label and define data
 %%skip:   ; macro-local label
%endm

%macro STDPUSH 1  ; macro to push argument list
 %ifnum %1  ; argument is a number
  ; check number to fit in signed byte
  %if %1 >= -128 && %1 <= 127
   push byte %1
  %else ; cannot be pushed as signed byte
   push dword %1
  %endif
 %else  ; argument is not a number
  %ifstr %1 ; argument is a string
   CONST %%str, db %1, 0 ; define macro-local label
   push dword %%str
  %else ; neither string nor number (label or register)
   push dword %1
  %endif
 %endif
%endmacro


%imacro cal 1-*  
%assign i	%0
	%if %0 > 1
		%rep %0 - 1
			%rotate -1  
			%assign i	i-1		                               
			%rotate -1
			%ifidni %1,"ADDR_"
			%rotate 1
			lea	eax,	[%1]
			push	eax
			%rotate -1
			%assign i	i-1
			%else		
			%rotate 1
			STDPUSH {%1}     
			%endif
			%if i <=1
			%exitrep
			%endif
		%endrep		
		%rotate -1
	%endif
                extern %1     ; declare API function as external
                CALL [%1]          ; call API function via IAT

%endmacro




