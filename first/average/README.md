Первый вариант. Вывод:
```
2025-08-10T18:06:54+05:00
Running /mnt/LinuxFS/CLionProjects/effective-mobile-test/cmake-build-release/first_average
Run on (20 X 5100 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x10)
  L1 Instruction 32 KiB (x10)
  L2 Unified 2048 KiB (x10)
  L3 Unified 24576 KiB (x1)
Load Average: 0.82, 1.02, 0.84
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
by_value_first      627387 ns       627284 ns         1183
by_ref_first      19399341 ns     19396271 ns           35
by_value_second   13361804 ns     13360220 ns           51
by_ref_second     20110094 ns     20108594 ns           34
by_value_third    15173569 ns     15171541 ns           45
by_ref_third      12047253 ns     12046139 ns           64

Process finished with exit code 0

```
Ниже находятся фрагменты кода на ассемблере, полученные из исполняемого файла:
```asm
/*by_value_first*/
//0x555555557000:
cmp    BYTE PTR [rax+0x4],0x0   //Проверка на наличие значения в std::optional<int>
je     0x555555556363 <by_value(benchmark::State&)-1853> //Прыжок на исключение bad_optional_access
add    edx,DWORD PTR [rax]      //Прибавить к edx (avg) значение, находящееся по адресу rax.
add    rax,0x8                  //Следующий элемент массива
cmp    r14,rax                  //Проверка условия выхода из цикла
jne    0x555555557000 <by_value(benchmark::State&)+1376> //Прыжок на следующую итерацию
```
```asm
/*by_ref_first*/
//0x555555557660:
mov    rcx,QWORD PTR [rax]      //Сохранить в rcx адрес переменной, находящееся по адресу rax (Разыменовывание).
add    rax,0x8                  //Следующий элемент массива
add    edx,DWORD PTR [rcx]      //Прибавить к edx (avg) значение, находящееся по адресу rcx.
cmp    rbp,rax                  //Проверка условия выхода из цикла
jne    0x555555557660 <by_ref(benchmark::State&)+1472>//Прыжок на следующую итерацию
```
В первом случае есть проверка на значение, а во втором разыменование.
Проверка на пустоту в value.value() занимает меньше процессорного времени нежели mov и не касается предмета темы. 
Основное влияние на время выполнения алгоритма by_ref оказывает эта самая операция разыменования (инструкция mov),
так как она записывает значение в регистр процессора, что, очевидно, имеет оверхед.

Второй вариант.
В сравнении с предыдущим вариантом, время для by_value значительно выросло. Вот что было сгенерировано:
```asm
/*by_value_second*/
//0x555555558de0:
cmp    BYTE PTR [rax+0x50],0x0  
je     0x55555555664c <by_value_second(benchmark::State&)-7268>
add    edx,DWORD PTR [rax+0x48] 
add    rax,0x58                 
cmp    rbx,rax                  
jne    0x555555558de0 <by_value_second(benchmark::State&)+2864>
```

```asm
/*by_ref_second*/
//0x555555558210:
mov    rcx,QWORD PTR [rax]      
add    rax,0x8                  
add    edx,DWORD PTR [rcx+0x48] 
cmp    rbp,rax                  
jne    0x555555558210 <by_ref_second(benchmark::State&)+1680>
```
В сравнении с предыдущим вариантом, здесь применяются бОльшие значения смещения, что негативно влияет на скорость 
доступа к кэшу процессора.

Третий вариант.
```asm
//by_value_third
//0x555555559b40:
cmp    BYTE PTR [rax+0x50],0x0  
je     0x555555559b49 <by_value_third(benchmark::State&)+3705>
add    edx,DWORD PTR [rax+0x48] 
//0x555555559b49:
add    rax,0x58                 
cmp    rbx,rax                  
jne    0x555555559b40 <by_value_third(benchmark::State&)+3696>
```
```asm
//by_ref_third
//0x555555558520:
mov    rdx,QWORD PTR [rax]      
test   rdx,rdx                  
je     0x55555555852b <by_ref_third(benchmark::State&)+1899>
add    ecx,DWORD PTR [rdx+0x48] 
//0x55555555852b:
add    rax,0x8                  
cmp    rax,rbp                  
jne    0x555555558520 <by_ref_third(benchmark::State&)+1888>
```
by_ref быстрее по той же причине, по которой во втором варианте в by_value значительно медленнее первого варианта. 
То есть здесь в by_ref половину времени выполняются инструкции без смещения, а половину - с ним, тогда как by_value 
всегда со смещением.

