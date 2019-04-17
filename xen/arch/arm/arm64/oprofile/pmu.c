//xen/arch/arm-asm/arm64/

/******************************************************************************
 *  pmu.h: ARM PMU specification.
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
 */

#include <xen/types.h>
#include <xen/percpu.h>
#include <xen/errno.h>
#include <xen/bitops.h>
#include <xen/sched.h>


#include <asm/current.h>
#include <asm/cpufeature.h>
#include <asm/system.h>
#include <asm/processor.h>


/*FIXME: remove /asm/arm64 declaration*/
#include <asm/arm64/pmu.h>
#include <asm/arm64/arm_cortex_a57_events.h>

/*
 * low level cycle counter 
 */
#define	ARMV8_IDX_CYCLE_COUNTER	0
#define	ARMV8_IDX_COUNTER0	1
#define	ARMV8_IDX_COUNTER_LAST(cpu_pmu) \
	(ARMV8_IDX_CYCLE_COUNTER + cpu_pmu->num_events - 1)


//#ifndef __ASSEMBLY__

#define READ_PMUREG32(name) ({                           \
    uint32_t _r;                                		 \
	asm volatile("mrs %0," (name) : "=r"(_r));          	 \
    _r; })

#define WRITE_PMUREG32(v, name) do {                    \
    uint32_t _r = v;                                    \
    asm volatile ("msr " name", %0" : : "r" (_r));       \
} while (0)

#define WRITE_PMUREG64(v, name) do {                    \
    uint64_t _r = v;                                    \
    asm volatile ("msr " name", %0" : : "r" (_r));       \
} while (0)

#define READ_PMUREG64(name) ({                          \
    uint64_t _r;                                        \
    asm volatile ("mrs  %0, " (name) : "=r" (_r));         \
    _r; })

#define READ_PMUREG(name)     READ_SYSREG32(name)
#define WRITE_PMUREG(v, name) WRITE_SYSREG32(v, name)

//#endif /* __ASSEMBLY__ */

/*
 * ARMv8 low level PMU access
 */

/*
 * low level counters mapping
 */
#define	ARMV8_IDX_TO_COUNTER(x)	\
	(((x) - ARMV8_IDX_COUNTER0) & ARMV8_PMU_COUNTER_MASK)




static inline bool armv8pmu_event_is_64bit(struct arm_event_entry * evt)
{
	return   evt->evt_64bit &  0x1;
}

static inline bool armv8pmu_event_is_chained(struct arm_event_entry *evt, int idx)
{
	return armv8pmu_event_is_64bit(evt) && 
		   (idx != ARMV8_IDX_CYCLE_COUNTER);
}

static inline uint32_t armv8pmu_pmcr_read(void)
{
	return READ_PMUREG(ARMV8_PMCR_EL0);
}


static inline void armv8pmu_pmcr_write(uint32_t val)
{
	val &= 0x7f; /* Mask for writable bits */
	isb();
	WRITE_PMUREG(val, ARMV8_PMCR_EL0);
}

static inline int armv8pmu_has_overflowed(uint32_t pmovsr)
{
	return pmovsr & ARMV8_PMU_OVERFLOWED_MASK;
}


static inline int armv8pmu_counter_valid(void * this , int idx)
{
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;
	return idx >= ARMV8_IDX_CYCLE_COUNTER &&
		   idx <= ARMV8_IDX_COUNTER_LAST(cpu_pmu);
}

static inline int armv8pmu_counter_has_overflowed(uint32_t pmnc, int idx)
{
	return pmnc & BIT(ARMV8_IDX_TO_COUNTER(idx));
}

static inline void armv8pmu_select_counter(int idx)
{
	uint32_t counter = ARMV8_IDX_TO_COUNTER(idx);
	WRITE_PMUREG(counter, ARMV8_PMSELR_EL0);
	isb();
}

static inline uint32_t armv8pmu_read_evcntr(int idx)
{
	armv8pmu_select_counter(idx);
	return READ_PMUREG(ARMV8_PMXEVCNTR_EL0);	
}

static inline uint64_t armv8pmu_read_hw_counter(struct arm_event_entry * evt,  int idx)
{

	uint64_t val = 0; 
	val = armv8pmu_read_evcntr(idx);
	if (armv8pmu_event_is_chained(evt, idx))
		val = (val<<32) | armv8pmu_read_evcntr(idx - 1 );
	return val; 
}


