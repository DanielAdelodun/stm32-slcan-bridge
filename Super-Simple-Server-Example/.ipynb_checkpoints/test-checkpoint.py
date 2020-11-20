#!/usr/bin/env python

# WS server that sends messages at random intervals

import asyncio
import datetime
import random
import websockets
import json

async def bytearr(websocket, path):
    while True:
        await websocket.send(json.dumps(list(b"\x01\x22\xf1\xeb")))
        await asyncio.sleep(random.random() * 3)
        await websocket.

start_server = websockets.serve(bytearr, "127.0.0.1", 5678)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
print("Ran")