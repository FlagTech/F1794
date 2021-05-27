# 請參閱A-5頁

import asyncio
import time


def now(): return time.time()


async def task(name: str):   # 定義協程 (coroutine)
    print(f'"{name}" 訂單處理中…')
    await asyncio.sleep(3)
    print(f'"{name}" 完成')

task1 = asyncio.ensure_future(task('牛肉麵'))
task2 = asyncio.ensure_future(task('熱狗'))
task3 = asyncio.ensure_future(task('珍珠奶茶'))
tasks = [task1, task2, task3]

start = now()
loop = asyncio.get_event_loop()            # 建立事件迴圈
loop.run_until_complete(asyncio.wait(tasks))  # 執行到完成全部任務
loop.close()    # 關閉事件迴圈
print(f'花費時間：{now() - start}秒')
