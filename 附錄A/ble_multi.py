# 請參閱A-21頁

import asyncio
from bleak import BleakClient, discover
import sys

BLE_DEVICE_NAME = 'ESP32'  # 目標裝置名稱
NORDIC_TX = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


class TXCallback:
    def __init__(self, name):
        self.name = name

    def callback(self, sender, data):
        print(f'收到 "{self.name}" 傳來：{data.decode()}')


async def select_device():
    ble_list = []
    devices = await discover()
    print("搜尋藍牙裝置…")
    for d in devices:
        print(f"{d.name} @ {d.address}")  # 列舉藍牙裝置名稱和位址
        if d.name.find(BLE_DEVICE_NAME) != -1:
            ble_list.append({'name': d.name, 'address': d.address})
            print('發現目標：', d.name)

    return ble_list


async def run():
    ble_list = await select_device()

    if len(ble_list) == 0:
        print('找不到ESP32藍牙裝置')
        loop.stop()
        return

    print("準備連線～")
    for d in ble_list:
        client = BleakClient(d['address'])
        try:
            await client.connect()
            cb = TXCallback(d['name'])
            await client.start_notify(
                NORDIC_TX, cb.callback
            )
        except Exception as e:
            print(e)


if __name__ == "__main__":
    try:
        loop = asyncio.get_event_loop()
        asyncio.ensure_future(run())
        loop.run_forever()
    except KeyboardInterrupt:
        loop.stop()
        print("關閉程式，bye~")
