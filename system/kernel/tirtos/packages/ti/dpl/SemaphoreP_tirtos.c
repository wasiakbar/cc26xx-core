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
 *  ======== SemaphoreP_tirtos.c ========
 */
#include <ti/drivers/dpl/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Semaphore.h>


/*
 *  ======== SemaphoreP_construct ========
 */
SemaphoreP_Handle SemaphoreP_construct(SemaphoreP_Struct *handle,
        unsigned int count, SemaphoreP_Params *params)
{
    Semaphore_Params semParams;
    Semaphore_Struct *semStruct = (Semaphore_Struct *)handle;

    if (params == NULL) {
        Semaphore_construct(semStruct, count, NULL);
    }
    else {
        Semaphore_Params_init(&semParams);
        /*
         * The default mode for TI-RTOS is counting, so only change if a binary
         * semaphore is requested.
         */
        if (params->mode == SemaphoreP_Mode_BINARY) {
            semParams.mode = Semaphore_Mode_BINARY;
        }
        semParams.instance->name = params->name;
        Semaphore_construct(semStruct, count, &semParams);
    }

    return ((SemaphoreP_Handle)(Semaphore_handle(semStruct)));
}

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(unsigned int count,
                                    SemaphoreP_Params *params)
{
    Semaphore_Handle  handle;
    Semaphore_Params  semaphoreParams;

    if (params == NULL) {
        handle = Semaphore_create(count, NULL, Error_IGNORE);
    }
    else {
        Semaphore_Params_init(&semaphoreParams);
        /*
         * The default mode for TI-RTOS is counting, so only change if a binary
         * semaphore is requested.
         */
        if (params->mode == SemaphoreP_Mode_BINARY) {
            semaphoreParams.mode = Semaphore_Mode_BINARY;
        }
        semaphoreParams.instance->name = params->name;
        handle = Semaphore_create(count, &semaphoreParams, Error_IGNORE);
    }

    return ((SemaphoreP_Handle)handle);
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    Semaphore_Handle semaphore = (Semaphore_Handle)handle;

    Semaphore_delete(&semaphore);

    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_destruct ========
 */
void SemaphoreP_destruct(SemaphoreP_Struct *semP)
{
    Semaphore_destruct((Semaphore_Struct *)semP);
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init(SemaphoreP_Params *params)
{
    params->mode = SemaphoreP_Mode_COUNTING;
    params->name = NULL;
    params->callback = NULL;
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    Bool flag;

    flag = Semaphore_pend((Semaphore_Handle)handle, timeout);
    if (FALSE == flag) {
        return (SemaphoreP_TIMEOUT);
    }

    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    Semaphore_post((Semaphore_Handle)handle);
    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_postFromClock ========
 */
SemaphoreP_Status SemaphoreP_postFromClock(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/*
 *  ======== SemaphoreP_staticObjectSize ========
 */
size_t SemaphoreP_staticObjectSize(void)
{
    return (sizeof(Semaphore_Struct));
}
