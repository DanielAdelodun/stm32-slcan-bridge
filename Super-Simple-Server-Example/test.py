#!/usr/bin/env python

import asyncio
import datetime
import random
import websockets
import isotp
import json

s = isotp.socket()
s.bind("vcan0", isotp.Address(rxid=0x7E8, txid=0x7DF))

async def bytestring(websocket, path):
    while True:
        # Recieve command from client
        command = await websocket.recv()
        if (command == "DTC"):
            data = b"\x03"       
            print("DTC Recieved")

        elif (command == "RPM"):
            data = b"\x01\x0C"
            print("RPM Recieved")

        elif (command == "PIDS"):
            data = b"\x01\x00"
            print("PIDS Recieved")

        elif (command == "SPEED"):
            data = b"\x01\x0D"
            print("SPEED Recieved")

        else:
            print("Unknown Command.")
            continue

        # Send request data to CAN Bus.
        try:
            s.send(data)
        except:
            print("Error: ISOTP: Could not send request.")

        # Wait a little bit, then get the response.
        await asyncio.sleep(0.1)
        try: 
            response = s.recv() 
        except: 
            print("Error: ISOTP: Could not get reponse.")
            continue

        # Send response back to client.
        try: 
            await websocket.send(json.dumps(list(response))) 
        except: 
            print("Error: Websockets: Could not send reponse.")
            continue

start_server = websockets.serve(bytestring, "127.0.0.1", 5678)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
