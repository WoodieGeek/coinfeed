import asyncio
import websockets
import json
import time

async def connect():
    uri = 'wss://ws-feed.exchange.coinbase.com'
    # uri = 'wss://ws-feed-public.sandbox.exchange.coinbase.com'
    # Connecting to the WebSocket.
    async with websockets.connect(uri, ping_interval=None) as websocket:
        print("connected\n")
        subscribe_message = json.dumps({
            'type': 'subscribe',
            'channels': [{'name': 'heartbeat'}, {'name': 'ticker', 'product_ids': ['ETH-BTC', 'ETH-USD']}],
            'timestamp': str(time.time())
        })
        print("sent\n")
        await websocket.send(subscribe_message)
        print("sented\n")
        while True:
            greeting = await websocket.recv()
            print(f"Received: {greeting}")

if __name__ == '__main__':
    # Starting the asyncio event loop to run the connect coroutine.
    print('run\n')
    asyncio.run(connect())

