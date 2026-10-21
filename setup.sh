#!/usr/bin/env bash
set -euo pipefail

echo "Installing necessary build tools for TeTRIS"

OPT_ROOT="${OPT_ROOT:-/home/zzj/opt}"
LLVM_HOME="${LLVM_HOME:-$OPT_ROOT/llvm17}"
AFL_SRC_DIR="${AFL_SRC_DIR:-$OPT_ROOT/src/AFLplusplus}"
AFL_PREFIX="${AFL_PREFIX:-$OPT_ROOT/aflplusplus}"
VENV_DIR="${VENV_DIR:-$OPT_ROOT/venvs/tetris}"

if [ ! -x "$LLVM_HOME/bin/clang" ]; then
    echo "Missing clang at $LLVM_HOME/bin/clang"
    echo "Set LLVM_HOME to your LLVM installation, or install LLVM there first."
    exit 1
fi

if [ ! -x "$LLVM_HOME/bin/clang++" ]; then
    echo "Missing clang++ at $LLVM_HOME/bin/clang++"
    exit 1
fi

if [ ! -x "$LLVM_HOME/bin/llvm-config" ]; then
    echo "Missing llvm-config at $LLVM_HOME/bin/llvm-config"
    exit 1
fi

export PATH="$LLVM_HOME/bin:$AFL_PREFIX/bin:$PATH"
export CC="$LLVM_HOME/bin/clang"
export CXX="$LLVM_HOME/bin/clang++"
export LLVM_CONFIG="$LLVM_HOME/bin/llvm-config"
export AFL_CC="$LLVM_HOME/bin/clang"
export AFL_CXX="$LLVM_HOME/bin/clang++"

APT_PACKAGES=(
    make
    g++
    bison
    flex
    build-essential
    cmake
    libssl-dev
    pkg-config
    python3
    git
    libcapstone-dev
    automake
    autoconf
    libtool
    libglib2.0-dev
    libpixman-1-dev
    python2
)

PYTHON_PACKAGES=(
    matplotlib
    numpy
    scipy
    psutil
    absl-py
)

BUILD_PYTHON_PACKAGES=(
    meson
    ninja
)

sudo apt update
sudo apt install -y "${APT_PACKAGES[@]}"

for tool in clang-format clang-tidy; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "Missing $tool in $LLVM_HOME/bin"
        echo "Install it into your LLVM tree or set LLVM_HOME to a complete LLVM installation."
        exit 1
    fi
done

echo "Installing Python dependencies"
if command -v uv >/dev/null 2>&1; then
    if [ "${INSTALL_PYTHON_SYSTEM:-0}" = "1" ]; then
        uv pip install --system "${PYTHON_PACKAGES[@]}" "${BUILD_PYTHON_PACKAGES[@]}"
    else
        mkdir -p "$(dirname "$VENV_DIR")"
        uv venv "$VENV_DIR" --python python3
        uv pip install --python "$VENV_DIR/bin/python" "${PYTHON_PACKAGES[@]}" "${BUILD_PYTHON_PACKAGES[@]}"
        export PATH="$VENV_DIR/bin:$PATH"
        echo "Python packages were installed in $VENV_DIR."
        echo "Use: source $VENV_DIR/bin/activate"
    fi
else
    echo "uv is required for Python dependencies. Install uv or put it on PATH."
    exit 1
fi

if ! command -v meson >/dev/null 2>&1; then
    echo "Missing meson after Python dependency installation"
    exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
    echo "Missing ninja after Python dependency installation"
    exit 1
fi

echo "Installing AFL++ and afl-qemu-trace"
mkdir -p "$(dirname "$AFL_SRC_DIR")" "$AFL_PREFIX"

if [ ! -d "$AFL_SRC_DIR" ]; then
    git clone https://github.com/AFLplusplus/AFLplusplus.git "$AFL_SRC_DIR"
fi

cd "$AFL_SRC_DIR"
make -j"$(nproc)" all
cd qemu_mode
unset CFLAGS
./build_qemu_support.sh
cd ..
make install PREFIX="$AFL_PREFIX"

which afl-fuzz
which afl-qemu-trace

echo
echo "Add these lines to your shell profile if they are not already present:"
echo "export LLVM_HOME=$LLVM_HOME"
echo "export AFL_PREFIX=$AFL_PREFIX"
echo 'export PATH="$LLVM_HOME/bin:$AFL_PREFIX/bin:$PATH"'
