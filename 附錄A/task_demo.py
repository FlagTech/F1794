# 請參閱A-3頁

import time

orders = ['牛肉麵', '熱狗', '珍珠奶茶']  # 定義3筆訂單

def now(): return time.time()  # 傳回現在時間（秒數）

def task(name):  # 執行訂單任務
    print(f'"{name}" 訂單處理中…')
    time.sleep(3)  # 等待3秒
    print(f'"{name}" 完成')

start = now() # 紀錄現在時間
for i in range(len(orders)):  # 處理所有訂單
    task(orders[i])

print(f'花費時間：{now() - start}秒')