static inline uint64_t armv8pmu_read_counter (struct arm_event_entry * evt, void * this, int idx)
{
	uint64_t val = 0; 
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;
	if(!armv8pmu_counter_valid(cpu_pmu, idx))
		printk("CPU%u reading wrong counter %d\n", smp_processor_id(), idx);
	else if (idx == ARMV8_IDX_CYCLE_COUNTER)
		val = READ_PMUREG(ARMV8_PMCCNTR_EL0);
	else
		val = armv8pmu_read_hw_counter(evt , idx);

	return val;
}


static inline void armv8pmu_write_evcntr(int idx, uint32_t value)
{
	armv8pmu_select_counter(idx);
	WRITE_PMUREG(value, ARMV8_PMXEVCNTR_EL0);
}

static inline void armv8pmu_write_hw_counter(struct arm_event_entry * evt, int idx,
					     uint64_t value)
{
	if(armv8pmu_event_is_chained(evt, idx))
	{
		uint32_t upper_val = (value & 0xFFFFFFFF00000000ULL) >> 31;
		uint32_t lower_val = (value & 0xFFFFFFFFUL);	
		armv8pmu_write_evcntr(idx, upper_val);
		armv8pmu_write_evcntr(idx - 1, lower_val);
	} else {
		armv8pmu_write_evcntr(idx, value);
	}
}


static inline void armv8pmu_write_counter(struct arm_event_entry * evt, void * this, int idx , uint64_t value)
{
	
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;

	if(!armv8pmu_counter_valid(cpu_pmu, idx))
		printk("CPU%u writing wrong counter %d\n", smp_processor_id(), idx);
	else if (idx == ARMV8_IDX_CYCLE_COUNTER)
	{
		/*
		 * The cycles counter is really a 64-bit counter.
		 * When treating it as a 32-bit counter, we only count
		 * the lower 32 bits, and set the upper 32-bits so that
		 * we get an interrupt upon 32-bit overflow.
		 */
		if(!armv8pmu_event_is_64bit(evt))
		
			value |= 0xffffffff00000000ULL;
			WRITE_PMUREG(value, ARMV8_PMCCNTR_EL0);
	} else 
		armv8pmu_write_hw_counter(evt, idx, value);

}


static inline void armv8pmu_write_evtype(int idx, uint32_t val)
{
	armv8pmu_select_counter(idx);
	val &= ARMV8_PMU_EVTYPE_MASK;
	WRITE_PMUREG(val, ARMV8_PMXEVTYPER_EL0);
	isb();
}

static inline void armv8pmu_write_event_type(struct arm_event_entry * evt, int idx)
{
	uint32_t chain_evt=0;
	/*
	 * For chained events, the low counter is programmed to count
	 * the event of interest and the high counter is programmed
	 * with CHAIN event code with filters set to count at all ELs.
	 */
	if(armv8pmu_event_is_chained(evt,idx))
	{
		chain_evt = evt->modmsk;
		armv8pmu_write_evtype(idx - 1, evt->code);
		armv8pmu_write_evtype(idx, chain_evt);				

	}else{
		chain_evt =  evt->modmsk | evt->code;
		armv8pmu_write_evtype(idx, chain_evt);
	}
}

static inline int armv8pmu_enable_counter(int idx)
{
	uint32_t counter = ARMV8_IDX_TO_COUNTER(idx);
	uint32_t val = BIT(counter); 
	WRITE_PMUREG(val,ARMV8_PMCNTENSET_EL0);
	return idx;
}

static inline void armv8pmu_enable_event_counter(struct arm_event_entry * evt , int idx)
{
	armv8pmu_enable_counter(idx);
	if (armv8pmu_event_is_chained(evt,idx))
		armv8pmu_enable_counter(idx - 1);
	isb();
}

static inline int armv8pmu_disable_counter(int idx)
{
	uint32_t counter = ARMV8_IDX_TO_COUNTER(idx);
	uint32_t val = BIT(counter); 
	WRITE_PMUREG(val,ARMV8_PMCNTENSET_EL0);
	return idx;
}


static inline void armv8pmu_disable_event_counter(struct arm_event_entry * evt, int idx)
{
	
	if(armv8pmu_event_is_chained(evt,idx))
	  	 armv8pmu_disable_counter(idx - 1);
	armv8pmu_disable_counter(idx);
}


static inline int armv8pmu_enable_intens(int idx)
{
	uint32_t counter = ARMV8_IDX_TO_COUNTER(idx);
	uint32_t val = BIT(counter); 
	WRITE_PMUREG(val, ARMV8_PMINTENSET_EL1);
	return idx;
}


static inline int armv8pmu_enable_event_irq(int idx)
{
	return armv8pmu_enable_intens(idx);
}




