import struct
from PIL import Image

im = Image.open(r'minist\test_image.bmp')
size = 0
#D:\program files\C++\NN\Release\
with open(r'minist\test_image', 'wb')as fp:
    fp.write(struct.pack('4i784B', *[size] * 4,
                         *[int(i) for i in im.tobytes()][::3]))
