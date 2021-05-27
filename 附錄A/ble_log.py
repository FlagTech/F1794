# 請參閱A-19頁

import asyncio
from aioconsole import ainput
from bleak import BleakClient, discover
import time


NORDIC_RX = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
NORDIC_TX = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


async def select_device():
    print("搜尋藍牙裝置…")
    devices = await discover()  # 探索藍牙裝置

    for i, d in enumerate(devices):
        print(f"{i}: {d.name}")

    choose = -1
    while True:
        choose = await ainput("請輸入選擇編號：")
        try:
            choose = int(choose.strip())
        except:
            print("請輸入數字～")

        # 若使用者輸入的數字不在藍牙選項數字範圍…
        if choose > -1 and choose < len(devices):
            break
        else:
            print("請輸入有效的數字")

    name = devices[choose].name
    addr = devices[choose].address
    return (name, addr)


def TX_callback(sender, data):
    date_str = time.strftime('%Y/%m/%d %H:%M:%S', time.localtime())
    log = date_str + ',' + data.decode() + '\n'

    with open('ble_log.csv', mode='a+', encoding='utf-8') as file:
        file.write(log)


async def main():
    (name, addr) = await select_device()
    client = BleakClient(addr)

    print("準備連線～")
    try:
        await client.connect()

        if client.is_connected:
            print(f"{name} 連線成功！")
            await client.start_notify(
                NORDIC_TX, TX_callback,
            )
            while True:
                txt = await ainput("請輸入on或off：")
                if txt != 'on':
                    txt = 'off'
                data = bytearray(txt, 'utf-8')

                await client.write_gatt_char(NORDIC_RX, data)
                print(f"已傳送：{txt}")
        else:
            print("連線失敗…")
    except Exception as e:
        print('連線出錯了：', e)

try:
    loop = asyncio.get_event_loop()
    asyncio.ensure_future(main())
    loop.run_forever()
except KeyboardInterrupt:
    loop.stop()
    print("關閉程式，bye~")
