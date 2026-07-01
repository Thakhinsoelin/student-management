# Stage 1: Build the app
# Use Fedora Rawhide for the absolute latest GCC/libstdc++
FROM fedora:rawhide AS build

# Install build dependencies via dnf
RUN dnf install -y \
    cmake \
    gcc-c++ \
    libpq-devel \
    make \
    ninja-build \
    && dnf clean all

WORKDIR /app
COPY . .

# Build your project
RUN mkdir build && cd build && \
    cmake .. -G "Ninja Multi-Config" && \
    cmake --build . --config Release -j$(nproc)

# Stage 2: Runtime
# Use a standard fedora:latest for the runtime environment
FROM fedora:latest

# Install runtime dependencies (libpq)
RUN dnf install -y libpq && dnf clean all

WORKDIR /app

# Preserve your directory structure: 
# Copy the entire build directory or just the binary using your find strategy
RUN --mount=type=bind,from=build,source=/app/build,target=/mnt/build \
    find /mnt/build -name "StudentManagement" -exec cp {} /app/StudentManagement \;

RUN chmod +x /app/StudentManagement

EXPOSE 9000
CMD ["/app/StudentManagement"]
