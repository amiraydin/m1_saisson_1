
import cv2 
import face_recognition

fc = cv2.CascadeClassifier('face_cascade.xml')

def face_enc(image_path):
    img1 = face_recognition.load_image_file(image_path)
    face_enc = face_recognition.face_encodings(img1)[0]
    return face_enc

# face_1 = ffe('amir.jpg')
cap = cv2.VideoCapture(0)
my_pic = face_enc('./images/amir.jpg')

while True : 
    _, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = fc.detectMultiScale(gray, 1.1, 4)
    b = 255
    v = 0
    cv2.imwrite('./images/fram.jpg', img)
    encode = cv2.imread('./images/fram.jpg')
    face_cam = face_recognition.face_encodings(encode)[0]
    is_same = face_recognition.compare_faces([my_pic], face_cam)

    if is_same :
        b = 0
        v = 255
        print("Hello !")
    else :
        b = 255
        v = 0
    # else :
    #     continue

    for (x, y, w, h) in faces : 
        cv2.rectangle(img, (x, y), (x + w, y + h), (b, v, 0), 2)
    cv2.imshow('img', img)
    k = cv2.waitKey(30) & 0xff
    if (k == 27) : 
        break

cap.release()


# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# faces = fc.detectMultiScale(gray, 1.1, 4)

# for (x, y, w, h) in faces : 
#     cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

# scale = 30
# width = int(img.shape[1] * scale / 100)
# height = int(img.shape[0] * scale / 100)
# dim = (width, height)
# resized = cv2.resize(img, dim, interpolation= cv2.INTER_AREA)
# cv2.imshow('img', resized)
# cv2.waitKey()

