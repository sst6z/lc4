import socket

class TcpClient:
    def __init__(self, ip, port):
        self.server_info = (ip, port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(self.server_info)

    def send_buffer(self, buffer):
        self.sock.send(buffer)

    def close():
        self.sock.close()
