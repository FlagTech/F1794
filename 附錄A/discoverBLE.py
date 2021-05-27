# 請參閱A-11頁

import asyncio
from bleak import discover


async def main():
    devices = await discover()
    for d in devices:
        print(f'{d.name} @ {d.address}, RSSI:{d.rssi}dBm')

asyncio.run(main())
