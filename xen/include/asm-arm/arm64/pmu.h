 /******************************************************************************
 *
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


#ifndef __ASM_PMU_H
#define __ASM_PMU_H


#include <xen/types.h>
#include <xen/percpu.h>
#include <xen/errno.h>
#include <asm/bitops.h>
#include <asm/cpufeature.h>
#include <asm/system.h>
#include <asm/processor.h>
#include <asm/arm64/arm_pmu_events.h>

/*GENERIC MACROS*/

#define SHIFR_32(r,n)	(r >> n)
#define SHIFL_32(r,n)	(r << n)

#define	ARMV8_PMU_MAX_COUNTERS	32



#define ARM_PROCESSOR_IMPLEMENTED	0x40
#define PMU_ARCH_CORTEX_A53 		0x03
#define PMU_ARCH_CORTEX_A57 		0x01

/*PMU Macros*/

/*ARM64_PMU_Counters*/
/*MACRO                     NAME               TYPE        Width             DESCRIPTION*/
#define ARMV8_PMCCNTR_EL0  PMCCNTR_EL0   //  RW          64               Performance Monitors Cycle Count Register
/*ARM64_PMU_MONITORS_EVENT_COUNT*/
/*MACRO                       		 NAME                 TYPE       Width            DESCRIPTION*/
#define ARMV8_PMEVCNTRn_EL0(n)  "PMEVCNTR"  #n "_EL0"    // RW		  32		   Performance Monitors Event Count Registers n = 0 - 30
#define ARMV8_PMEVTYPERn_EL0(n) "PMEVTYPER" #n "_EL0"    // RW        32           Performance Monitors Event Type Registers n = 0  - 30



/*ARM64_PMU_controls*/
/*MACRO                     NAME               TYPE        Width             DESCRIPTION*/
#define ARMV8_PMCR_EL0          PMCR_EL0        //  RW         32                Performance Monitors Control Register 
#define ARMV8_PMCNTENSET_EL0    PMCNTENSET_EL0  //  RW         32                Performance Monitors Count Enable Set Register
#define ARMV8_PMCNTENCLR_EL0    PMCNTENCLR_EL0  //  RW         32                Performance Monitors Count Enable Clear Register 
#define ARMV8_PMOVSCLR_EL0      PMOVSCLR_EL0    //  RW         32                Performance Monitors Overflow Flag Status Register
#define ARMV8_PMSWINC_EL0       PMSWINC_EL0     //  WO         32                Performance Monitors Software Increment Register
#define ARMV8_PMSELR_EL0        PMSELR_EL0      //  RW         32                Performance Monitors Event Counter Selection Register
#define ARMV8_PMCEID0_EL0       PMCEID0_EL0     //  RO         32                Performance Monitors Common Event Identification Register
#define ARMV8_PMCEID1_EL0       PMCEID1_EL0     //  RO         32                Performance Monitors Common Event Identification Register
#define ARMV8_PMXEVTYPER_EL0    PMXEVTYPER_EL0  //  RW         32                Performance Monitors Selected Event Type and Filter Register
#define ARMV8_PMCCFILTR_EL0     PMCCFILTR_EL0   //  RW         32                Performance Monitors Cycle Count Filter Register
#define ARMV8_PMXEVCNTR_EL0     PMXEVCNTR_EL0   //  RW         32                Performance Monitors Selected Event Count Register
#define ARMV8_PMUSERENR_EL0     PMUSERENR_EL0   //  RW         32                Performance Monitors User Enable Register
/*ARMV8_INTERRUPT_MACROS*/
#define ARMV8_PMINTENSET_EL1    PMINTENSET_EL1  //  RW         32                Performance Monitors Interrupt Enable Set Register
#define ARMV8_PMINTENCLR_EL1    PMINTENCLR_EL1  //  RW         32                Performance Monitors Interrupt Enable Clear Register
#define ARMV8_PMOVSSET_EL0      PMOVSSET_EL0    //  RW         32                Performance Monitors Overflow Flag Status Set Register
#define ARMV8_PMCCFILTR_EL0     PMCCFILTR_EL0   //  RW         32                Performance Monitors Cycle Count Filter Register

/*ARMV0_PMU_ENABLE_MACROS*/
#define ARMV8_PMCNTENSET_EL0_ENABLE                    BIT(31)      	// bit for enable cycle counter register and 
#define ARMV8_ALL_COUNTERS_ENABLE                      BIT(0)       	// bit for enable all implemented event counter PMEVCNTR<n>_EL0
#define ARMV8_PMCNTENSET_EL0_DISABLE                   BIT(31)      	// bit for disable cycle counter register and all mplemented event counter PMEVCNTR<n>_EL0
#define ARMV8_PMCR_EL0_OVERFLOW_FLAG_RESET			   BIT(6)		    // Overflow on 64 bit cycle counter 
#define ARMV8_PMCR_EL0_CYCLE_COUNT_RESET               BIT(2)       	// bit for reset cycle counter register whithout affecting overflow flag
#define ARMV8_PMCR_EL0_EVENT_COUNT_RESET               BIT(1)       	// bit for reset all implemented event counter PMEVCNTR<n>_EL0 whithout affecting overflow flag
#define ARMV8_PMOVSCLR_EL0_CLEAR_CYCLE_CONT_OVERFLOW   BIT(31)       	// bit for clear cycle counter overflow flag
#define ARMV8_PMOVSCLR_EL0_CLEAR_EVENT_CONT_OVERFLOW   (0x7fffffff) 	// clear all event counter overflow flag
#define ARMV8_MOVSCLR_EL0_CLEAR_UNIQUE_EVENT_OVERFLOW(n)  	BIT(n)    	// clear specific counter overflow flag 
/*
 * PMUSERENR: user enable reg
 */
