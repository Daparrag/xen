/*
 * Copyright (c) 2014 Google Inc. All rights reserved
 * Contributed by Stephane Eranian <eranian@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Cortex A57 r1p1
 * based on Table 11-24 from the "Cortex A57 Technical Reference Manual"
 */

#ifndef __ARM_A57_EVENTS_H
#define __ARM_A57_EVENTS_H



#define ARM_ATTR_K	0 /* pl1 priv level */
#define ARM_ATTR_U	1 /* user priv level */
#define ARM_ATTR_HV	2 /* hypervisor priv level */	


#define _ARM_ATTR_K		(1 << ARM_ATTR_K)
#define _ARM_ATTR_U		(1 << ARM_ATTR_U)
#define _ARM_ATTR_HV	(1 << ARM_ATTR_HV)

#define ARM_ATTR_PLM_ALL (_ARM_ATTR_K|_ARM_ATTR_U|_ARM_ATTR_HV)

#define ARMV8_ATTRS	    (_ARM_ATTR_K|_ARM_ATTR_U|_ARM_ATTR_HV)





static const struct arm_event_entry arm_cortex_a57_pe[]={
	{.name = "SW_INCR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x00,
	 .evt_64bit = 0x00,
	 .desc = "Instruction architecturally executed (condition check pass) Software increment"	 
	},
	{.name = "L1I_CACHE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x01,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 instruction cache refill"
	},
	{.name = "L1I_TLB_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x02,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 instruction TLB refill"
	},
	{.name = "L1D_CACHE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x03,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache refill"
	},
	{.name = "L1D_CACHE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x04,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache access"
	},
	{.name = "L1D_TLB_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x05,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data TLB refill"
	},

	{.name = "INST_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x08,
	 .evt_64bit = 0x00,
	 .desc = "Instruction architecturally executed"
	},
	{.name = "EXCEPTION_TAKEN",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x09,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken"
	},
	{.name = "EXCEPTION_RETURN",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x0a,
	 .evt_64bit = 0x00,
	 .desc = "Instruction architecturally executed (condition check pass) Exception return"
	},
	{.name = "CID_WRITE_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x0b,
	 .evt_64bit = 0x00,
	 .desc = "Instruction architecturally executed (condition check pass)  Write to CONTEXTIDR"
	},

	{.name = "BRANCH_MISPRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x10,
	 .evt_64bit = 0x00,
	 .desc = "Mispredicted or not predicted branch speculatively executed"
	},
	{.name = "CPU_CYCLES",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x11,
	 .evt_64bit = 0x00,
	 .desc = "Cycles"
	},
	{.name = "BRANCH_PRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x12,
	 .evt_64bit = 0x00,
	 .desc = "Predictable branch speculatively executed"
	},
	{.name = "DATA_MEM_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x13,
	 .evt_64bit = 0x00,
	 .desc = "Data memory access"
	},
	{.name = "L1I_CACHE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x14,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 instruction cache access"
	},
	{.name = "L1D_CACHE_WB",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x15,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache WriteBack"
	},
	{.name = "L2D_CACHE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x16,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache access"
	},
	{.name = "L2D_CACHE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x17,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache refill"
	},
	{.name = "L2D_CACHE_WB",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x18,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache WriteBack"
	},
	{.name = "BUS_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x19,
	 .evt_64bit = 0x00,
	 .desc = "Bus access"
	},
	{.name = "LOCAL_MEMORY_ERROR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x1a,
	 .evt_64bit = 0x00,
	 .desc = "Local memory error"
	},
	{.name = "INST_SPEC_EXEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x1b,
	 .evt_64bit = 0x00,
	 .desc = "Instruction speculatively executed"
	},
	{.name = "TTBR_WRITE_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x1c,
	 .evt_64bit = 0x00,
	 .desc = "Instruction architecturally executed (condition check pass)  Write to translation table base"
	},
	{.name = "BUS_CYCLES",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x1d,
	 .evt_64bit = 0x00,
	 .desc = "Bus cycle"
	},
	{.name = "L1D_READ_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x40,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache read access"
	},
	{.name = "L1D_WRITE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x41,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache write access"
	},
	{.name = "L1D_READ_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x42,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache read refill"
	},
	{.name = "L1D_WRITE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x43,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache write refill"
	},
	{.name = "L1D_WB_VICTIM",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x46,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache writeback victim"
	},
	{.name = "L1D_WB_CLEAN_COHERENCY",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x47,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache writeback cleaning and coherency"
	},
	{.name = "L1D_INVALIDATE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x48,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data cache invalidate"
	},
	{.name = "L1D_TLB_READ_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4c,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data TLB read refill"
	},
	{.name = "L1D_TLB_WRITE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4d,
	 .evt_64bit = 0x00,
	 .desc = "Level 1 data TLB write refill"
	},
	{.name = "L2D_READ_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x50,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache read access"
	},
	{.name = "L2D_WRITE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x51,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache write access"
	},
	{.name = "L2D_READ_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x52,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache read refill"
	},
	{.name = "L2D_WRITE_REFILL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x53,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache write refill"
	},
	{.name = "L2D_WB_VICTIM",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x56,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache writeback victim"
	},
	{.name = "L2D_WB_CLEAN_COHERENCY",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x57,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache writeback cleaning and coherency"
	},
	{.name = "L2D_INVALIDATE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x58,
	 .evt_64bit = 0x00,
	 .desc = "Level 2 data cache invalidate"
	},
	{.name = "BUS_READ_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x60,
	 .evt_64bit = 0x00,
	 .desc = "Bus read access"
	},
	{.name = "BUS_WRITE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x61,
	 .evt_64bit = 0x00,
	 .desc = "Bus write access"
	},
	{.name = "BUS_NORMAL_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .evt_64bit = 0x00,
	 .code = 0x62,
	 .desc = "Bus normal access"
	},
	{.name = "BUS_NOT_NORMAL_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x63,
	 .evt_64bit = 0x00,
	 .desc = "Bus not normal access"
	},
	{.name = "BUS_NORMAL_ACCESS_2",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x64,
	 .evt_64bit = 0x00,
	 .desc = "Bus normal access"
	},
	{.name = "BUS_PERIPH_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x65,
	 .evt_64bit = 0x00,
	 .desc = "Bus peripheral access"
	},
	{.name = "DATA_MEM_READ_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x66,
	 .evt_64bit = 0x00,
	 .desc = "Data memory read access"
	},
	{.name = "DATA_MEM_WRITE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x67,
	 .evt_64bit = 0x00,
	 .desc = "Data memory write access"
	},
	{.name = "UNALIGNED_READ_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x68,
	 .evt_64bit = 0x00,
	 .desc = "Unaligned read access"
	},
	{.name = "UNALIGNED_WRITE_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x69,
	 .evt_64bit = 0x00,
	 .desc = "Unaligned read access"
	},
	{.name = "UNALIGNED_ACCESS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6a,
	 .evt_64bit = 0x00,
	 .desc = "Unaligned access"
	},
	{.name = "INST_SPEC_EXEC_LDREX",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6c,
	 .evt_64bit = 0x00,
	 .desc = "LDREX exclusive instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_STREX_PASS",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6d,
	 .evt_64bit = 0x00,
	 .desc = "STREX pass exclusive instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_STREX_FAIL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6e,
	 .evt_64bit = 0x00,
	 .desc = "STREX fail exclusive instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_LOAD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x70,
	 .evt_64bit = 0x00,
	 .desc = "Load instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_STORE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x71,
	 .evt_64bit = 0x00,
	 .desc = "Store instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_LOAD_STORE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x72,
	 .evt_64bit = 0x00,
	 .desc = "Load or store instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_INTEGER_INST",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x73,
	 .evt_64bit = 0x00,
	 .desc = "Integer data processing instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_SIMD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x74,
	 .evt_64bit = 0x00,
	 .desc = "Advanced SIMD instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_VFP",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x75,
	 .evt_64bit = 0x00,
	 .desc = "VFP instruction speculatively executed"
	},
	{.name = "INST_SPEC_EXEC_SOFT_PC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x76,
	 .evt_64bit = 0x00,
	 .desc = "Software of the PC instruction speculatively executed"
	},
	{.name = "BRANCH_SPEC_EXEC_IMM_BRANCH",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x78,
	 .evt_64bit = 0x00,
	 .desc = "Immediate branch speculatively executed"
	},
	{.name = "BRANCH_SPEC_EXEC_RET",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x79,
	 .evt_64bit = 0x00,
	 .desc = "Return branch speculatively executed"
	},
	{.name = "BRANCH_SPEC_EXEC_IND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7a,
	 .evt_64bit = 0x00,
	 .desc = "Indirect branch speculatively executed"
	},
	{.name = "BARRIER_SPEC_EXEC_ISB",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7c,
	 .evt_64bit = 0x00,
	 .desc = "ISB barrier speculatively executed"
	},
	{.name = "BARRIER_SPEC_EXEC_DSB",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7d,
	 .evt_64bit = 0x00,
	 .desc = "DSB barrier speculatively executed"
	},
	{.name = "BARRIER_SPEC_EXEC_DMB",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7e,
	 .evt_64bit = 0x00,
	 .desc = "DMB barrier speculatively executed"
	},
	{.name = "EXCEPTION_UNDEF",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x81,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, other synchronous"
	},
	{.name = "EXCEPTION_SVC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x82,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, supervisor call"
	},
	{.name = "EXCEPTION_PABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x83,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, instruction abort"
	},
	{.name = "EXCEPTION_DABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x84,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, data abort or SError"
	},
	{.name = "EXCEPTION_IRQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x86,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, irq"
	},
	{.name = "EXCEPTION_FIQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x87,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, irq"
	},
	{.name = "EXCEPTION_SMC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x88,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, secure monitor call"
	},
	{.name = "EXCEPTION_HVC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8a,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, hypervisor call"
	},
	{.name = "EXCEPTION_TRAP_PABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8b,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, instruction abort not taken locally"
	},
	{.name = "EXCEPTION_TRAP_DABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8c,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, data abort or SError not taken locally"
	},
	{.name = "EXCEPTION_TRAP_OTHER",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8d,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, other traps not taken locally"
	},
	{.name = "EXCEPTION_TRAP_IRQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8e,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, irq not taken locally"
	},
	{.name = "EXCEPTION_TRAP_FIQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8f,
	 .evt_64bit = 0x00,
	 .desc = "Exception taken, fiq not taken locally"
	},
	{.name = "RC_LD_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x90,
	 .evt_64bit = 0x00,
	 .desc = "Release consistency instruction speculatively executed (load-acquire)",
	},
	{.name = "RC_ST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x91,
	 .evt_64bit = 0x00,
	 .desc = "Release consistency instruction speculatively executed (store-release)",
	},
	/* END Cortex A47 specific events */
};
#endif /*__ARM_A57_EVENTS_H*/