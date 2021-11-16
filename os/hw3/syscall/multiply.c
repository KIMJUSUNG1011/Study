#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

asmlinkage long sys_multiply(int a, int b, int *c)
{
	printk("multiply 연산 호출\n");
	put_user(a * b, c);
	return 0;
}

SYSCALL_DEFINE3(multiply, int, a, int, b, int *, c)
{
	return sys_multiply(a, b, c);
}
