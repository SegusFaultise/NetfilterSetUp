# NetfilterSetUp
Check out the main.c file!,
this example is within ubuntu

# Install
  ```
  sudo apt-get install libnetfilter-queue-dev
  ```
# includes
main includes from the lib
  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <netinet/ip.h>
  #include <netinet/tcp.h>
  #include <libnetfilter_queue/libnetfilter_queue.h>
  ```
# Compile 
  ```
  gcc -o my_netfilter_program my_netfilter_program.c -lnetfilter_queue
  ```
# Run
  ```
  sudo ./my_netfilter_program
  ```
