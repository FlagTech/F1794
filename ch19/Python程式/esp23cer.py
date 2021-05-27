# 參閱19-21頁

import argparse
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument("src", help=".cer檔案來源")
args = parser.parse_args()    # 解析命令參數

dirname, filename = os.path.split(args.src)  # 取出路徑和檔名
file_ext = filename.split('.')[-1]           # 取得副檔名

if file_ext != 'cer':         # 若副檔名不是.cer則退出
    print('請輸入.cer檔')
    sys.exit(2)

with open(args.src, 'r') as f:
    lines = f.readlines()     # 讀入每一行文字

for index, item in enumerate(lines[:-1]):
    lines[index] = f'"{item.strip()}\\n"\\\n'   # 取代每一行內容

lines[-1] = f'"{lines[-1].strip()}\\n";\n'       # 取代最後一行
lines = ['const char* root_ca = \\\n'] + lines  # 新增起頭行

outfile = filename.split('.')[0] + '.h'       # 寫入檔名以.h結尾
with open(os.path.join(dirname, outfile), 'w') as f:
    f.writelines(lines)