static inline int armv8pmu_disable_intens(int idx)
{
	uint32_t counter = ARMV8_IDX_TO_COUNTER(idx);
	uint32_t val = BIT(counter); 
	WRITE_PMUREG(val, ARMV8_PMINTENCLR_EL1);
	isb();
	/* Clear the overflow flag in case an interrupt is pending. */
	WRITE_PMUREG(val, ARMV8_PMOVSCLR_EL0);
	isb();
	return idx;
}

static inline int armv8pmu_disable_event_irq(int idx)
{
	return armv8pmu_disable_intens (idx);
}


static inline uint32_t armv8pmu_getreset_flags(void)
{
	uint32_t value;
	/* Read */
	value = READ_PMUREG(ARMV8_PMOVSCLR_EL0);

	/* Write to clear flags */
	value &= ARMV8_PMU_OVSR_MASK;
	WRITE_PMUREG(value,ARMV8_PMOVSCLR_EL0);

	return value;

}



static void armv8pmu_enable_event(struct arm_event_entry* evt,  int idx)
{
	/*
	 * Disable counter
	 */
	armv8pmu_disable_event_counter(evt, idx);
	/*
	 * Set event (if destined for PMNx counters).
	 */

	/*
	 * Enable interrupt for this counter
	 */
	armv8pmu_enable_event_irq(idx);
	/*
	 * Enable counter
	 */	
	armv8pmu_enable_event_counter(evt, idx);
}


static void armv8pmu_disable_event(struct arm_event_entry * evt, int idx)
{

	/*
	 * Disable counter
	 */
		armv8pmu_disable_event_counter(evt,idx);

	/*
	 * Disable interrupt for this counter
    */
		armv8pmu_disable_event_irq(idx);

}

static void armv8pmu_start(void)
{
	/* Enable all counters */
	armv8pmu_pmcr_write(armv8pmu_pmcr_read() | ARMV8_ALL_COUNTERS_ENABLE);
}

static void armv8pmu_stop(void)
{
	/* Disable all counters */
	armv8pmu_pmcr_write(armv8pmu_pmcr_read() & ~ARMV8_ALL_COUNTERS_ENABLE);
}


static void  armv8pmu_handle_irq(void * this) // this should be extern. 
{

	// uint32_t pmovsr;
	// /*
	//  * Get and reset the IRQ flags
	//  */
	// pmovsr = armv8pmu_getreset_flags();

	// /*
	//  * Did an overflow occur?
	//  */
	// if (!armv8pmu_has_overflowed(pmovsr))
}


static int armv8pmu_get_single_idx(void * this)
{
	int idx;
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;

	for (idx = ARMV8_IDX_COUNTER0; idx < cpu_pmu->num_events; idx ++)
	{
		if(!test_and_set_bit(idx, cpu_pmu->count_mask))
			return idx;
	}
	return -EAGAIN;
}

static int armv8pmu_get_chain_idx(void * this)
{
	int idx;
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;
	/*
	 * Chaining requires two consecutive event counters, where
	 * the lower idx must be even.
	 */
	for (idx = ARMV8_IDX_COUNTER0 + 1; idx < cpu_pmu->num_events; idx += 2){
		if (!test_and_set_bit(idx, cpu_pmu->count_mask)) {
			/* Check if the preceding even counter is available */
			if (!test_and_set_bit(idx - 1, cpu_pmu->count_mask))
				return idx;
			/* Release the Odd counter */
			clear_bit(idx, cpu_pmu->count_mask);
		}
	}
	return -EAGAIN;
}


static int armv8pmu_get_event_idx(struct arm_event_entry * evt, void* this )
{

	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;
	/* Always prefer to place a cycle counter into the cycle counter. */
	if( evt->code ==  ARMV8_PMUV3_PERFCTR_CPU_CYCLES){
		if (!test_and_set_bit(ARMV8_IDX_CYCLE_COUNTER, cpu_pmu->count_mask))
			return ARMV8_IDX_CYCLE_COUNTER;
	}
	/*
	 * Otherwise use events counters
	 */
	if(armv8pmu_event_is_64bit(evt))
		return armv8pmu_get_chain_idx (cpu_pmu);
	else
		return armv8pmu_get_single_idx(cpu_pmu);
}


static void armv8pmu_clear_event_idx(struct arm_event_entry * evt, void* this,  int idx)
{
	struct arm_pmu * cpu_pmu = (struct arm_pmu *)this;

	clear_bit(idx, cpu_pmu->count_mask);
	if(armv8pmu_event_is_64bit(evt))
		clear_bit(idx - 1, cpu_pmu->count_mask);
}


/*
 * Add an event filter to a given event.
 */
static int armv8pmu_set_event_filter(struct arm_event_entry * evt)
{
	return 0;
}

