#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

asmlinkage long sys_add(int a, int b, int *c)
{
	printk("add 연산 호출\n");
	put_user(a + b, c);
	return 0;
}

SYSCALL_DEFINE3(add, int, a, int, b, int *, c)
{
	return sys_add(a, b, c);
}
