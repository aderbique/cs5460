/*
 * Copyright (c) 1997-8,2007 Andrew G Morgan <morgan@kernel.org>
 *
 * This file deals with setting capabilities on processes.
 */

#include "libcap.h"

cap_t cap_get_proc(void)
{
    cap_t result;

    /* allocate a new capability set */
    result = cap_init();
    if (result) {
	_cap_debug("getting current process' capabilities");

	/* fill the capability sets via a system call */
	if (capget(&result->head, &result->u[0].set)) {
	    cap_free(result);
	    result = NULL;
	}
    }

    return result;
}

int cap_set_proc(cap_t cap_d)
{
    int retval;

    if (!good_cap_t(cap_d)) {
	errno = EINVAL;
	return -1;
    }

    _cap_debug("setting process capabilities");
    retval = capset(&cap_d->head, &cap_d->u[0].set);

    return retval;
}

/* the following two functions are not required by POSIX */

/* read the caps on a specific process */

int capgetp(pid_t pid, cap_t cap_d)
{
    int error;

    if (!good_cap_t(cap_d)) {
	errno = EINVAL;
	return -1;
    }

    _cap_debug("getting process capabilities for proc %d", pid);

    cap_d->head.pid = pid;
    error = capget(&cap_d->head, &cap_d->u[0].set);
    cap_d->head.pid = 0;

    return error;
}

/* set the caps on a specific process/pg etc.. */

int capsetp(pid_t pid, cap_t cap_d)
{
    int error;

    if (!good_cap_t(cap_d)) {
	errno = EINVAL;
	return -1;
    }

    _cap_debug("setting process capabilities for proc %d", pid);
    cap_d->head.pid = pid;
    error = capset(&cap_d->head, &cap_d->u[0].set);
    cap_d->head.version = _LINUX_CAPABILITY_VERSION;
    cap_d->head.pid = 0;

    return error;
}

/* Disable a cap on current process */
int cap_disable(cap_value_t capflag)
{
	cap_t mycaps;
	
	mycaps = cap_get_proc();
	if (mycaps == NULL)
		return -1;
	if (cap_set_flag(mycaps, CAP_EFFECTIVE, 1, &capflag, CAP_CLEAR) != 0)
		return -1;
	if (cap_set_proc(mycaps) != 0)
		return -1;
	return 0;
}

/* Enalbe a cap on current process */
int cap_enable(cap_value_t capflag)
{
	cap_t mycaps;
	
	mycaps = cap_get_proc();
	if (mycaps == NULL)
		return -1;
	if (cap_set_flag(mycaps, CAP_EFFECTIVE, 1, &capflag, CAP_SET) != 0)
		return -1;
	if (cap_set_proc(mycaps) != 0)
		return -1;
	return 0;
}

/* Drop a cap on current process */
int cap_drop(cap_value_t capflag)
{
	cap_t mycaps;
	
	mycaps = cap_get_proc();
	if (mycaps == NULL)
		return -1;
	if (cap_set_flag(mycaps, CAP_EFFECTIVE, 1, &capflag, CAP_CLEAR) != 0)
		return -1;
	if (cap_set_flag(mycaps, CAP_PERMITTED, 1, &capflag, CAP_CLEAR) != 0)
		return -1;
	if (cap_set_proc(mycaps) != 0)
		return -1;
	return 0;
}

