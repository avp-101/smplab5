 .code
 public to_gray_asm
 to_gray_asm proc uses RCX R8 RDX,
 image:ptr, im_height:qword, im_width:qword
 mov RAX, RDX
 mul R8
 mov RSI, RCX
 mov RDI, RAX ; dimensiune plan (offset G)
 mov R10, RSI
 add R10, RDI ; limita pointer R
 mov R11, 0FEFEFEFEFEFEFEFEh ; constanta pentru stergerea LSB
 mov R9, 00101010101010101h ; constanta pentru ajustarea rezultatului
 bucla:
 mov RAX, qword ptr [RSI] ; R
 mov RBX, qword ptr [RSI] ; G
 mov RCX, qword ptr [RSI] ; B
 and RAX, R11 ; sterge bitul cel mai putin semnificativ
 and RBX, R11 ; din fiecare octet pentru a permite
 and RCX, R11 ; adunarea vectorizata (octet cu octet)
 add RAX, RBX ; R/2 + G/2 (deplasat cu 1 bit)
 rcr RAX, 1 ; R/2 + G/2
 add RBX, RCX ; G/2 + B/2 (deplasat cu 1 bit)
 rcr RBX, 1 ; G/2 + B/2
 and RAX, R11 ; stergem LSB pentru o noua adunare vectorizata
 and RBX, R11 ; (R/2 + G/2) / 2, (G/2 + B/2) / 2
 add RAX, RBX ; (R/2 + G/2) / 2 + (G/2 + B/2) / 2 (deplasat cu 1 bit)
 rcr RAX, 1 ; (R/2 + G/2) / 2 + (G/2 + B/2) / 2
 add RAX, R9 ; rezultat final: (R/2 + G/2) / 2 + (G/2 + B/2) / 2 + 1
 mov qword ptr [RSI], RAX
 mov qword ptr [RSI+RDI], RAX
 mov qword ptr [RSI+RDI*2], RAX
 add RSI, 8
 cmp RSI, R10
 jb bucla
 ret
 to_gray_asm endp
 end