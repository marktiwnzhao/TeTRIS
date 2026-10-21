#!/usr/bin/env bash

OPT_ROOT="${OPT_ROOT:-/home/zzj/opt}"
LLVM_HOME="${LLVM_HOME:-$OPT_ROOT/llvm17}"
AFL_SRC_DIR="${AFL_SRC_DIR:-$OPT_ROOT/src/AFLplusplus}"
AFL_PREFIX="${AFL_PREFIX:-$OPT_ROOT/aflplusplus}"
VENV_DIR="${VENV_DIR:-$OPT_ROOT/venvs/tetris}"

export PATH="$VENV_DIR/bin:$LLVM_HOME/bin:$AFL_PREFIX/bin:$PATH"

echo "========== Check commands =========="

check_cmd() {
    if command -v "$1" >/dev/null 2>&1; then
        echo "[OK] $1 -> $(command -v "$1")"
        "$1" --version 2>/dev/null | head -n 1
    else
        echo "[MISS] $1"
    fi
    echo
}

check_cmd make
check_cmd g++
check_cmd bison
check_cmd flex
check_cmd clang
check_cmd clang-format
check_cmd clang-tidy
check_cmd cmake
check_cmd llvm-config
check_cmd meson
check_cmd ninja
check_cmd python3
check_cmd uv
check_cmd git
check_cmd automake
check_cmd autoconf
check_cmd libtoolize
check_cmd afl-fuzz
check_cmd afl-qemu-trace


echo "========== Check apt packages =========="

check_pkg() {
    if dpkg -s "$1" >/dev/null 2>&1; then
        ver=$(dpkg-query -W -f='${Version}' "$1")
        echo "[OK] $1  $ver"
    else
        echo "[MISS] $1"
    fi
}

packages=(
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

for p in "${packages[@]}"; do
    check_pkg "$p"
done


echo
echo "========== Check pip3 packages =========="

if [ -x "$VENV_DIR/bin/python" ]; then
    PIP_CHECK_PYTHON="$VENV_DIR/bin/python"
elif [ -x ".venv/bin/python" ]; then
    PIP_CHECK_PYTHON=".venv/bin/python"
else
    PIP_CHECK_PYTHON="python3"
fi

echo "Using Python for package checks: $PIP_CHECK_PYTHON"

check_pip() {
    if ver=$("$PIP_CHECK_PYTHON" - "$1" <<'PY' 2>/dev/null
import importlib.metadata as md
import sys

print(md.version(sys.argv[1]))
PY
); then
        echo "[OK] $1  $ver"
    else
        echo "[MISS] $1"
    fi
}

pip_packages=(
    matplotlib
    numpy
    scipy
    psutil
    absl-py
)

for p in "${pip_packages[@]}"; do
    check_pip "$p"
done


echo
echo "========== Check AFL++ source directory =========="

if [ -d "$AFL_SRC_DIR" ]; then
    echo "[OK] AFLplusplus source directory exists: $(realpath "$AFL_SRC_DIR")"
else
    echo "[MISS] AFLplusplus source directory not found: $AFL_SRC_DIR"
fi

echo
echo "========== Check opt layout =========="

for path in \
    "$LLVM_HOME/bin/clang" \
    "$LLVM_HOME/bin/clang++" \
    "$LLVM_HOME/bin/llvm-config" \
    "$LLVM_HOME/bin/clang-format" \
    "$LLVM_HOME/bin/clang-tidy" \
    "$VENV_DIR/bin/meson" \
    "$VENV_DIR/bin/ninja" \
    "$AFL_PREFIX/bin/afl-fuzz" \
    "$AFL_PREFIX/bin/afl-qemu-trace" \
    "$VENV_DIR/bin/python"; do
    if [ -e "$path" ]; then
        echo "[OK] $path"
    else
        echo "[MISS] $path"
    fi
done
