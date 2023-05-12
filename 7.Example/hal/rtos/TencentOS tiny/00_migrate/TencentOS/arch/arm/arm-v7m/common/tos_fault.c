/*----------------------------------------------------------------------------
 * Tencent is pleased to support the open source community by making TencentOS
 * available.
 *
 * Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
 * If you have downloaded a copy of the TencentOS binary from Tencent, please
 * note that the TencentOS binary is licensed under the BSD 3-Clause License.
 *
 * If you have downloaded a copy of the TencentOS source code from Tencent,
 * please note that TencentOS source code is licensed under the BSD 3-Clause
 * License, except for the third-party components listed below which are
 * subject to different license terms. Your integration of TencentOS into your
 * own projects may require compliance with the BSD 3-Clause License, as well
 * as the other licenses applicable to the third-party components included
 * within TencentOS.
 *---------------------------------------------------------------------------*/

#include "tos_k.h"

#if TOS_CFG_FAULT_BACKTRACE_EN > 0u

__STATIC_INLINE__ void fault_spin(void)
{
    tos_knl_sched_lock();
    tos_cpu_int_disable();
    while (K_TRUE) {
        ;
    }
}

/* EXC_RETURN:
   31 - 28 : EXC_RETURN flag
   27 -  5 : reserved
   4       : 1, basic stack frame; 0, extended stack frame
   3       : 1, return to Thread mode; 0, return to Handler mode
   2       : 1, return to PSP; 0, return to MSP
   1       : reserved, 0
   0       : reserved, 1
 */
__STATIC_INLINE__ int fault_is_on_task(cpu_data_t lr)
{
    return (lr & (1u << 2)) != 0;
}

__STATIC_INLINE__ int fault_is_thumb(cpu_data_t psr)
{
    return (psr & (1u << 24)) != 0;
}

__STATIC_INLINE__ int fault_is_code(fault_info_t *info, cpu_data_t value)
{
    return value >= info->code_start && value <= info->code_limit;
}

#if defined (TOS_CFG_CPU_ARM_FPU_EN) && (TOS_CFG_CPU_ARM_FPU_EN == 1U)
__STATIC_INLINE__ int fault_is_extended_stack_frame(cpu_data_t lr)
{
    return (lr & (1u << 4)) == 0;
}

__STATIC__ void fault_dump_fpu_frame(fault_fpu_frame_t *fpu_frame)
{
    /*
     * As known, v7-m has a feature named "LAZY PUSH", for the reason we do not do any float
     * operation in fault_backtrace, cpu will not do the real fpu register push to the stack.
     * that means the value we dump in fault_dump_fpu_frame will not be the correct value of
     * each FPU register.
     * We define a function here which access to FPSCR, if this function involved, cpu will do
     * the real FPU register push so we will get the correct dump.
     * I know it's ugly, but it works. If you know a better way, please tell me.
     */
    cpu_flush_fpu();

    k_fault_log_writer("\n\n====================== FPU Registers =======================\n");
    k_fault_log_writer("  %s: %08x\n", "FPSCR", fpu_frame->fpscr);
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                            "S0 ", fpu_frame->s0,
                                            "S1 ", fpu_frame->s1,
                                            "S2 ", fpu_frame->s2,
                                            "S3 ", fpu_frame->s3);
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                            "S4 ", fpu_frame->s4,
                                            "S5 ", fpu_frame->s5,
                                            "S6 ", fpu_frame->s6,
                                            "S7 ", fpu_frame->s7);
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                            "S8 ", fpu_frame->s8,
                                            "S9 ", fpu_frame->s9,
                                            "S10", fpu_frame->s10,
                                            "S11", fpu_frame->s11);
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                            "S12", fpu_frame->s12,
                                            "S13", fpu_frame->s13,
                                            "S14", fpu_frame->s14,
                                            "S15", fpu_frame->s15);
}

#endif

__STATIC__ void fault_dump_cpu_frame(fault_cpu_frame_t *cpu_frame)
{
    k_fault_log_writer("\n\n====================== CPU Registers =======================\n");
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                                        "R0 ", cpu_frame->r0,
                                                        "R1 ", cpu_frame->r1,
                                                        "R2 ", cpu_frame->r2,
                                                        "R3 ", cpu_frame->r3);
    k_fault_log_writer("  %s: %08x  %s: %08x  %s: %08x  %s: %08x\n",
                                                        "R12", cpu_frame->r12,
                                                        "LR ", cpu_frame->lr,
                                                        "PC ", cpu_frame->pc,
                                                        "PSR", cpu_frame->spsr);
}

__STATIC__ void fault_dump_stack(fault_info_t *info, size_t depth)
{
    cpu_addr_t sp = info->sp_before_fault;

    k_fault_log_writer("\nTASK STACK DUMP:\n");
    while (sp <= info->stack_limit && depth--) {
        k_fault_log_writer("  addr: %08x  data: %08x\n", sp, (cpu_data_t)*(cpu_data_t *)sp);
        sp += sizeof(cpu_addr_t);
    }
}

