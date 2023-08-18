from numpy import asarray
from PIL import Image

CYL_HEIGHT = 72
CYL_WIDTH = 374
CYL_ASPECT = CYL_WIDTH / CYL_HEIGHT
# Correction for non-square pixels
CYL_CORRECTION = 0.8

class Frame:
    def __init__(self, path):
        self.path = path
        self.shift = 0

    def __enter__(self):
        self.img = Image.open(self.path)
        self.is_animated = getattr(self.img, 'is_animated', False)
        self.n_frames = getattr(self.img, 'n_frames', 1)
        return self

    def __exit__(self, *exc):
        self.img.close()

    def get_num_frames(self):
        return self.n_frames

    def scale(self):
        self.img_scaled = self.img.convert('RGB')
        src_aspect = self.img_scaled.size[0] / self.img_scaled.size[1]
        # Is correction applied correctly?
        if src_aspect > (CYL_ASPECT * CYL_CORRECTION):
            height = (CYL_WIDTH * CYL_CORRECTION) / src_aspect
            self.img_scaled = self.img_scaled.resize(((int)(CYL_WIDTH), (int)(height)))
        else:
            width = (CYL_HEIGHT / CYL_CORRECTION) * src_aspect
            self.img_scaled = self.img_scaled.resize(((int)(width), (int)(CYL_HEIGHT)))

    def get_buffer(self):
        img_width = self.img_scaled.size[0]
        img_height = self.img_scaled.size[1]
        mult = (int)(CYL_WIDTH / img_width)
        x_padding = []
        y_padding = []
        if (img_height < CYL_HEIGHT):
            x_padding = [0, 0]
            x_padding.append((int)((CYL_HEIGHT - img_height) / 2))
            x_padding.append(CYL_HEIGHT - (img_height + x_padding[0]))
            y_padding = [0, 0]
        else:
            x_padding = [0, 0]
            padding = (int)((CYL_WIDTH - img_width * mult) / mult)
            for i in range(mult - 1):
                y_padding.append(padding)
            y_padding.append(CYL_WIDTH - (img_width * mult + padding * (mult - 1)))

        img_array = asarray(self.img_scaled)

        buffer = bytearray()
        black_pixel = bytearray([0, 0, 0])
        for i in range(mult):
            for y in range(img_width):
                buffer += black_pixel * x_padding[0]
                for x in range(img_height):
                    pixel = img_array[x][y]
                    buffer += bytearray([pixel[0], pixel[1], pixel[2]])
                buffer += black_pixel * x_padding[1]
            buffer += black_pixel * CYL_HEIGHT * y_padding[i]

        cut = (CYL_HEIGHT * self.shift) * 3
        start = buffer[cut:]
        end = buffer[:cut]
        buffer = start + end;

        return buffer

    def next_frame(self):
        duration = 0.1
        if self.is_animated:
            self.shift -= 4
            if self.shift < 0:
                self.shift += CYL_WIDTH
            try:
                self.img.seek(self.img.tell() + 1)
            except EOFError:
                self.img.seek(0)
            duration = self.img.info['duration'] / 1000.
        return duration
