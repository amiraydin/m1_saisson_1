from pathlib import Path
from collections import Counter
import numpy as np
import os, face_recognition, pickle

DEFAULT_ENCODINGS_PATH = Path("db/encoding/encodings.pkl")

def encode_known_faces(path,
    model: str = "hog", encodings_location: Path = DEFAULT_ENCODINGS_PATH
) -> None:
    names = []
    encodings = []
    folders = os.listdir(path)
    # people = [person for person in os.listdir(path)]
    # print(people)

    for person in folders:
        name = person.replace("_", " ") if "_" in person else person
        person_path = path + "/" + person
        person_files = os.listdir(person_path)
        for img_path in person_files:
            path_name = person_path + "/" + img_path
            print(path_name)
            image = face_recognition.load_image_file(path_name)

            face_locations = face_recognition.face_locations(image, model=model)
            face_encodings = face_recognition.face_encodings(image, face_locations)
            for encoding in face_encodings:
                names.append(name)
                encodings.append(encoding)

        name_encodings = {"names": names, "encodings": encodings}
        with encodings_location.open(mode="wb") as f:
            pickle.dump(name_encodings, f)

def encode_images_faces(name, images,
    model: str = "hog") -> None:
    
    encodings = []
    names = []
    name = name.split(" ")
    name = name[0] + "_" + name[1]
    encodings_location: str = Path(f"db/encoding/liste_personne/{name}.plk") 
    print(encodings_location)
    for image_file in images:
        print(image_file)
        face_reco_image = face_recognition.load_image_file(image_file)

        face_locations = face_recognition.face_locations(face_reco_image, model=model)
        face_encodings = face_recognition.face_encodings(face_reco_image, face_locations)

        #Normalement 1 seul visage sur l'image donc pas necessaire le for
        for encoding in face_encodings:
            names.append(name)
            encodings.append(encodings)
        
    names_encodings = {"names": names, "encodings": encodings}
    for enc in names_encodings['encodings']:
        print(enc)

    with encodings_location.open(mode="wb") as f:
        pickle.dump(names_encodings, f)
    
    return encodings_location

def encode_tout(encodings_location: Path = DEFAULT_ENCODINGS_PATH,) -> None:
    path = Path("db/encoding/liste_personne")
    liste_personne = os.listdir(path)
    print(liste_personne)
    print(path)
    for personne in liste_personne:
        path_personne = str(path) + '/' + str(personne)
    with encodings_location.open(mode="rb") as f:
        loaded_encodings = pickle.load(f)
    


def _recognize_face(unknown_encoding, loaded_encodings):

    boolean_matches = face_recognition.compare_faces(loaded_encodings["encodings"], unknown_encoding)
    distance = face_recognition.face_distance(loaded_encodings["encodings"], unknown_encoding)

    list_distance = list(map(lambda x: round(x * 100), distance))
    list_accuracy = list(map((lambda x: 100 - x), list_distance))
    list_validation = list(
        map((lambda ac: True if ac >= 40 else False),list_accuracy))
    print("Boolean matches", boolean_matches)
    print("List accuracy", list_accuracy)
    print("List validation", list_validation)
    votes = Counter(    
        name
        for match, name in zip(list_validation, loaded_encodings["names"])
        if match
    )
    print("votes", votes)
    if votes:
        return votes.most_common(1)[0][0]
    

def _recognize_face_video(unknown_encoding, loaded_encodings):
    unknown_encoding = np.array(unknown_encoding)
    boolean_matches = face_recognition.compare_faces( loaded_encodings["encodings"], unknown_encoding, tolerance= 0.4)
    distance = face_recognition.face_distance(loaded_encodings["encodings"], unknown_encoding)
    
    list_distance = list(map(lambda x: round(x * 100), distance))
    list_accuracy = list(map((lambda x: 100 - x), list_distance))
    # list_validation = list(
    #     map((lambda ac: True if ac >= 50 else False),list_accuracy))
    
    votes = Counter(
        name
        for match, name in zip(boolean_matches, loaded_encodings["names"])
        if match
    )
    if votes:
        return votes.most_common(1)[0][0]


def recognize_faces(
    image_location: str,
    model: str = "hog",
    encodings_location: Path = DEFAULT_ENCODINGS_PATH,
) -> None:
    with encodings_location.open(mode="rb") as f:
        loaded_encodings = pickle.load(f)

    input_image = face_recognition.load_image_file(image_location)
    input_face_locations = face_recognition.face_locations(input_image, model=model)
    input_face_encodings = face_recognition.face_encodings(input_image, input_face_locations)

    for bounding_box, unknown_encoding in zip(input_face_locations, input_face_encodings):
        name = _recognize_face(unknown_encoding, loaded_encodings)
        print(name)
        if not name:
            name = "Unknown"
        return [name, bounding_box]


def recognize_faces_video(im,
    encodings_location: Path = DEFAULT_ENCODINGS_PATH,
) -> None:
    with encodings_location.open(mode="rb") as f:
        loaded_encodings = pickle.load(f)

    face_encodings = face_recognition.face_encodings(im)

    for unknown_encoding in zip(face_encodings):
        name = _recognize_face_video(unknown_encoding, loaded_encodings)
        if not name:
            name = "Unknown"
        return name


# Training and encode

# encode_known_faces("db/training_images")

# Test 

# name, faces = recognize_faces("C:/Users/Ni2/Documents/M1_git/robotique/db/test/asiat1.jpg")
# print(name, faces)
