.code
public reverse
reverse proc uses RSI, arraychar:ptr ; pentru Windows
 mov arraychar, RCX ; doar pentru Windows
 mov RSI, arraychar
 mov RAX, 0
 push RAX 
 jmp loop1
 ret

 
 cloop1: 
 mov AL, [RSI]
 push RAX
 inc RSI
 loop1:  
 cmp byte ptr [RSI],0
 jne cloop1 
 mov RSI, arraychar
 loop2:
 pop RAX
 mov [RSI], AL
 inc RSI
 cmp RAX, 0
 jne loop2
 ret
reverse endp
end




