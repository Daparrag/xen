/******************************************************************************
 * @file pmu_int.c -> used for xen 
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


#include <xen/event.h>
#include <xen/types.h>
#include <xen/errno.h>
#include <xen/init.h>
#include <xen/string.h>
#include <xen/delay.h>
#include <xen/xenoprof.h>
#include <public/xen.h>
#include <asm/arm64/pmu.h>


#include "op_arm64_model.h"
#include "op_counter.h"



static char *cpu_type;

struct op_counter_config counter_config[OP_MAX_COUNTER];
static struct op_pmu cpu_pmu[NR_CPUS];
struct op_arm_model_spec const *__read_mostly model;




static void pmu_save_registers(void * dummy)
{
	
	int cpu = smp_processor_id();
	struct op_pmu * pmu_data = &cpu_pmu[cpu];
	model->save_counters(pmu_data->counters);


}

static void pmu_restore_registers(struct op_pmu * pmu_data)
{	
	model->load_counters( pmu_data ->counters);
}



static void free_pmu_data(void)
{
	int i;
	for (i = 0; i < nr_cpu_ids; ++i) {
		xfree(cpu_pmu[i].counters);
		cpu_pmu[i].counters = NULL;
	}

}


static int allocate_pmu_data(void)
{
	int success = 1; 
	int i;
	
	size_t counter_size = sizeof(struct op_perf_ctr) * model->num_counters;
	for_each_online_cpu (i)
	{
		cpu_pmu[i].counters = xmalloc_bytes(counter_size);
		if (!cpu_pmu[i].counters) {
			success = 0;
			break;
		}
	}

	if(!success)
		free_pmu_data();

	return success;

}


static void pmu_cpu_setup(void * dummy)
{
	int cpu = smp_processor_id();
	struct op_pmu * pmu_data = &cpu_pmu[cpu];
	model->setup_counters(pmu_data->counters);
}

static void pmu_cpu_enable_user(void * dummy)
{
	model->enable_user();
}

int pmu_setup_events(void)
{
	on_each_cpu(pmu_cpu_setup, NULL, 1);
	return 0;
}

int pmu_reserve_counters(void)
{
	if(!allocate_pmu_data())
		return -ENOMEM;

	on_each_cpu(pmu_save_registers, NULL, 1);
	return 0;

}

int pmu_enable_virq(void)
{
	return 0;
}

int pmu_disable_virq(void)
{
	return 0;
}

int pmu_enable_user(void)
{
	on_each_cpu(pmu_cpu_enable_user,NULL,1);
	return 0;
}


static void pmu_cpu_shutdown(void * dummy)
{
	int cpu = smp_processor_id();
	struct op_pmu * pmu_data = &cpu_pmu[cpu];
	pmu_restore_registers(pmu_data);
}

void pmu_release_counters(void)
{
	on_each_cpu(pmu_cpu_shutdown, NULL, 1);
	free_pmu_data();
}


/**************FIXME******************/
static void pmu_cpu_start(void * dummy)
{
	int cpu = smp_processor_id();
	struct op_pmu * pmu_data = &cpu_pmu[cpu];

	/*
	 * FIXME: Interrupt .
	 */

	model->start(pmu_data->counters);
}

int pmu_start(void)
{
	on_each_cpu(pmu_cpu_start, NULL, 1);
	return 0;
}


static void pmu_cpu_stop(void * dummy)
{
	int cpu = smp_processor_id();
	struct op_pmu * pmu_data = &cpu_pmu[cpu];
	model->stop(pmu_data->counters);

}

void pmu_stop(void)
{
	on_each_cpu(pmu_cpu_stop, NULL, 1);
}



static int __init pmu_init(void)
{
	/*FIXME: this code has to be tested*/
	__u8 architecture = current_cpu_data.midr.architecture;
	__u8 implementer  = current_cpu_data.midr.implementer;
	__u8 variant = 0x00;

	if(implementer != ARM_PROCESSOR_IMPLEMENTED)
	{
		printk("xenoprof: Initialization failed. "
		       "Unsupported processor. Unknown implementer %d\n",
	    		implementer);
		return -ENODEV;
	}

	if (architecture != 0xf){
		printk("xenoprof: Initialization failed. "
		       "Unsupported processor. Unknown architecture %d\n",
	    		architecture);

		return -ENODEV;
	}

	variant = armv8pmu_get_arch();

	switch(variant)
	{
		case PMU_ARCH_CORTEX_A57:
		cpu_type= "arm_ac57";
		model =&op_arm_cortex_a57;
		break;
		case PMU_ARCH_CORTEX_A53:
			cpu_type= "arm_ac53";
		break;
		default:
		printk("xenoprof: Initialization failed. "
			       "Unsupported processor. Unknown pmu_architecture %d\n",
				variant);
			return -ENODEV;
	}

return 0;

}


__initcall(pmu_init);
int xenoprof_arch_init(int *num_events, char *_cpu_type)
{
	if (cpu_type == NULL)
		return -ENODEV;
	*num_events = model->num_events;
	strlcpy(_cpu_type, cpu_type, XENOPROF_CPU_TYPE_SIZE);
	return 0;
}
