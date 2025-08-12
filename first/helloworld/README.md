Символы исполняемого файла:
```shell
objdump -t cmake-build-debug/first_helloworld 
```

Примерный вывод команды:

```
objdump -t cmake-build-debug/first_helloworld

cmake-build-debug/first_helloworld:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000              Scrt1.o
000000000000038c l     O .note.ABI-tag  0000000000000020              __abi_tag
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
0000000000001070 l     F .text  0000000000000000              deregister_tm_clones
00000000000010a0 l     F .text  0000000000000000              register_tm_clones
00000000000010e0 l     F .text  0000000000000000              __do_global_dtors_aux
0000000000004010 l     O .bss   0000000000000001              completed.0
0000000000003df8 l     O .fini_array    0000000000000000              __do_global_dtors_aux_fini_array_entry
0000000000001120 l     F .text  0000000000000000              frame_dummy
0000000000003df0 l     O .init_array    0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*  0000000000000000              main.cpp
0000000000000000 l    df *ABS*  0000000000000000              hello.cpp
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
00000000000020e8 l     O .eh_frame      0000000000000000              __FRAME_END__
0000000000000000 l    df *ABS*  0000000000000000              
0000000000003e00 l     O .dynamic       0000000000000000              _DYNAMIC
0000000000002004 l       .eh_frame_hdr  0000000000000000              __GNU_EH_FRAME_HDR
0000000000003fc0 l     O .got   0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000000000       F *UND*  0000000000000000              __libc_start_main@GLIBC_2.34
0000000000000000  w      *UND*  0000000000000000              _ITM_deregisterTMCloneTable
0000000000004000  w      .data  0000000000000000              data_start
0000000000004010 g       .data  0000000000000000              _edata
0000000000001150 g     F .fini  0000000000000000              .hidden _fini
0000000000004000 g       .data  0000000000000000              __data_start
0000000000000000  w      *UND*  0000000000000000              __gmon_start__
0000000000004008 g     O .data  0000000000000000              .hidden __dso_handle
0000000000002000 g     O .rodata        0000000000000004              _IO_stdin_used
000000000000113f g     F .text  000000000000000f              _Z5helloPKc
0000000000004018 g       .bss   0000000000000000              _end
0000000000001040 g     F .text  0000000000000026              _start
0000000000004010 g       .bss   0000000000000000              __bss_start
0000000000001129 g     F .text  0000000000000016              main
0000000000004010 g     O .data  0000000000000000              .hidden __TMC_END__
0000000000000000  w      *UND*  0000000000000000              _ITM_registerTMCloneTable
0000000000000000  w    F *UND*  0000000000000000              __cxa_finalize@GLIBC_2.2.5
0000000000001000 g     F .init  0000000000000000              .hidden _init
```

Нас интересуют символы под названием `main` и `_Z5helloPKc`

Символы объектного файла hello.o
```shell
objdump -t cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/hello.cpp.o
```
Вывод команды:
```
objdump -t cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/hello.cpp.o

cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/hello.cpp.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 hello.cpp
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .debug_info    0000000000000000 .debug_info
0000000000000000 l    d  .debug_abbrev  0000000000000000 .debug_abbrev
0000000000000000 l    d  .debug_line    0000000000000000 .debug_line
0000000000000000 l    d  .debug_str     0000000000000000 .debug_str
0000000000000000 l    d  .debug_line_str        0000000000000000 .debug_line_str
0000000000000000 g     F .text  000000000000000f _Z5helloPKc
```

Символы объектного файла main.o
```shell
objdump -t cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/main.cpp.o
```
Вывод команды:
```
objdump -t cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/main.cpp.o

cmake-build-debug/CMakeFiles/first_helloworld.dir/1/helloworld/main.cpp.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 main.cpp
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .debug_info    0000000000000000 .debug_info
0000000000000000 l    d  .debug_abbrev  0000000000000000 .debug_abbrev
0000000000000000 l    d  .debug_line    0000000000000000 .debug_line
0000000000000000 l    d  .debug_str     0000000000000000 .debug_str
0000000000000000 l    d  .debug_line_str        0000000000000000 .debug_line_str
0000000000000000 g     F .text  0000000000000016 main
```
