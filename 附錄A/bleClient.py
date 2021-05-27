# 請參閱A-12頁

import asyncio
from bleak import BleakClient, discover

BLE_DEVICE_NAME = 'ESP32藍牙LED開關'
NORDIC_TX = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


def TX_callback(sender, data):  # 偵聽TX特徵的回呼函式
    print(f'收到 "{sender}" 傳來：{data.decode()}')


async def main():
    print("搜尋藍牙裝置…")
    mac_addr = None
    devices = await discover()  # 探索藍牙週邊
    for d in devices:
        print(f"{d.name} @ {d.address}")
        if d.name == BLE_DEVICE_NAME:
            mac_addr = d.address
            print('發現目標了～')
            break

    if mac_addr != None:      # 只要藍牙位址不是「無」
        print("準備連線～")
        client = BleakClient(mac_addr)
        await client.connect()

        try:
            if client.is_connected:
                print("連線成功！")
                await client.start_notify(  # 開始偵聽TX特徵
                    NORDIC_TX, TX_callback
                )
            else:
                print("連線失敗…")
        except Exception as e:
            print(e)
    else:
        loop.stop()   # 停止事件迴圈
        print(f'找不到 "{BLE_DEVICE_NAME}"，請確認有開電源～')
        return

try:
    loop = asyncio.get_event_loop()
    asyncio.ensure_future(main())
    loop.run_forever()
except KeyboardInterrupt:
    loop.stop()
    print("結束程式，bye~")
