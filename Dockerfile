FROM ubuntu:20.04

# Set the working directory in the container to /app
WORKDIR /app

# Install any needed packages
RUN apt-get update && \
    apt-get install -y g++ make

# Install PJSIP from source
RUN apt-get install -y git && \
    git clone https://github.com/pjsip/pjproject.git && \
    cd pjproject && \
    ./configure --enable-shared && \
    make dep && \
    make && \
    make install && \
    ldconfig && \
    cd .. && \
    rm -rf pjproject

# Copy the current directory contents into the container at /app
COPY . /app

# Build the application
RUN make

# Command to run the application
CMD ["./main"]
