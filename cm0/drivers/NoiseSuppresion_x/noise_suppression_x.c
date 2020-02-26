/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "noise_suppression_x.h"

#include <stdlib.h>

#include "real_fft.h"
#include "nsx_core.h"
#include "nsx_defines.h"

int WebRtcNsx_Create(NsxHandle* nsxInst) {

    WebRtcSpl_Init();
    nsxInst->real_fft = NULL;
    nsxInst->initFlag = 0;
    return 0;
  

}

int WebRtcNsx_Free(NsxHandle* nsxInst) {
  //WebRtcSpl_FreeRealFFT(((NsxInst_t*)nsxInst)->real_fft);
  //free(nsxInst);
  return 0;
}

int WebRtcNsx_Init(NsxHandle* nsxInst, uint32_t fs) {
  return WebRtcNsx_InitCore((NsxInst_t*)nsxInst, fs);
}

int WebRtcNsx_set_policy(NsxHandle* nsxInst, int mode) {
  return WebRtcNsx_set_policy_core((NsxInst_t*)nsxInst, mode);
}

int WebRtcNsx_Process(NsxHandle* nsxInst, short* speechFrame,
                      short* speechFrameHB, short* outFrame,
                      short* outFrameHB) {
  return WebRtcNsx_ProcessCore(
      (NsxInst_t*)nsxInst, speechFrame, speechFrameHB, outFrame, outFrameHB);
}
