#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <string>

struct SharedMemoryHandle {
    std::string triton_shm_name_;
    std::string shm_key_;
    cudaIpcMemHandle_t cuda_shm_handle_;
    int device_id_;
    void* base_addr_;
    int shm_fd_;
    size_t offset_;
    size_t byte_size_;
};
extern "C" {
    int CudaSharedMemoryRegionSet(
        void* cuda_shm_handle, size_t offset, size_t byte_size, const void* data)
    {
        // remember previous device and set to new device
        int previous_device;
        cudaGetDevice(&previous_device);
        cudaError_t err = cudaSetDevice(
            reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->device_id_);
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -1;
        }

        // Copy data into cuda shared memory
        void* base_addr =
            reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->base_addr_;
        std::cout <<"base_addr is :"<< (unsigned long long) base_addr << std::endl;
        err = cudaMemcpy(
            reinterpret_cast<uint8_t*>(base_addr) + offset, data, byte_size,
            cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -3;
        }

        // Set device to previous GPU
        cudaSetDevice(previous_device);

        return 0;
    }

    int CudaSharedMemoryRegionSet_from_decoder(void* cuda_shm_handle, void* ptr_vpf,size_t offset, size_t byte_size) {
        // remember previous device and set to new device
        int previous_device;
        cudaGetDevice(&previous_device);
        cudaError_t err = cudaSetDevice(
            reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->device_id_);
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -1;
        }

        // Copy data into cuda shared memory
        void* base_addr = reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->base_addr_;
        auto res = cudaMemcpy(reinterpret_cast<uint8_t*>(base_addr) + offset, ptr_vpf, byte_size, cudaMemcpyDeviceToDevice);
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -3;
        }

        // Set device to previous GPU
        cudaSetDevice(previous_device);

        return 0;
    }
int CudaSharedMemoryRegionSet_from_decoder_contiguous(void* cuda_shm_handle, void* ptr_vpf, size_t offset, size_t byte_size,
        size_t width,size_t height, size_t sPitch) {
        // remember previous device and set to new device
        int previous_device;
        cudaGetDevice(&previous_device);
        cudaError_t err = cudaSetDevice(
            reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->device_id_);
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -1;
        }

        // Copy data into cuda shared memory
        void* base_addr = reinterpret_cast<SharedMemoryHandle*>(cuda_shm_handle)->base_addr_;
        //auto res = cudaMemcpy(reinterpret_cast<uint8_t*>(base_addr) + offset, ptr_vpf, byte_size, cudaMemcpyDeviceToDevice);
//        1111111111 0 0 640 1152 2560
//        printf("1111111111 %d %d %d %d %d\n",offset,byte_size,width,height,sPitch);
        auto res = cudaMemcpy2D(reinterpret_cast<float*>(base_addr) + offset, width * sizeof(float),
            ptr_vpf, sPitch,
            width * sizeof(float), height, cudaMemcpyDeviceToDevice);


           /* auto res = cudaMemcpy2D((void*)devMem, width,
                                 (const void*)ptr, pitch,
                                  width, height, cudaMemcpyDeviceToDevice);*/
        if (err != cudaSuccess) {
            cudaSetDevice(previous_device);
            return -3;
        }

        // Set device to previous GPU
        cudaSetDevice(previous_device);

        return 0;
    }
}
