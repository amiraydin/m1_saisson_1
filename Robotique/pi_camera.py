import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray
import face_recognition
# Load the cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
img1_ni2 = cv2.imread("Ni2_3.jpg")

cam = PiCamera()
cam.resolution = (512, 304)
cam.framerate = 30
rawCapture = PiRGBArray(cam, size=(512, 304))



name = "Ni2_"

photo = 0
while True:
    for frame in cam.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        image = frame.array
        
    
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        # Detect the faces
        faces = face_cascade.detectMultiScale(gray, 1.1, 4)
        # Draw the rectangle around each face
        for (x, y, w, h) in faces:
            cv2.rectangle(image, (x, y), (x+w, y+h), (255, 0, 0), 2)
            cv2.putText(image, name, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36,255,12), 2)
        # Display
        
        cv2.imshow("Press", image)
        rawCapture.truncate(0)
        
        k = cv2.waitKey(30) & 0xff
        leave = False
        
        if k == 27:
            print("Escape")
            leave = True
            break
        if k == 115:
            photo = photo + 1
            print(photo)
            photo_name = 'db/screen/' + name + str(photo) + ".jpg"
            print(photo_name)
            cv2.imwrite(photo_name, image)
    if leave:
        break
    

cv2.destroyAllWindows()