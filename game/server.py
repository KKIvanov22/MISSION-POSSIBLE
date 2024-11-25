import socket
import threading
import json
import random
import time

class LANServer:
    index = 0  # Class attribute to keep track of client IDs

    def __init__(self, host='0.0.0.0', port=12345, tick_rate=0.1):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        self.clients = []  # List to store connected client sockets
        self.client_data = {}  # Dictionary to store client data
        self.access_code = str(random.randint(1000, 9999))  # Random access code
        self.tick_rate = tick_rate  # Time in seconds between ticks
        self.lock = threading.Lock()  # For thread-safe data access

    def broadcast(self):
        """Send all clients' data to every connected client."""
        with self.lock:
            if not self.clients:
                return

            # Prepare data in the format "id,characterSelected,xpos2D,ypos2D"
            messages = []
            for client_id, data in self.client_data.items():
                message = f"{client_id},{data['character']},{data['xpos2D']},{data['ypos2D']}"
                messages.append(message)
            
            # Join all messages with a newline character
            message = '\n'.join(messages) + '\n'

            # Send data to all clients
            for client in self.clients:
                try:
                    client.sendall(message.encode('utf-8'))
                except Exception as e:
                    print(f"Error broadcasting to a client: {e}")
                    self.clients.remove(client)

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
            with self.lock:
                client_id = LANServer.index
                LANServer.index += 1
                self.client_data[client_id] = {"character": None, "xpos2D": 0, "ypos2D": 0}
            
            # Send the client ID
            client_socket.sendall(f"{client_id}\n".encode('utf-8'))
            print(f"Access code verified. Client {client_id} connected from {address}.")
            
            # Add client to the list
            with self.lock:
                self.clients.append(client_socket)

            # Handle client messages
            while True:
                data = client_socket.recv(1024).decode('utf-8').strip()
                if not data:
                    break  # Client disconnected
                
                try:
                    message = json.loads(data)
                    with self.lock:
                        id = message.get("id")
                        if id is not None:
                            id = int(id)
                            self.client_data[id] = {
                                "character": 2,
                                "xpos2D": message.get("xpos2D"),
                                "ypos2D": message.get("ypos2D"),
                            }
                            print(f"Updated client {id} data: {self.client_data[id]}")
                        else:
                                print("Client ID missing in message!")
                        for i in range (0,LANServer.index):
                             print(f"Client ID 0 data: {json.dumps(self.client_data.get(i, {}), indent=4)}")
                except json.JSONDecodeError:
                    print(f"Invalid JSON received from {address}: {data}")
        except Exception as e:
            print(f"Error with client {address}: {e}")
        finally:
            with self.lock:
                self.clients.remove(client_socket)
                del self.client_data[client_id]
            print(f"Client {client_id} disconnected.")
            client_socket.close()
            
    def start_broadcast_loop(self):
        """Broadcast data to all clients periodically."""
        while True:
            time.sleep(self.tick_rate)
            self.broadcast()

    def start(self):
        """Start the server and handle incoming connections."""
        print(f"Server starting on {self.host}:{self.port}")
        print(f"Server access code: {self.access_code}")
        
        # Start the broadcasting thread
        broadcast_thread = threading.Thread(target=self.start_broadcast_loop)
        broadcast_thread.daemon = True
        broadcast_thread.start()

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
