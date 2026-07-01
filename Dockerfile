# Stage 1: Build the app
FROM debian:bookworm-slim AS build

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    libpq-dev \
    make \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build your project
RUN mkdir build && cd build && \
    cmake .. -G "Ninja Multi-Config" && \
    cmake --build . --config Release -j

# Stage 2: Runtime
FROM debian:bookworm-slim

# Install runtime dependencies (libpq)
RUN apt-get update && apt-get install -y libpq5 && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Copy the compiled binary from the build stage
RUN --mount=type=bind,from=build,source=/app/build,target=/mnt/build \
    find /mnt/build -name "StudentManagement" -exec cp {} /app/StudentManagement \;

RUN chmod +x /app/StudentManagement

EXPOSE 9000
CMD ["/app/StudentManagement"]
