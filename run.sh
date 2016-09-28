make clean && make DEBUG=1
qemu-system-x86_64 -kernel kernel -serial stdio -s
#vncviewer 127.0.0.1:5900
#gdb kernel
#set architecture i386:x86-64
#target remote localhost:1234
#set var wait=0


