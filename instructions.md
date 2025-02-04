Steps to Create an HTTP Server in C with API Routes

- Include Required Libraries
  - Use libraries such as <stdio.h>, <stdlib.h>, <string.h>, <unistd.h>, and <arpa/inet.h>.
- Define Server Configuration
  - Set constants for the port number (e.g., #define PORT 8080) and buffer size (e.g., #define BUFFER_SIZE 1024).
- Create a Socket
  - Use the socket() function to create a socket.
  - Check for errors in socket creation.
- Bind the Socket
  - Use bind() to attach the socket to a specific port and address.
  - Set the server address with struct sockaddr_in.
- Listen for Connections
  - Call listen() to make the server listen for incoming client connections.
- Accept Client Connections
  - Use accept() to accept incoming connections.
  - Create a loop to handle multiple clients.
- Read Incoming Requests
  - Use recv() to read the HTTP request data sent by the client.
  - Parse the request to extract the HTTP method and path.
- Define and Implement Routes
  - GET /all Route:
    - Return a hardcoded or dynamically fetched collection of items in JSON format.
  - GET /item/:id Route:
    - Parse the ID from the request path and return the specific item if it exists, or a 404 error if it doesn’t.
- Send Responses
  - Construct HTTP responses (e.g., headers and body).
  - Use send() to send responses to the client.
- Close Connections
  - Close the client socket using close() after processing the request.
- Shut Down the Server
  - Close the server socket when terminating the application.
