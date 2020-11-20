import isotp
import socket

# Create the ISOTP socket to act as Car
s = isotp.socket()
s.bind("vcan0", isotp.Address(rxid=0x7DF, txid=0x7E8))

# Respond to requests as if you where a Car.
while(True):
    request = s.recv()
    if (request):
        if (request == b"\x03"): # DTCs
            s.send(b"\x00\x11\x22\xDE\xAD\xBE\xEF\xAA\xBB\xCC\xDD\xEE\xFF")
        if (request == b"\x01\x0C"): # RPM
            s.send(b"\x01\x0C\x0F\x0F")
        if (request == b"\x01\x00"): # PIDS
            s.send(b"\x01\x00\xFF\xFF\xFF\xFF")
        if (request == b"\x01\x0D"): # SPEED
            s.send(b"\x01\x0D\x00")