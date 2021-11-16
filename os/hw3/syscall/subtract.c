#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

asmlinkage long sys_subtract(int a, int b, int *c)
{
	printk("subtract 연산 호출\n");
	put_user(a - b, c);
	return 0;
}

SYSCALL_DEFINE3(subtract, int, a, int, b, int *, c)
{
	return sys_subtract(a, b, c);
}

