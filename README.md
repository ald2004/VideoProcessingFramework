# VideoProcessingFramework

VPF stands for Video Processing Framework. It’s set of C++ libraries and Python bindings which provides full HW acceleration for video processing tasks such as decoding, encoding, transcoding and GPU-accelerated color space and pixel format conversions.

VPF also supports exporting GPU memory objects such as decoded video frames to PyTorch tensors without Host to Device copies.

![image](https://user-images.githubusercontent.com/4443533/114979113-e3ee0800-9ebc-11eb-8142-853a6e9978f8.png)

curl http://localhost:8000/v2/cudasharedmemory/status

import tritonclient.http as httpclient

def clean_triron_shm():
    try:
        import tritonclient.http as httpclient
        triton_server_url = "localhost:8000"
        triton_client = httpclient.InferenceServerClient(url=triton_server_url, verbose=False)
        triton_client.unregister_system_shared_memory()
        triton_client.unregister_cuda_shared_memory()
    except:
        pass
