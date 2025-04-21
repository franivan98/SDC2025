set logging file gdb_output.txt
set logging enabled on

echo ===========================\n
echo INICIO DE DEPURACIÓN\n
echo ===========================\n

break float_to_int
run

echo \n>>> Corriendo hasta entrar a float_to_int\n

echo \n>>> [1] Estado de registros al entrar a float_to_int:\n
info registers

echo \n>>> [2] Stack inicial (16 palabras desde ESP):\n
x/16wx $esp

# Paso a paso: push ebp
ni
# Paso a paso: mov ebp, esp
ni

echo \n>>> [3] Float original (en [ebp+8]):\n
x/x $ebp+8
p *(float*)($ebp+8)

# sub esp, 4
ni

# fld dword [ebp + 8]
ni

# fistp dword [esp]
ni
echo \n>>> [4] Valor entero guardado en stack:\n
x/wx $esp

# mov eax, [esp]
ni
echo \n>>> [5] Valor en eax (entero convertido):\n
print $eax

# add eax, 1
ni
echo \n>>> [6] Valor en eax después de sumar 1:\n
print $eax

# mov esp, ebp
ni
# pop ebp
ni

echo \n>>> [7] Stack restaurado (16 palabras desde ESP):\n
x/16wx $esp

# ret
ni

echo \n=============================\n
echo FIN DE LA DEPURACIÓN\n
echo =============================\n

set logging enabled off
quit