__STATIC__ void fault_call_stack_backtrace(fault_info_t *info, size_t depth)
{
    cpu_data_t value;
    cpu_addr_t sp = info->sp_before_fault;

    if (info->is_stk_ovrf) {
        return;
    }

    k_fault_log_writer("\n\n====================== Dump Call Stack =====================\n");

    k_fault_log_writer("%08x", info->pc);

    /* walk through the stack, check every content on stack whether is a instruction(code) */
    for (; sp < info->stack_limit && depth; sp += sizeof(cpu_addr_t)) {
        value = *((cpu_addr_t *)sp) - sizeof(cpu_addr_t);

        /* if thumb, a instruction's first bit must be 1 */
        if (info->is_thumb && !(value & 1)) {
            continue;
        }

        if (fault_is_code(info, value)) {
            k_fault_log_writer(" %08x\n", value);
            --depth;
        }
    }
    
    k_fault_log_writer("\nusage: addr2line -e <*.axf> -a -f <dump call stack>");
}

__STATIC__ void fault_dump_task(fault_info_t *info)
{
    k_task_t *task;

    if (!info->is_on_task) {
        return;
    }

    task = k_curr_task;
    k_fault_log_writer("\n\n====================== Fault on task =======================\n");
    k_fault_log_writer("  TASK NAME:  %s\n", task->name);
    k_fault_log_writer("  STK BASE:   %x\n", info->stack_start);
    k_fault_log_writer("  STK SIZE:   %x\n", task->stk_size * sizeof(k_stack_t));
    k_fault_log_writer("  STK LIMIT:  %x\n", info->stack_limit);

    if (!info->is_stk_ovrf) {
        fault_dump_stack(info, K_FAULT_STACK_DUMP_DEPTH);
    }
}

__STATIC__ void fault_dump_information(fault_info_t *info)
{
    k_fault_log_writer("\n\n================== Dump Fault Information ==================\n");
    k_fault_log_writer("  THUMB:     %s\n", info->is_thumb ? "TRUE" : "FALSE");
    k_fault_log_writer("  ON TASK:   %s\n", info->is_on_task? "TRUE" : "FALSE");
    k_fault_log_writer("  STK OVRF:  %s\n", info->is_stk_ovrf? "TRUE" : "FALSE");

#if defined (TOS_CFG_CPU_ARM_FPU_EN) && (TOS_CFG_CPU_ARM_FPU_EN == 1U)
    k_fault_log_writer("  EXT STK:   %s\n", info->is_ext_stk_frm? "TRUE" : "FALSE");
#endif

    k_fault_log_writer("  PC:        %08x\n", info->pc);
    k_fault_log_writer("  SP:        %08x\n", info->sp_before_fault);
    k_fault_log_writer("  STK START: %08x\n", info->stack_start);
    k_fault_log_writer("  STK LIMIT: %08x\n", info->stack_limit);
    k_fault_log_writer("  COD START: %08x\n", info->code_start);
    k_fault_log_writer("  COD LIMIT: %08x\n", info->code_limit);
}

__STATIC__ void fault_gather_information(cpu_data_t lr, fault_exc_frame_t *frame, fault_info_t *info)
{
    info->is_thumb = fault_is_thumb(frame->cpu_frame.spsr);
    info->is_on_task = fault_is_on_task(lr);

    info->pc = frame->cpu_frame.pc;

    info->sp_before_fault = (cpu_addr_t)frame + sizeof(fault_cpu_frame_t);

#if defined (TOS_CFG_CPU_ARM_FPU_EN) && (TOS_CFG_CPU_ARM_FPU_EN == 1U)
    info->is_ext_stk_frm = fault_is_extended_stack_frame(lr);

    if (info->is_ext_stk_frm) {
        info->sp_before_fault += sizeof(fault_fpu_frame_t);
    }
#endif

    info->code_start = fault_code_start();
    info->code_limit = fault_code_limit();

    if (info->is_on_task) {
        info->stack_start = (cpu_addr_t)k_curr_task->stk_base;
        info->stack_limit = info->stack_start + k_curr_task->stk_size;
    } else {
        info->stack_start = fault_msp_start();
        info->stack_limit = fault_msp_limit();
    }

    info->is_stk_ovrf = (info->sp_before_fault < info->stack_start || info->sp_before_fault > info->stack_limit);
}

__KNL__ int fault_default_log_writer(const char *format, ...)
{
    int len;
    va_list ap;

    va_start(ap, format);
    len = vprintf(format, ap);
    va_end(ap);

    return len;
}

__API__ void tos_fault_log_writer_set(k_fault_log_writer_t log_writer)
{
    k_fault_log_writer = log_writer;
}

__KNL__ void fault_backtrace(cpu_addr_t lr, fault_exc_frame_t *frame)
{
    fault_info_t info;

    fault_gather_information(lr, frame, &info);

    fault_dump_information(&info);

    fault_dump_task(&info);

    fault_dump_cpu_frame(&frame->cpu_frame);

#if defined (TOS_CFG_CPU_ARM_FPU_EN) && (TOS_CFG_CPU_ARM_FPU_EN == 1U)
    if (info.is_ext_stk_frm) {
        fault_dump_fpu_frame(&frame->fpu_frame);
    }
#endif

    fault_call_stack_backtrace(&info, K_FAULT_CALL_STACK_BACKTRACE_DEPTH);

    cpu_fault_diagnosis();

    fault_spin();
}

#endif

