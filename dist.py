#!/usr/bin/env python3

import subprocess

PLATFORMS = [
    ("windows", "cmake/mingw-toolchain.cmake"),
    ("linux", ""),
]

subprocess.run(["git", "submodule", "update", "--init", "--recursive"])

for platform_name, toolchain_file in PLATFORMS:
    build_directory = f"build/{platform_name}-release"

    subprocess.run(["cmake", "-DCMAKE_BUILD_TYPE=Release", f"-DCMAKE_TOOLCHAIN_FILE={toolchain_file}", "-B", build_directory])
    subprocess.run(["cmake", "--build", build_directory])
    subprocess.run(["cmake", "--build", build_directory, "--target", "package"])
