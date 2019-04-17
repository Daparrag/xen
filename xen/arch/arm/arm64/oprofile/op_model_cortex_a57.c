/**
 * @file op_model_cortex_a57.c -> used for xenoprof 
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


#include <xen/types.h>
#include <xen/xenoprof.h>
#include <xen/sched.h>
#include <asm/processor.h>
#include <asm/current.h>
#include <asm/arm64/pmu.h>

#include "op_arm64_model.h"
#include "op_counter.h"


static int num_counters = 6;
static struct arm_pmu const *__read_mostly pmu = &arm_cortex_a57;
static unsigned long reset_value[OP_MAX_COUNTER];



static void op_arm_a57_save_counters(struct op_perf_ctr * counters)
{
	int i;

	for(i=0; i< num_counters; i++)
	{
		struct arm_event_entry * entry = &counters[i].event;


		if (counter_config[i].enabled)
		{
			counters[i].val = pmu->read_counter(entry, (void *)pmu ,i);
		}

	}
}

static void op_arm_a57_load_counters(struct op_perf_ctr * counters)
{
	int i;
	for(i=0; i< num_counters; i++)
	{
		struct arm_event_entry * entry = &counters[i].event;

		if (counter_config[i].enabled)
		{
			pmu->write_counter(entry,(void*)pmu,i,counters[i].val);
		}
	
	}

}


static void op_arm_a57_setup_counters(struct op_perf_ctr * counters)
{
	int i;

	/*reset all counters*/
	pmu->reset((void*)pmu);

	/*enable active counters*/
	for(i=0; i <num_counters; i++){

		struct arm_event_entry * entry = &counters[i].event;

		if(counter_config[i].enabled){

			reset_value[i] = counter_config[i].count;

			/* setup event */
			entry->modmsk =  ( ARMV8_PMEVTYPER_EL0_P   & counter_config[i].kernel )  
						 | ( ARMV8_PMEVTYPER_EL0_U   & counter_config[i].user   )
						 | ( ARMV8_PMEVTYPER_EL0_NSH & counter_config[i].hyp    );
			entry->code  =  counter_config[i].evt_code;

			/* enable counter */
			pmu->enable(entry,i);
			/* enable event */
			pmu->event_init(entry,i);
		}else {
			reset_value[i] = 0;
		}


	}

}


static void op_arm_a57_start(struct op_perf_ctr * counters)
{
	int i;

	for (i = 0; i < num_counters; ++i)
		if (reset_value[i]) {

			struct arm_event_entry * evt = &counters[i].event;
			pmu->enable_event(evt,i);
		}


}


static void op_arm_a57_stop(struct op_perf_ctr * counters)
{
	int i;
	for (i = 0; i < num_counters; ++i)
		if(reset_value[i])
		{
			struct arm_event_entry * evt = &counters[i].event;
			pmu->disable(evt, i);
		}

}


static void op_arm_a57_enable_user(void)
{
	pmu->enable_user();
}





struct op_arm_model_spec const op_arm_cortex_a57 =
{	
	.num_counters 			= 6,
	.num_events				= 31,
	.start 					= &op_arm_a57_start,
	.stop					= &op_arm_a57_stop,
	.enable_user            = &op_arm_a57_enable_user,
	.setup_counters			= &op_arm_a57_setup_counters,
	.load_counters			= &op_arm_a57_load_counters,
	.save_counters			= &op_arm_a57_save_counters
};


