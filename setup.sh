
#!/bin/bash
#Install necessary build tools and libraries
echo "Installing necessary build tools for Tetris"
sudo apt update
sudo apt install -y make g++ bison flex clang-format
sudo apt install -y build-essential llvm clang libclang-dev cmake libssl-dev pkg-config python3 python3-pip git
sudo apt install libcapstone-dev
sudo apt-get install -y  automake autoconf libtool libglib2.0-dev libpixman-1-dev

# Additional dependencies
sudo apt install -y python2
sudo apt install -y clang
sudo apt install -y clang-tidy
pip3 install matplotlib numpy scipy psutil absl-py

echo "Installing AFL++ and afl-qemu-trace"
git clone https://github.com/AFLplusplus/AFLplusplus.git
cd AFLplusplus
make distrib
sudo make install          
cd qemu_mode && ./build_qemu_support.sh
which afl-qemu-trace
