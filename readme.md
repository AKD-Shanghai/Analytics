1.Install dependent library:
	sudo apt-get install libatlas-base-dev libprotobuf-dev libleveldb-dev libsnappy-dev libboost-all-dev libhdf5-serial-dev libgflags-dev libgoogle-glog-dev liblmdb-dev protobuf-compiler -y
	sudo apt-get install libtcmalloc-minimal4
	sudo vim /home/nvidia/.bashrc 
	export LD_PRELOAD="/usr/lib/libtcmalloc_minimal.so.4"

2.Install opencv3.2:

3.Modify the model_dir of each node in TX1ProcessFlow.xml and copy the TX1ProcessFlow.xml to /etc folder of your NVIDIA tegra.



