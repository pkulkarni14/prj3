# Project Name - Multi-Threaded Chat Server

Project Member
- Prasad Kulkarni
- Rohan Patil
- Ruturaj Nilawar
- Pushkar Kaswankar

# Project Description
The Chat Server is a multi-threaded TCP server application designed to handle real-time chat functionalities. It supports multiple clients simultaneously, allowing them to communicate with each other through messages. Each client has a unique ID and username, and the server logs events such as client connections, disconnections, and message broadcasts. This project demonstrates concepts like sockets, threading, concurrency, and signal handling in C.

# Commands to execute the Project
1. make
2. ./chat_server [port_number](Default: 8080)
3. nc localhost [port_number]