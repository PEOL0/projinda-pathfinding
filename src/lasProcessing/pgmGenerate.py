import numpy as np
from PIL import Image

data = np.random.randint(0, 256, (370, 725), dtype=np.uint8)
Image.fromarray(data).save("test.pgm")
