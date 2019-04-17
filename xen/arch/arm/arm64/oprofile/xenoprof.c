/******************************************************************************
 * arch/arm/arm64/oprofile/xenoprof
 *
 * @file xenoprof.c -> used for xenoprof 
 *	
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author Diego Alejadro Parra Guzman <daparrag@correo.udistrital.edu.co>
 *
 */

#include <xen/guest_access.h>
#include <xen/sched.h>
#include <xen/xenoprof.h>
#include <public/xenoprof.h>
#ifdef COMPAT
#include <compat/xenoprof.h>
#endif

#include "op_counter.h"

int xenoprof_arch_counter(XEN_GUEST_HANDLE_PARAM(void) arg)
{ 

	struct xenoprof_counter counter;

	if(copy_from_guest(&counter, arg, 1))
		return -EFAULT;

	if (counter.ind >= OP_MAX_COUNTER)
		return -E2BIG;

	counter_config[counter.ind].count     = counter.count;
    counter_config[counter.ind].enabled   = counter.enabled;
    counter_config[counter.ind].evt_code  = counter.event;
    counter_config[counter.ind].kernel    = counter.kernel;
    counter_config[counter.ind].user      = counter.user;
    counter_config[counter.ind].unit_mask = counter.unit_mask;

	return 0;
}


int xenoprof_arch_ibs_counter(XEN_GUEST_HANDLE_PARAM(void) arg)
{
	return 0;
}

int compat_oprof_arch_counter(XEN_GUEST_HANDLE_PARAM(void) arg)
{
	return 0;
}

int xenoprofile_get_mode(struct vcpu *curr, const struct cpu_user_regs *regs)
{

	if(!guest_mode(regs)) return 2;
	if(!psr_mode_is_user(regs)) return 1;

	return 0;
}





