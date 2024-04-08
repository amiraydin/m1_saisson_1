import sys
# caution: path[0] is reserved for script path (or '' in REPL)
sys.path.insert(1, 'db')

from db2 import *


import tkinter as tk    
from tkinter import *
# from tkinter import ttk
from tkinter import filedialog
from tkinter.messagebox import showinfo
from tkinter import messagebox
from train_and_recognition import *
from win_camera import *

database = r"C:/Users/Amir/Desktop/paris8/Robotique/db/myDB.db"
db_fenetre_open = False
db_fenetre_destroy = True

creer_individu_fenetre_open = False
creer_individu_fenetre_destroy = True

def uploadImagesIndividu(list_box, list_name):
    is_empty = (not list_box.curselection())

    if(not is_empty):
        select = list_box.curselection()
        individu = list_name[select[0]]
        
        filename = filedialog.askopenfilenames(initialdir = "C:/Users/Amir/Desktop/paris8/Robotique",
                                                title = "Select a File",
                                                filetypes = (("images files",("*.jpg", "*.png", "*.jpeg")),("all files","*.*"))
        )
        
        if not filename:
            return 0
        name = list_box.get(select)
        plk_path = encode_images_faces(name, filename)
        data = [plk_path, individu[0]]
        print(data)
        insert_into_images(database, data)
    else:
        messagebox.showerror("Erreur", "Veuillez selectionner une personne !")
        
def delete_indiv(list_box, list_name):
    is_empty = (not list_box.curselection())

    if(not is_empty):
        select = list_box.curselection()
        individu = list_name[select[0]]
        
        # name = list_box.get(select)
        id = individu[0]
        print(id)
        delete_individu(database, id)
        majListBox()
        dbFenetreFermer()
        dbInterface()
    else:
        messagebox.showerror("Erreur", "Veuillez selectionner une personne !")

def creerIndividuFenetreFermer():
    global creer_individu_fenetre, creer_individu_fenetre_open, creer_individu_fenetre_destroy
    
    creer_individu_fenetre.destroy()
    creer_individu_fenetre_open = False
    creer_individu_fenetre_destroy = True
    
def creerIndividuFenetre():
    global creer_individu_fenetre, creer_individu_fenetre_open, creer_individu_fenetre_destroy

    if creer_individu_fenetre_destroy:
        creer_individu_fenetre = tk.Tk()
        creer_individu_fenetre.title("Créer Individu")
        creer_individu_fenetre_destroy = False
        creer_individu_fenetre.protocol("WM_DELETE_WINDOW", creerIndividuFenetreFermer)
    
    if not creer_individu_fenetre_open:

        creer_individu_fenetre_open = True

        frame_input = tk.Frame(db_fenetre, highlightbackground="red", highlightthickness=2)
        frame_input.pack(pady=10)

        tk.Label(creer_individu_fenetre, text="Nom").grid(row=0)
        tk.Label(creer_individu_fenetre, text="Prenom").grid(row=1)
        
        boutton_input_nom = tk.Entry(creer_individu_fenetre)
        boutton_input_nom.grid(row=0, column=1)

        boutton_input_prenom = tk.Entry(creer_individu_fenetre)
        boutton_input_prenom.grid(row=1, column=1)


        boutton_valider = Button(creer_individu_fenetre, text="Ajouter", anchor="w", command=lambda: dbAjouterIndividu(boutton_input_nom.get(), boutton_input_prenom.get()))
        boutton_valider.grid(row=2, column=1)
    creer_individu_fenetre.deiconify()

def majListBox():
    list_box.delete(0, tk.END)
    list_name = get_invidivu_list(database)
    for id, nom, prenom in list_name:
        list_box.insert('end', f"{nom} {prenom}")

def dbFenetreFermer():
    global db_fenetre_open
    global db_fenetre
    global db_fenetre_destroy
    db_fenetre.destroy()
    db_fenetre_open = False
    db_fenetre_destroy = True

def dbInterface():
    global database, db_fenetre, db_fenetre_destroy ,db_fenetre_open, list_box

    if db_fenetre_destroy:
        db_fenetre = tk.Tk()
        db_fenetre.geometry("500x500")
        db_fenetre_destroy = False
        db_fenetre.protocol("WM_DELETE_WINDOW", dbFenetreFermer)


    if not db_fenetre_open:

        db_fenetre_open = True
        frame_bouttons = tk.Frame(db_fenetre, highlightbackground="blue", highlightthickness=2)
        frame_bouttons.pack(pady=10)

        frame_list_box = tk.Frame(db_fenetre, highlightbackground="red", highlightthickness=2)
        frame_list_box.pack(pady=10)

        # db_label = Label(db_fenetre, text="Base de données")
        # default_button_grid = {"padx": 10, "pady": 10, "sticky": "ew"}
        # default_list_box = {"padx": 10, "pady": 10, "sticky": "ns"}

        list_box = tk.Listbox(frame_list_box)

        list_name = get_invidivu_list(database)
        for id, nom, prenom in list_name:
            list_box.insert('end', f"{nom} {prenom}")
        list_box.pack(pady=10)
        print(list_name)
    
        # affichage des buttons dans interface database 
        boutton_creer_individu = Button(frame_bouttons, text="Ajouter Individu", anchor="w" , command= lambda: creerIndividuFenetre())
        boutton_creer_individu.pack(side=tk.LEFT, padx=5)
        boutton_upload_image = Button(frame_bouttons, text="Upload Images", anchor="w",  command= lambda: uploadImagesIndividu(list_box, list_name))
        boutton_upload_image.pack(side=tk.LEFT, padx=5)

        boutton_image_encoder = Button(frame_bouttons, text="Encodage", anchor="w", command= lambda: encode_tout())
        boutton_image_encoder.pack(side=tk.LEFT, padx=5)
        boutton_supprimer = Button(frame_bouttons, text="Supprimer", anchor="w", command= lambda: delete_indiv(list_box, list_name))
        boutton_supprimer.pack(side=tk.LEFT, padx=5)

    db_fenetre.deiconify()

def dbAjouterIndividu(nom, prenom):
    
    if not nom or not prenom:
        messagebox.showerror("Erreur", "Les champs ne peuvent pas etre vides !")
    elif not nom.isalpha() or not prenom.isalpha():
        messagebox.showerror("Erreur", "Les champs doivent contenir uniquement des lettres de l'alphabet !")
    else:
        insert_into_individu(database, nom, prenom)
        majListBox()
        creer_individu_fenetre.destroy()
        dbFenetreFermer()
        dbInterface()

# def encoderTout():
# creation_DB(database)

fenetre = Tk()
fenetre.geometry("500x200")

boutton_db = Button(fenetre, text="Base de données", command=lambda: dbInterface())
boutton_db.place(x=50,y=50)

button_exit = Button(fenetre, text = "Exit", command = exit)
button_exit.place(x=50,y=140)

boutton_camera = Button(fenetre, text="Camera", command=rec_fac)
boutton_camera.place(x=50,y=80)
fenetre.mainloop()