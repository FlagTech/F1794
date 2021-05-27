# 請參閱A-9頁

import asyncio
import time

orders = ['牛肉麵', '熱狗', '珍珠奶茶']  # 定義3筆訂單


def now(): return time.time()


async def task(name):   # 定義協程 (coroutine)
    print(f'"{name}" 訂單處理中…')
    await asyncio.sleep(3)
    print(f'"{name}" 完成')

# 定義協程列表
tasks = [task(orders[i]) for i in range(len(orders))]

start = now()
asyncio.run(asyncio.wait(tasks))  # 用run()非同步執行任務
print(f'花費時間：{now() - start}秒')
