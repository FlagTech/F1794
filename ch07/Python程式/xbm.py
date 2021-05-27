import argparse
from os import path, listdir
from PIL import Image, ImageOps

parser = argparse.ArgumentParser()
parser.add_argument('src', help='來源資料夾或檔案路徑')
args = parser.parse_args()


# 轉換單一圖檔
# src參數：原始圖檔名
# dir參數：原始圖檔所在路徑
def to_xbm(f, dir):
    img_path = path.join(dir, f)  # 合併原始檔路徑和檔名
    img_name = f.split('.')[0]    # 取得檔名（去除副檔名）

    try:
        img = Image.open(img_path)
        img = img.convert('L')
        img = ImageOps.invert(img)
        xbm = img.convert('1').tobitmap(img_name)
    except:
        print(f'轉換 {f} 檔案時發生錯誤！')

    try:
        src = xbm.decode('utf-8')
        xbm_path = path.join(dir, f'{img_name}.xbm')
        with open(xbm_path, encoding='utf-8', mode='w') as f:
            f.write(src)       # 儲存XBM影像檔

        begin = src.find('{')  # 取得圖像編碼
        code = src[begin:]
        src_code = f'const unsigned char {img_name}[] {code}'
        code_path = path.join(dir, 'bmp.h')
        with open(code_path, encoding='utf-8', mode='a') as f:
            f.write(src_code + '\n\n')
    except:
        print('寫入檔案時出錯了～')


# 批次轉換XBM檔，接收一個「檔案路徑」參數。
def batch_convert(dir):
    try:
        for f in listdir(dir):
            if path.isfile(path.join(dir, f)):
                to_xbm(f, dir)
    except:
        print('指定路徑沒有檔案！')


def main():
    if path.isfile(args.src):
        dir_path = path.dirname(args.src)
        to_xbm(args.src, dir_path)
    elif path.isdir(args.src):
        batch_convert(args.src)
    else:
        print(f'找不到 {args.src}')


if __name__ == '__main__':
    main()