static int armv8pmu_filter_match(struct arm_event_entry * evt)
{
	unsigned long evtype =  evt->code;
	return evtype != ARMV8_PMUV3_PERFCTR_CHAIN;
}


static void armv8pmu_reset(void *this)
{
	
	struct arm_pmu *cpu_pmu = (struct arm_pmu *)this;
	uint32_t idx, nb_cnt = cpu_pmu->num_events;
	/* The counter and interrupt enable registers are unknown at reset. */
	for (idx = ARMV8_IDX_CYCLE_COUNTER; idx < nb_cnt; ++idx) {
		armv8pmu_disable_counter(idx);
		armv8pmu_disable_intens(idx);
	}

	/*
	 * Initialize & Reset PMNC. Request overflow interrupt for
	 * 64 bit cycle counter but cheat in armv8pmu_write_counter().
	 */
	armv8pmu_pmcr_write(ARMV8_PMCR_EL0_EVENT_COUNT_RESET | ARMV8_PMCR_EL0_CYCLE_COUNT_RESET |  
		ARMV8_PMCR_EL0_OVERFLOW_FLAG_RESET);

}

int armv8pmu_get_arch(void)
{
	return (READ_PMUREG(ARMV8_PMCR_EL0) & ARMV8_PMU_ARCH_ID_MASK) >> 16;
}


static void armv8pmu_enable_user(void)
{
	uint32_t val;

	val = READ_PMUREG(ARMV8_PMUSERENR_EL0);
	val |=ARMV8_PMU_USERENR_EL0_EN & 0x0F;
	WRITE_PMUREG(val,ARMV8_PMUSERENR_EL0);	
}

static int armv8pmu_validate_event(struct arm_event_entry * evt)
{
	return 1;
}




int armv8pmu_init(struct arm_pmu *cpu_pmu)
{
	if(cpu_pmu == NULL) 
		return 0;

	switch (armv8pmu_get_arch()){

		case PMU_ARCH_CORTEX_A53:
			cpu_pmu=&arm_cortex_a53;
			return 1;

		case PMU_ARCH_CORTEX_A57:
			cpu_pmu=&arm_cortex_a53;
			return 1;
		default: 
			return 0;
	}
	    
}






struct arm_pmu arm_cortex_a53 = {
	.desc			  = "ARM Cortex A53",
	.name			  = "arm_ac53",
	.max_encoding	  = 1,
	.num_cntrs		  = 6,
	.pe				  = NULL,
	.num_events       = 0,
	.state			  = ARMV8_PMU_IDLE,
	.read_counter     = &armv8pmu_read_counter,
	.write_counter    = &armv8pmu_write_counter,
	.enable			  = &armv8pmu_enable_event,
	.disable 		  = &armv8pmu_disable_event,
	.start 			  = &armv8pmu_start,
	.stop 			  = &armv8pmu_stop,
	.reset 			  = &armv8pmu_reset,
	.event_init		  = &armv8pmu_write_event_type,
	.enable_event     = &armv8pmu_enable_event,
	.get_event_idx    = &armv8pmu_get_event_idx, 
	.set_event_filter = &armv8pmu_set_event_filter,
	.filter_match     = &armv8pmu_filter_match,
	.event_is_valid   = &armv8pmu_validate_event,
	.clear_event_idx  = &armv8pmu_clear_event_idx,
	.enable_user      = &armv8pmu_enable_user,
	.handler_irq 	  = &armv8pmu_handle_irq


};

struct arm_pmu arm_cortex_a57 = {

	.desc			  = "ARM Cortex A57",
	.name			  = "arm_ac57",
	.max_encoding	  = 1,
	.num_cntrs		  = 6,
	.pe 			  = arm_cortex_a57_pe,
	.num_events       = EVENT_ARRAY_SIZE(arm_cortex_a57_pe),
	.read_counter     = &armv8pmu_read_counter,
	.write_counter    = &armv8pmu_write_counter,
	.enable			  = &armv8pmu_enable_event,
	.disable 		  = &armv8pmu_disable_event,
	.start 			  = &armv8pmu_start,
	.stop 			  = &armv8pmu_stop,
	.reset 			  = &armv8pmu_reset,
	.event_init		  = &armv8pmu_write_event_type,
	.enable_event     = &armv8pmu_enable_event,
	.get_event_idx    = &armv8pmu_get_event_idx, 
	.set_event_filter = &armv8pmu_set_event_filter,
	.filter_match     = &armv8pmu_filter_match,
	.event_is_valid   = &armv8pmu_validate_event,
	.clear_event_idx  = &armv8pmu_clear_event_idx,
	.enable_user      = &armv8pmu_enable_user,
	.handler_irq 	  = &armv8pmu_handle_irq
};