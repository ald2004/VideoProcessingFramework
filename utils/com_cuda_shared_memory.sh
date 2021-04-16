g++ --std=c++11 -fPIC  -shared -W `pkg-config --cflags cuda-11.1` -o libcuda_shared.so  cuda_shared.cc  -L/usr/local/cuda-11.1/targets/x86_64-linux/lib/ -lcuda -lcudart -lrt 
