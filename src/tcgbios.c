//  Implementation of the TCG BIOS extension according to the specification
//  described in
//  https://www.trustedcomputinggroup.org/specs/PCClient/TCG_PCClientImplementationforBIOS_1-20_1-00.pdf
//
//  Copyright (C) 2006-2013 IBM Corporation
//
//  Authors:
//      Stefan Berger <stefanb at linux.vnet.ibm.com>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.


#include "config.h"

#include "types.h"
#include "tpm_drivers.h" // tpm_drivers[]


typedef struct {
    u8            tpm_probed:1;
    u8            tpm_found:1;
    u8            tpm_working:1;
    u8            if_shutdown:1;
    u8            tpm_driver_to_use:4;
} tcpa_state_t;


static tcpa_state_t tcpa_state = {
    .tpm_driver_to_use = TPM_INVALID_DRIVER,
};


/********************************************************
  Extensions for TCG-enabled BIOS
 *******************************************************/


static u32
is_tpm_present(void)
{
    u32 rc = 0;
    unsigned int i;

    for (i = 0; i < TPM_NUM_DRIVERS; i++) {
        struct tpm_driver *td = &tpm_drivers[i];
        if (td->probe() != 0) {
            td->init();
            tcpa_state.tpm_driver_to_use = i;
            rc = 1;
            break;
        }
    }

    return rc;
}


int
has_working_tpm(void)
{
    if (!tcpa_state.tpm_probed) {
        tcpa_state.tpm_probed = 1;
        tcpa_state.tpm_found = (is_tpm_present() != 0);
        tcpa_state.tpm_working = 1;
    }
    if (!tcpa_state.tpm_working)
        return 0;

    return tcpa_state.tpm_found;
}
