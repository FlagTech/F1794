# 參閱5-31頁

import os
import argparse
import sys
# import re

BDFCONV_PATH = 'bdfconv\\bdfconv.exe'  # 請自行修改路徑
BDF_PATH = 'bdf\\'                     # 請自行修改路徑

parser = argparse.ArgumentParser()
parser.add_argument("-n", "--name", default='myFont',
                    action="store", help="指定字體的識別名稱")
parser.add_argument("-f", "--font", action="store",
                    default='unifont.bdf', help="指定字體檔名")

args = parser.parse_args()
name = args.name
font = BDF_PATH + args.font


def format_code(c):
    code = ord(c)
    if code > 255:
        return '${:x}'.format(code)
    else:
        return str(code)  # 整數轉成字串


def main():
    text_file = name + '.txt'

    try:
        # 讀取myFont.txt裡的文字
        with open(text_file, encoding='utf-8') as file:
            # txt = re.sub(r"\n|\r|\s+", "", file.read())
            txt = file.read().replace('\n', '').replace('\r', '')
            # 轉換成UTF-8編碼
            encoded = ','.join(format_code(c) for c in txt)
            # encoded = ','.join("${:x}".format(ord(c)) for c in txt)
            print(encoded)

    except FileNotFoundError:
        print(f"出錯囉～找不到 {text_file} 檔。")
        sys.exit()  # 關閉程式

    # 輸出C語言程式檔
    os.system(BDFCONV_PATH +
              f' {font} -b 0 -f 1 -m "{encoded}" -d {font} -n {name} -o {name}.h')


if __name__ == '__main__':
    main()
