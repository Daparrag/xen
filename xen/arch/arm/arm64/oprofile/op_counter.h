/******************************************************************************
 * @file op_counter.h
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * @author diego alejandro parra guzman <daparrag@correo.udistrital.edu.co>
 */
 
#ifndef OP_COUNTER_H
#define OP_COUNTER_H

#include <asm/arm64/arm_pmu_events.h>

#define OP_MAX_COUNTER 8


struct op_counter_config {
        unsigned long count;
        unsigned long enabled;
        unsigned long evt_code;
        unsigned long kernel;
        unsigned long user;
        unsigned long hyp;
        unsigned long unit_mask;
};

extern struct op_counter_config counter_config[];

#endif /* OP_COUNTER_H */