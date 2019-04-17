/**
 * @file op_x86_model.h 
 * interface to op_arm model-specific operations
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
 * @author Diego Alejandro Parra Guzman
 */
/******************************************************************************
 * @file op_arm64_model.h -> used for xenoprof 
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



#ifndef OP_ARM64_MODEL_H
#define OP_ARM64_MODEL_H

#include <asm/arm64/arm_pmu_events.h>

struct op_perf_ctr 
{
	struct arm_event_entry event;
	u64 flags;
	u64 val;
};


struct op_pmu
{
	struct op_perf_ctr * counters;
};


/*
 * xenoprof specifict models
 */
struct op_arm_model_spec 
{
	unsigned int num_counters;
	unsigned int num_events;
	void (*start)(struct op_perf_ctr *);
	void (*stop) (struct op_perf_ctr *);
	void (*enable_user)(void);
	void (*setup_counters)(struct op_perf_ctr *);
	void (*load_counters)(struct op_perf_ctr *);	
	void (*save_counters)(struct op_perf_ctr *);

};


extern struct op_arm_model_spec const op_arm_cortex_a57;
extern struct op_arm_model_spec __read_mostly const * model;







#endif /* OP_ARM64_MODEL_H */

