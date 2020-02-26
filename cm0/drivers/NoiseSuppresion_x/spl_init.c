/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/* The global function contained in this file initializes SPL function
 * pointers, currently only for ARM platforms.
 *
 * Some code came from common/rtcd.c in the WebM project.
 */

#include "real_fft.h"
#include "signal_processing_library.h"
#include "cpu_features_wrapper.h"

/* Declare function pointers. */
MaxAbsValueW16 WebRtcSpl_MaxAbsValueW16;
MaxAbsValueW32 WebRtcSpl_MaxAbsValueW32;
MaxValueW16 WebRtcSpl_MaxValueW16;
MaxValueW32 WebRtcSpl_MaxValueW32;
MinValueW16 WebRtcSpl_MinValueW16;
MinValueW32 WebRtcSpl_MinValueW32;
CrossCorrelation WebRtcSpl_CrossCorrelation;
DownsampleFast WebRtcSpl_DownsampleFast;
ScaleAndAddVectorsWithRound WebRtcSpl_ScaleAndAddVectorsWithRound;
CreateRealFFT WebRtcSpl_CreateRealFFT;
FreeRealFFT WebRtcSpl_FreeRealFFT;
RealForwardFFT WebRtcSpl_RealForwardFFT;
RealInverseFFT WebRtcSpl_RealInverseFFT;


static void InitPointersToC() {
  WebRtcSpl_MaxAbsValueW16 = WebRtcSpl_MaxAbsValueW16C;
  WebRtcSpl_MaxAbsValueW32 = WebRtcSpl_MaxAbsValueW32C;
  WebRtcSpl_MaxValueW16 = WebRtcSpl_MaxValueW16C;
  WebRtcSpl_MaxValueW32 = WebRtcSpl_MaxValueW32C;
  WebRtcSpl_MinValueW16 = WebRtcSpl_MinValueW16C;
  WebRtcSpl_MinValueW32 = WebRtcSpl_MinValueW32C;
  WebRtcSpl_CrossCorrelation = WebRtcSpl_CrossCorrelationC;
  WebRtcSpl_DownsampleFast = WebRtcSpl_DownsampleFastC;
  WebRtcSpl_ScaleAndAddVectorsWithRound =
      WebRtcSpl_ScaleAndAddVectorsWithRoundC;
  WebRtcSpl_CreateRealFFT = WebRtcSpl_CreateRealFFTC;
  WebRtcSpl_FreeRealFFT = WebRtcSpl_FreeRealFFTC;
  WebRtcSpl_RealForwardFFT = WebRtcSpl_RealForwardFFTC;
  WebRtcSpl_RealInverseFFT = WebRtcSpl_RealInverseFFTC;
}

static void InitFunctionPointers(void) {
  InitPointersToC();
}


void WebRtcSpl_Init() {
  //once(InitFunctionPointers);
  InitFunctionPointers();
}
