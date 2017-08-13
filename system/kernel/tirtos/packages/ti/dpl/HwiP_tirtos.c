/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== HwiP_tirtos.c ========
 */

#include <ti/drivers/dpl/HwiP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#ifdef xdc_target__isaCompatible_28
#include <ti/sysbios/hal/Hwi.h>
#else
#define ti_sysbios_family_arm_m3_Hwi__internalaccess
#include <ti/sysbios/family/arm/m3/Hwi.h>
#endif
//todo msp430

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int interruptNum)
{
    Hwi_clearInterrupt(interruptNum);
}

/*
 *  ======== HwiP_construct ========
 */
HwiP_Handle HwiP_construct(HwiP_Struct *hwiP, int interruptNum,
        HwiP_Fxn hwiFxn, HwiP_Params *params)
{
    Hwi_Handle  hwi;
    Hwi_Params  hwiParams;
    Error_Block eb;

    /*
     *  Hwi_construct() can fail, but the only way to determine this
     *  is with an Error_Block.
     */
    Error_init(&eb);

    if (params == NULL) {
        Hwi_construct((Hwi_Struct *)hwiP, interruptNum,
                (Hwi_FuncPtr)hwiFxn, NULL, &eb);
    }
    else {
        Hwi_Params_init(&hwiParams);
        hwiParams.instance->name = params->name;
        hwiParams.arg            = (xdc_UArg)(params->arg);
        hwiParams.priority       = (int)params->priority;
        Hwi_construct((Hwi_Struct *)hwiP, interruptNum, (Hwi_FuncPtr)hwiFxn,
                      &hwiParams, &eb);
    }

    if (Error_check(&eb)) {
        return (NULL);
    }

    hwi = Hwi_handle((Hwi_Struct *)hwiP);

    return ((HwiP_Handle)hwi);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn,
                        HwiP_Params *params)
{
    Hwi_Handle  handle;
    Hwi_Params  hwiParams;
    Error_Block eb;

    /*
     *  Hwi_create() can fail, but the only way to determine this
     *  is with an Error_Block.
     */
    Error_init(&eb);

    if (params == NULL) {
        handle = Hwi_create(interruptNum, (Hwi_FuncPtr)hwiFxn, NULL, &eb);
    }
    else {
        Hwi_Params_init(&hwiParams);
        hwiParams.instance->name = params->name;
        hwiParams.arg            = (xdc_UArg)(params->arg);
        hwiParams.priority       = (int)params->priority;
        handle = Hwi_create(interruptNum, (Hwi_FuncPtr)hwiFxn,
                      &hwiParams, &eb);
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{
    Hwi_Handle hwi = (Hwi_Handle)handle;

    Hwi_delete(&hwi);

    return (HwiP_OK);
}

/*
 *  ======== HwiP_destruct ========
 */
void HwiP_destruct(HwiP_Struct *hwiP)
{
    Hwi_destruct((Hwi_Struct *)hwiP);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key;

    key = Hwi_disable();

    return (key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int interruptNum)
{
    Hwi_disableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int interruptNum)
{
    Hwi_enableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    params->name = NULL;
    params->arg = 0;
    params->priority = ~0;
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    Hwi_restore(key);
}

/*
 *  ======== HwiP_staticObjectSize ========
 */
size_t HwiP_staticObjectSize(void)
{
    return (sizeof(Hwi_Struct));
}
