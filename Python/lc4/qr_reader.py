import cv2

class QRReader:
    def __init__(self):
        camera_id = 0
        self.window_name = 'OpenCV QR Code'

        self.qcd = cv2.QRCodeDetector()
        self.cap = cv2.VideoCapture(camera_id)

    def scan(self):
        result = 0
        strings = []
        ret, frame = self.cap.read()

        if ret:
            ret_qr, decoded_info, points, _ = self.qcd.detectAndDecodeMulti(frame)
            if ret_qr:
                for s, p in zip(decoded_info, points):
                    if s:
                        strings.append(s)
                        color = (0, 255, 0)
                        result = 1
                    else:
                        color = (0, 0, 255)
                    frame = cv2.polylines(frame, [p.astype(int)], True, color, 8)
            cv2.imshow(self.window_name, frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                cv2.destroyWindow(self.window_name)
                result = -1
        return (result, strings)
