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
#include "CodecsSupport.hpp"
#include "MemoryInterfaces.hpp"
#include "NvCodecCLIOptions.h"
#include "TC_CORE.hpp"
#include "cuviddec.h"

extern "C" {
  #include <libavutil/frame.h>
}

using namespace VPF;

// VPF stands for Video Processing Framework;
namespace VPF {
class DllExport NvencEncodeFrame final : public Task {
public:
  NvencEncodeFrame() = delete;
  NvencEncodeFrame(const NvencEncodeFrame &other) = delete;
  NvencEncodeFrame &operator=(const NvencEncodeFrame &other) = delete;

  TaskExecStatus Execute() final;
  ~NvencEncodeFrame() final;
  static NvencEncodeFrame *Make(CUstream cuStream, CUcontext cuContext,
                                NvEncoderClInterface &cli_iface,
                                NV_ENC_BUFFER_FORMAT format, uint32_t width,
                                uint32_t height, bool verbose);

  bool Reconfigure(NvEncoderClInterface &cli_iface, bool force_idr,
                   bool reset_enc, bool verbose);

private:
  NvencEncodeFrame(CUstream cuStream, CUcontext cuContext,
                   NvEncoderClInterface &cli_iface, NV_ENC_BUFFER_FORMAT format,
                   uint32_t width, uint32_t height, bool verbose);
  static const uint32_t numInputs = 3U;
  static const uint32_t numOutputs = 1U;
  struct NvencEncodeFrame_Impl *pImpl = nullptr;
};

class DllExport NvdecDecodeFrame final : public Task {
public:
  NvdecDecodeFrame() = delete;
  NvdecDecodeFrame(const NvdecDecodeFrame &other) = delete;
  NvdecDecodeFrame &operator=(const NvdecDecodeFrame &other) = delete;

  void GetDecodedFrameParams(uint32_t &width, uint32_t &height,
                             uint32_t &elemSize);
  TaskExecStatus Execute() final;
  uint32_t GetDeviceFramePitch();
  ~NvdecDecodeFrame() final;
  static NvdecDecodeFrame *Make(CUstream cuStream, CUcontext cuContext,
                                cudaVideoCodec videoCodec,
                                uint32_t decodedFramesPoolSize,
                                uint32_t coded_width, uint32_t coded_height,
                                Pixel_Format format);

private:
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 1U;
  struct NvdecDecodeFrame_Impl *pImpl = nullptr;

  NvdecDecodeFrame(CUstream cuStream, CUcontext cuContext,
                   cudaVideoCodec videoCodec, uint32_t decodedFramesPoolSize,
                   uint32_t coded_width, uint32_t coded_height,
                   Pixel_Format format);
};

class DllExport FfmpegDecodeFrame final : public Task {
public:
  FfmpegDecodeFrame() = delete;
  FfmpegDecodeFrame(const FfmpegDecodeFrame &other) = delete;
  FfmpegDecodeFrame &operator=(const FfmpegDecodeFrame &other) = delete;

  TaskExecStatus Execute() final;
  TaskExecStatus GetSideData(AVFrameSideDataType);

  ~FfmpegDecodeFrame() final;
  static FfmpegDecodeFrame *Make(const char *URL,
                                 NvDecoderClInterface &cli_iface);

private:
  static const uint32_t num_inputs = 0U;
  // Reconstructed pixels + side data;
  static const uint32_t num_outputs = 2U;
  struct FfmpegDecodeFrame_Impl *pImpl = nullptr;

  FfmpegDecodeFrame(const char *URL, NvDecoderClInterface &cli_iface);
};

class DllExport CudaUploadFrame final : public Task {
public:
  CudaUploadFrame() = delete;
  CudaUploadFrame(const CudaUploadFrame &other) = delete;
  CudaUploadFrame &operator=(const CudaUploadFrame &other) = delete;

  TaskExecStatus Execute() final;
  size_t GetUploadSize() const;
  ~CudaUploadFrame() final;
  static CudaUploadFrame *Make(CUstream cuStream, CUcontext cuContext,
                               uint32_t width, uint32_t height,
                               Pixel_Format pixelFormat);

private:
  CudaUploadFrame(CUstream cuStream, CUcontext cuContext, uint32_t width,
                  uint32_t height, Pixel_Format pixelFormat);
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 1U;
  struct CudaUploadFrame_Impl *pImpl = nullptr;
};

class DllExport CudaDownloadSurface final : public Task {
public:
  CudaDownloadSurface() = delete;
  CudaDownloadSurface(const CudaDownloadSurface &other) = delete;
  CudaDownloadSurface &operator=(const CudaDownloadSurface &other) = delete;