#define ARMV8_PMU_USERENR_EL0_EN						BIT(0) 		// PMU regs can be accessed at EL0 
#define ARMV8_PMU_USERENR_EL0_SW						BIT(1) 		// PMSWINC can be written at EL0 
#define ARMV8_PMU_USERENR_EL0_CR						BIT(2) 		// Cycle counter can be read at EL0
#define ARMV8_PMU_USERENR_EL0_ER						BIT(3) // Event counter can be read at EL0 //
/*
 * Event filters for PMUv3
 */
#define ARMV8_PMEVTYPER_EL0_P							BIT(31)		// Count Events in EL1
#define ARMV8_PMEVTYPER_EL0_U							BIT(30)		// Count Events in EL0
#define ARMV8_PMEVTYPER_EL0_NSK							BIT(29)		// If the value of this bit is equal to the value of the PMEVTYPER<n>_EL0.P bit, events in Non-secure EL1 are counted.
#define ARMV8_PMEVTYPER_EL0_NSU							BIT(28)		// If the value of this bit is equal to the value of the PMEVTYPER<n>_EL0.U bit, events in Non-secure EL0 are counted.
#define ARMV8_PMEVTYPER_EL0_NSH							BIT(27)		// Count events in EL2

/*ARMV8_PMU_MASK*/
#define	ARMV8_PMU_OVSR_MASK		0xffffffff	/* Mask for writable bits */
#define	ARMV8_PMU_OVERFLOWED_MASK	ARMV8_PMU_OVSR_MASK
#define	ARMV8_PMU_COUNTER_MASK	(ARMV8_PMU_MAX_COUNTERS - 1)
#define ARMV8_PMU_ARCH_ID_MASK 0xFF0000	
/*
 * PMXEVTYPER: Event selection reg
 */
#define	ARMV8_PMU_EVTYPE_MASK	0xc800ffff	/* Mask for writable bits */
#define	ARMV8_PMU_EVTYPE_EVENT	0xffff		/* Mask for EVENT bits */

/*pmu_status*/
#define ARMV8_PMU_IDLE			0
#define ARMV8_PMU_INITIALIZED	1
#define ARMV8_PMU_READY			2	



struct arm_pmu{

	const char 	*desc;				 		/* pmu description */
	const char 	*name;				 		/* pmu short name */
	unsigned int num_cntrs;
	unsigned int	max_encoding;	 		/* max number of uint64_t to encode an event */
	unsigned int	flags;			 		/* 16 LSB: common, 16 MSB: arch spec*/
	unsigned int	num_fixed_cntrs; 		/* number of fixed counters */
	unsigned int	supported_plm;	 		/* supported priv levels */
	const void	*pe;			     		/* pointer to event table */
	unsigned int	num_events;      		/* number of events*/
	unsigned int state;       				/* pmu status for the PMU*/
	/*
	 * A 1 bit for an index indicates that the counter is being used for
	 * an event. A 0 means that the counter can be used.
	 */
	DECLARE_BITMAP(count_mask, ARMV8_PMU_MAX_COUNTERS);

	uint64_t (*read_counter)   (struct arm_event_entry *, void * , int );
	void     (*write_counter)  (struct arm_event_entry *, void *, int, uint64_t);
	void     (*clear_event_idx)(struct arm_event_entry * evt, void* this,  int idx);
	void     (*enable)         (struct arm_event_entry* ,  int );	
	void     (*disable)        (struct arm_event_entry* ,  int );
	void     (*start) (void);
	void     (*stop)  (void);
	void     (*reset) (void *this);

	/* event & event_hadlers */
	int  (*get_event_idx)   (struct arm_event_entry * , void * );
	int  (*set_event_filter)(struct arm_event_entry * );
	int  (*filter_match)    (struct arm_event_entry * ); 
	int  (*event_is_valid)  (struct arm_event_entry * evt);
	void (*event_init)      (struct arm_event_entry * , int );
	void (*enable_event)    (struct arm_event_entry* ,  int );
	void (*handler_irq)     (void * this); 

	/*special operations*/
	void (*enable_user)(void);

};

int armv8pmu_get_arch(void);
int armv8pmu_init(struct arm_pmu *cpu_pmu);


extern struct arm_pmu arm_cortex_a57;
extern struct arm_pmu arm_cortex_a53;
// per CPU declarations 

#endif /* __ASM_PMU64_H */