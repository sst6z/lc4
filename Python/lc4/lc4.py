from lc4.frame import Frame
from lc4.tcp_client import TcpClient
from lc4.qr_reader import QRReader
from time import sleep
import validators
import requests
from PIL import UnidentifiedImageError

IP = '10.0.0.42'
PORT = 23

SLEEP_TIME = 0.1

class LC4:
    def __init__(self, path):
        self.path = path
        self.url = ''
        self.tcp_client = TcpClient(IP, PORT)
        self.qr_reader = QRReader()

    def run(self):
        while True:
            try:
                new_image = False
                with Frame(self.path) as img:
                    while not new_image:
                        img.scale()
                        buffer = img.get_buffer()
                        self.tcp_client.send_buffer(buffer)
                        sleep_duration = img.next_frame()
                        time = 0
                        while time < sleep_duration:
                            sleep(SLEEP_TIME)
                            time += SLEEP_TIME
                            qr_result = self.qr_reader.scan()
                            if qr_result[0] == -1:
                                return
                            elif qr_result[0] == 1:
                                url = qr_result[1][0]
                                if self.url != url:
                                    print('New URL detected')
                                    if not validators.url(url):
                                        print('URL invalid')
                                    else:
                                        name = 'image'
                                        if url.find('/'):
                                            name = url.rsplit('/', 1)[1]
                                        r = requests.get(url, allow_redirects=True)
                                        print('Filename: ' + name)
                                        self.path = 'temp/' + name
                                        self.url = url
                                        open(self.path, 'wb').write(r.content)
                                        new_image = True
            except UnidentifiedImageError:
                print('Invalid image format')
                self.path = 'resources/fairy-dust.gif'
