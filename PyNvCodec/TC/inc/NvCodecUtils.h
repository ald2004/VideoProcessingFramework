 /*
 * Copyright 2019 NVIDIA Corporation
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *    http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <assert.h>
#include <chrono>
#include <cuda_runtime.h>
#include <iomanip>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <thread>

#ifndef _WIN32
#define _stricmp strcasecmp
#endif

void ResizeNv12(unsigned char *dpDstNv12, int nDstPitch, int nDstWidth,
                int nDstHeight, unsigned char *dpSrcNv12, int nSrcPitch,
                int nSrcWidth, int nSrcHeight,
                unsigned char *dpDstNv12UV = nullptr, cudaStream_t S = 0);
void xxxkernel(const uint8_t* input, size_t sPitch, float* output, size_t dPitch, int nSrcWidth, int nSrcHeight, cudaStream_t S,float did);