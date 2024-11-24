import socket
import threading
import json
import random

class LANServer:
    index = 0  # Class attribute to keep track of client IDs

    def __init__(self, host='0.0.0.0', port=12345):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        self.clients = []  
        self.access_code = str(random.randint(1000, 9999))  

    def broadcast(self, message, sender_socket):
        """Send a message to all connected clients except the sender."""
        for client in self.clients:
            if client != sender_socket:
                try:
                    client.sendall(message.encode('utf-8'))
                except Exception as e:
                    print(f"Error sending message: {e}")

    def handle_client(self, client_socket, address):
        """Handle communication with a single client."""
        print(f"Client connected: {address}")

        client_socket.sendall(f"Enter the server access code: {self.access_code}\n".encode('utf-8'))
        
        try:
            # Receive the code from the client
            client_code = client_socket.recv(1024).decode('utf-8').strip()
            if client_code != self.access_code:
                print(f"Incorrect access code from {address}, rejecting connection.")
                client_socket.sendall("Incorrect access code. Connection rejected.\n".encode('utf-8'))
                client_socket.close()
                return

            # Assign a unique ID to the client
            client_id = LANServer.index
            LANServer.index += 1

            # Send the client ID to the client
            client_socket.sendall(f"Your client ID is: {client_id}\n".encode('utf-8'))

            print(f"Access code verified for {address}. Connection established. Client ID: {client_id}")
            self.clients.append(client_socket)

            try:
                while True:
                    data = client_socket.recv(1024).decode('utf-8')
                    if not data:
                        break
                    
                    print(f"Received from {address}: {data}")
                    
                    try:
                        message = json.loads(data)
                        print(f"Processed JSON: {message}")
                        
                        character_selected = message.get('selectedCharacter')
                        xpos2d = message.get('xpos2D')
                        ypos2d = message.get('ypos2D')
                        id = message.get('id')
                        print(f"CharacterSelected: {character_selected}, Xpos2D: {xpos2d}, Ypos2d: {ypos2d}, id: {id}")
                        
                        # Optionally broadcast the message
                        self.broadcast(data, client_socket)
                    except json.JSONDecodeError:
                        print(f"Invalid JSON from {address}")
            except Exception as e:
                print(f"Error with client {address}: {e}")
            finally:
                print(f"Client disconnected: {address}")
                self.clients.remove(client_socket)
                client_socket.close()
        except ConnectionAbortedError as e:
            print(f"Connection aborted by client {address}: {e}")
        except Exception as e:
            print(f"Error with client {address}: {e}")
        finally:
            client_socket.close()

    def start(self):
        """Start the server and handle incoming connections."""
        print(f"Server starting on {self.host}:{self.port}")
        print(f"Server access code: {self.access_code}")  
        try:
            while True:
                client_socket, address = self.server_socket.accept()
                client_thread = threading.Thread(target=self.handle_client, args=(client_socket, address))
                client_thread.daemon = True
                client_thread.start()
        except KeyboardInterrupt:
            print("Server shutting down...")
        finally:
            self.server_socket.close()

if __name__ == "__main__":
    server = LANServer()
    server.start()