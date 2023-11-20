import socket
import threading
import sys


# Tool to test the irc server with many clients at the same time

server_address = ('127.0.0.1', 6667)
server_password = "hallo"
client_amount = 5


exit_flag = False

def connect_client(client_id):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect(server_address)
        # client_socket.send(f"PASS {server_password}\r\n".encode())
        # client_socket.send(f"NICK nick{client_id}\r\n".encode())
        # client_socket.send(f"USER username{client_id} 0 * :realname{client_id}\r\n".encode())
        # client_socket.send("JOIN #test\r\n".encode())
        # response = client_socket.recv(1024)
        # print(f"Client {client_id} received: {response.decode()}")
        while not exit_flag:
            pass
    except Exception as e:
        print(f"Client {client_id} error: {str(e)}")
    finally:
        client_socket.close()

print(f"Erstelle {client_amount} Clients...")
threads = []
for i in range(0, client_amount):
    client_thread = threading.Thread(target=connect_client, args=(i,))
    client_thread.start()
    threads.append(client_thread)
input("Drücken Sie Enter, um alle Verbindungen zu schließen...")
exit_flag = True
for thread in threads:
    thread.join()
print("Alle Verbindungen wurden geschlossen")
sys.exit(0)
