#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include <fcntl.h>

int main(void)
{
	if (open ("/etc/shadow",O_RDONLY) < 0)
		return -1;
	if (cap_disable(CAP_DAC_READ_SEARCH) < 0)
		return -2;
	if (open ("/etc/shadow", O_RDONLY) > 0)
		return -3;
	if (cap_enable(CAP_DAC_READ_SEARCH) < 0)
		return -4;
	if (open ("/etc/shadow",O_RDONLY) < 0)
		return -5;
	if (cap_drop(CAP_DAC_READ_SEARCH) < 0)
		return -6;
	if (open ("/etc/shadow",O_RDONLY) > 0)
		return -7;
	if (cap_enable(CAP_DAC_READ_SEARCH) == 0)
		return -8;
	printf("succeed!\n");
	return 0;
}
