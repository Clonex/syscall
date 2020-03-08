cp syscall_64.tbl ../linux-5.6-rc1/arch/x86/entry/syscalls/syscall_64.tbl
cp unistd_64.h ../linux-5.6-rc1/arch/x86/include/generated/uapi/asm/unistd_64.h
cp testsyscall.c ../linux-5.6-rc1/testsyscall.c
cp spawnThreads.sh ../linux-5.6-rc1/spawnThreads.sh
cp kernelMakefile ../linux-5.6-rc1/arch/um/kernel/Makefile

cp -a headers/. ../linux-5.6-rc1/arch/um/include/asm
cp -a kernel/. ../linux-5.6-rc1/arch/um/kernel/