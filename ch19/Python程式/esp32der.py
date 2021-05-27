# 參閱19-48頁

import argparse
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument("src", help=".der檔案來源")
args = parser.parse_args()    # 解析命令參數

dirname, filename = os.path.split(args.src)  # 分離路徑和檔名
file_ext = filename.split('.')[-1]           # 取得副檔名

exts = ['der', 'pem']

if file_ext.lower() not in exts:
    print('請輸入.der檔或.pem檔')
    sys.exit(2)

with open(args.src, 'rb') as f:
    hexdata = f.read().hex()

arr = '  '   # 兩個空格
for i in range(0, len(hexdata), 2):
    arr = arr + '0x' + hexdata[i:i+2] + ', '
    if (i+2) % 24 == 0:
        arr = arr + '\n  '  # 下一行用兩個空格起頭

if arr[-2] == ',':  # 去掉16進制碼最後一列後面的逗號
    arr = arr[:-2]
else:
    arr = arr[:-5]

if filename.find('.key.') == -1:   # 若檔名不包含'.key.'…
    header = '#ifndef CERT_H_\n'\
             '#define CERT_H_\n'\
             'unsigned char example_crt_DER[] = {\n'  # 程式檔開頭
    footer = '\n};\n'\
             'unsigned int example_crt_DER_len = ' + str(len(hexdata)//2) + ';\n'\
             '#endif'   # 程式檔結尾
    outfile = 'cer.h'   # 輸出檔名
else:
    header = '#ifndef PRIVATE_KEY_H_\n'\
             '#define PRIVATE_KEY_H_\n'\
             'unsigned char example_key_DER[] = {\n'
    footer = '\n};\n'\
             'unsigned int example_key_DER_len = ' + str(len(hexdata)//2) + ';\n'\
             '#endif'
    outfile = 'private_key.h'  # 輸出檔名

with open(os.path.join(dirname, outfile), 'w') as f:
    f.write(header + arr + footer)  # 寫入檔案
