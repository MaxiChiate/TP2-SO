GLOBAL play_sound
GLOBAL nosound


;;pone un sonido en el speaker
play_sound:
;;cargo la frecuencia
        push rbp
        mov rbp, rsp
        push rcx
        push rdx
        mov al, 0xB6            ; Cargar 0xB6 en AL
        out 0x43, al            ; Escribir AL en el port 0x43 (PIT command) indica que se van a enviar 2 datos al port 0x42
        mov eax, 1193180         ; Cargar 1193180 en AX
        mov rdx, 0
        div di                  ; Dividir EAX por la frecuencia
        mov dl, al              ; Guardo el bit bajo de rax
        mov al, ah              ; Guardo el bit alto de rax
        out 0x42, al            ; Escribir AL en el port 0x42 (PIT data)
        mov al, dl              ; Mover el bit guardado temporalmente en dl
        out 0x42, al            ; Escribir AL en el port 0x42 (PIT data)
    ;;prendo el speaker
        in      al, 0x61         ;;lee el valor del puerto
        mov     cl, al
        or      cl, 3
        cmp     al, cl
        jz      exit
        mov     al, cl
        out     0x61, al         ;;sobreescribo el puerto con la nueva configuracion
    exit:
        pop rdx
        pop rcx
        mov rsp, rbp
        pop rbp
        ret

; Function to silence the sound
nosound:
    in      al, 0x61         ; guardo la info del puerto
    and     al, 0xFC         ; hago un and para apagarlo
    out     0x61, al         ; sobreescribo
    ret
