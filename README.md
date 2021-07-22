# kubernetes-udp-hpa-issue

A demonstration repository to aid in the debugging of an issue when using the Horizontal Pod Autoscaler (HPA) in Kubernetes with UDP traffic when the socket sending the traffic is reused for each message.

It consists of a basic UDP server and client, where the server has been configured to perform a computationally-intensive operation every time it receives a UDP message.

## Local Usage

1. Compile the server: `gcc server.c -o server`
2. Compile the client: `gcc client.c -o client`
3. Run the server: `./server 127.0.0.1 8080`
4. Run the client: `./client 127.0.0.1 8080`

The expected result would be lots of messages of the form:

```
Hello message sent.
Server : Hello from 127.0.0.1
```

## Running on Kubernetes

Note: the Horizontal Pod Autoscaler requires the metrics-server to be setup and running on the Kubernetes cluster.

1. Load the configuration: `kubectl apply -f deployment.yaml`
2. Get the IP (a ClusterIP) of the udp-worker service: `kubectl get service udp-worker`
3. Compile the client: `gcc client.c -o client`
4. Run the client: `./client <udp-worker service IP> 8080`

### Observing the issue

The output from the client will be lots of messages of the form:

```
Hello message sent.
Server : Hello from <IP>
```

where `<IP>` is the internal IP of the pod (visible through `kubectl get pods -o wide`). The problem with this is that `<IP>` will not change, even as the HPA scales up more pods to deal with the "load" from the client.

If the CPU utilisation of the worker pods is observed through `kubectl top pods` then it will be observed that only one of the worker pods is receiving and responding to the UDP messages, since only one pod will have a CPU utilisation above 0 milli-cores, while all the other pods will have a utilisation of 0 milli-cores. (Note that it may take a few minutes for the utilisation of newly-created pods to drop to zero).
  
In contrast, if the client source file `client-newsocket.c` is compiled and used in place of `client.c`, then the UDP traffic is distributed among all the worker pods. The only difference between these two client source files is that `client.c` re-uses the same UDP socket for each message, while `client-newsocket.c` creates a new socket for every message.
