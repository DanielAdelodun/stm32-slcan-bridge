import isotp
import socket
import websockets

# Create the ISOTP socket
isotp_socket = isotp.socket()
isotp_socket.bind("slcan0", isotp.Address(rxid=0x7E8, txid=0x7DF))
print(s)