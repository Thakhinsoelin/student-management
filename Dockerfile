# Stage 1: Build the app
FROM debian:bookworm-slim AS build

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    libpq-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build your project
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Stage 2: Runtime
FROM debian:bookworm-slim

# Install runtime dependencies (libpq)
RUN apt-get update && apt-get install -y libpq5 && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Copy the compiled binary from the build stage
COPY --from=build /app/build/src/StudentManagement /app/StudentManagement

# Expose the port (Render will inject the PORT environment variable)
EXPOSE 9000

CMD ["/app/StudentManagement"]