  ~CudaDownloadSurface() final;
  TaskExecStatus Execute() final;
  static CudaDownloadSurface *Make(CUstream cuStream, CUcontext cuContext,
                                   uint32_t width, uint32_t height,
                                   Pixel_Format pixelFormat);

private:
  CudaDownloadSurface(CUstream cuStream, CUcontext cuContext, uint32_t width,
                      uint32_t height, Pixel_Format pixelFormat);
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 1U;
  struct CudaDownloadSurface_Impl *pImpl = nullptr;
};

class DllExport DemuxFrame final : public Task {
public:
  DemuxFrame() = delete;
  DemuxFrame(const DemuxFrame &other) = delete;
  DemuxFrame &operator=(const DemuxFrame &other) = delete;

  void GetParams(struct MuxingParams &params) const;
  TaskExecStatus Execute() final;
  ~DemuxFrame() final;
  static DemuxFrame *Make(const char *url, const char **ffmpeg_options,
                          uint32_t opts_size);

private:
  DemuxFrame(const char *url, const char **ffmpeg_options, uint32_t opts_size);
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 3U;
  struct DemuxFrame_Impl *pImpl = nullptr;
};

class DllExport MuxFrame final : public Task {
public:
  MuxFrame() = delete;
  MuxFrame(const MuxFrame &other) = delete;
  MuxFrame &operator=(const MuxFrame &other) = delete;

  TaskExecStatus Execute() final;
  ~MuxFrame() final;
  static MuxFrame *Make(const char *url);

private:
  MuxFrame(const char *url);
  static const uint32_t numInputs = 2U;
  static const uint32_t numOutputs = 0U;
  struct MuxFrame_Impl *pImpl = nullptr;
  char *output = nullptr;
};

class DllExport ConvertSurface final : public Task {
public:
  ConvertSurface() = delete;
  ConvertSurface(const ConvertSurface &other) = delete;
  ConvertSurface &operator=(const ConvertSurface &other) = delete;

  static ConvertSurface *Make(uint32_t width, uint32_t height,
                              Pixel_Format inFormat, Pixel_Format outFormat,
                              CUcontext ctx, CUstream str);

  ~ConvertSurface();

  TaskExecStatus Execute() final;

private:
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 1U;

  struct NppConvertSurface_Impl *pImpl;

  ConvertSurface(uint32_t width, uint32_t height, Pixel_Format inFormat,
                 Pixel_Format outFormat, CUcontext ctx, CUstream str);
};

class DllExport ResizeSurface final : public Task {
public:
  ResizeSurface() = delete;
  ResizeSurface(const ResizeSurface &other) = delete;
  ResizeSurface &operator=(const ResizeSurface &other) = delete;

  static ResizeSurface *Make(uint32_t width, uint32_t height,
                             Pixel_Format format, CUcontext ctx, CUstream str);

  ~ResizeSurface();

  TaskExecStatus Execute() final;

private:
  static const uint32_t numInputs = 1U;
  static const uint32_t numOutputs = 1U;

  struct ResizeSurface_Impl *pImpl;
  ResizeSurface(uint32_t width, uint32_t height, Pixel_Format format,
                CUcontext ctx, CUstream str);
};

class DllExport NormalizeSurface final: public Task{
    public:
        NormalizeSurface() = delete;
        NormalizeSurface(const NormalizeSurface& other) = delete;
        NormalizeSurface& operator=(const NormalizeSurface& other) = delete;

        static NormalizeSurface* Make(uint32_t width, uint32_t height,
            float divisor, CUcontext ctx, CUstream str,Pixel_Format format);

        ~NormalizeSurface();

        TaskExecStatus Execute() final;
    private:
        static const uint32_t numInputs = 1U;
        static const uint32_t numOutputs = 1U;

        struct NormalizeSurface_Impl* pImpl;
        NormalizeSurface(uint32_t width, uint32_t height, float divisor,CUcontext ctx, CUstream str,Pixel_Format format);
};
} // namespace VPF