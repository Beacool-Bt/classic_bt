/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains platform-specific typedefs and defines.
// Much of it is derived from Chromium's build/build_config.h.

#ifndef WEBRTC_TYPEDEFS_H_
#define WEBRTC_TYPEDEFS_H_

// For access to standard POSIXish features, use WEBRTC_POSIX instead of a
// more specific macro.
//#if defined(WEBRTC_MAC) || defined(WEBRTC_LINUX) || \
//    defined(WEBRTC_ANDROID)
#define WEBRTC_POSIX
//#endif

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN

#include <stdint.h>

#endif  // WEBRTC_TYPEDEFS_H_
