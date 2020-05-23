from os import system

import cv2

PIXWIDTH = 640
PIXHEIGHT = 480

SYMWIDTH = 80
SYMHEIGHT = 60

pWidth = int(PIXWIDTH / SYMWIDTH)
pHeight = int(PIXHEIGHT / SYMHEIGHT)

pixmap = '   ..,,""||iiIIOO##00YYXX@@'
maplen = int(len(pixmap) / 2) * 2

cap = cv2.VideoCapture(0)
cap.set(3, PIXWIDTH)
cap.set(4, PIXHEIGHT)
a = ''
while (True):
    file1 = open("cam.txt", "w")
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    screen = list()
    for j in range(SYMHEIGHT - 1):
        screen.append(list())
        y1 = j * pHeight
        y2 = y1 + pHeight
        for i in range(SYMWIDTH - 1):
            x1 = i * pWidth
            x2 = x1 + pWidth
            piece = gray[y1:y2, x1:x2]
            value = int(piece.mean() * (maplen / 256))
            screen[-1].append(pixmap[value:value + 2])

    for row in screen:
        a += ''.join(row)
    file1.write(a)
    file1.close()
    system("./client.o 40.113.19.1 8080")
    # system('cls')
    # system("pause")
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
