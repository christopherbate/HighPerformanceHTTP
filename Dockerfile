FROM gcc:latest

# Create our work dir
WORKDIR /app

# Add the files
ADD . . 

# Compile
RUN make

ENTRYPOINT [ "/bin/bash" ]