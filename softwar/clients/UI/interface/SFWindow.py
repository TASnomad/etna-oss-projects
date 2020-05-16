#-*- coding: utf-8 -*-
import time
import json
from threading import Thread
from Tkinter import *
from tkinter import messagebox
import sys
sys.path.append('...')

from AI.AI import *
from ZMQ.softZmq import *


#classe qui gère l'affichage du jeu et le jeu en lui même
class SFWindow(object) :
    def __init__(self):
        #principal window
        self.window = Tk()
        self.window.tk_setPalette(background = "#FFE082", foreground = "#3F51B5", activebackground = "#FFF9C4", activeforeground = "white")
        self.window.geometry("800x600")
        self.window.title("Softwar")
        self.window.resizable(False, False)
        self.window.protocol("WM_DELETE_WINDOW", self.on_closing)
        


        #datas = '{"notification_type": 0, "data": {"game_status": 1, "players": [{"y": 0, "x": 0, "energy": 48, "looking": 0, "id": "0x01"}, {"y": 0, "x": 4, "energy": 48, "looking": 1, "id": "0x02"}, {"y": 4, "x": 0, "energy": 48, "looking": 2, "id": "0x03"}, {"y": 4, "x": 4, "energy": 48, "looking": 3, "id": "0x04"}], "map_size": 5, "energy_cells": [{"y": 1, "x": 0, "value": 15}, {"y": 2, "x": 4, "value": 15}, {"y": 5, "x": 3, "value": 15}]}}'

        self.mapSize = 5

        self.items = {}

        #window base txt
        key = 'main_text'
        self.items.update({key : Label(self.window, text="S O F T W A R")})
        self.items[key].config(font = ('Arial',"75", "bold"))
        self.items[key].place(relx=0.5, rely=0.2, anchor=CENTER)

        self.zmq = False
        self.pubThread = False
        print (self.zmq)

        #self.displayGame(json.loads(datas))
        self.displayFormConnection()

        self.window.mainloop()

    #Affichage du formulaire de connection au serveur
    def displayFormConnection(self) :
        #champs du formulaire :

        #-- IP
        key = 'ip_input_label'
        self.items.update({key : Label(self.window, text="Server IP :")})
        self.items[key].config(font = ('Arial',"12"))
        self.items[key].place(relx=0.3, rely=0.35, anchor=CENTER)

        key = 'ip_input_field'
        self.items.update({key : Entry(self.window)})
        self.items[key].place(relx=0.5, rely=0.35, anchor=CENTER)

        #-- PORT REP
        key = 'port_rep_input_label'
        self.items.update({key : Label(self.window, text="Server's REP Port :")})
        self.items[key].config(font = ('Arial',"12"))
        self.items[key].place(relx=0.3, rely=0.4, anchor=CENTER)

        key = 'port_rep_input_field'
        self.items.update({ key : Entry(self.window)})
        self.items[key].place(relx=0.5, rely=0.4, anchor=CENTER)

        #-- PORT PUB
        key = 'port_pub_input_label'
        self.items.update({key : Label(self.window, text="Server's PUB Port :")})
        self.items[key].config(font = ('Arial',"12"))
        self.items[key].place(relx=0.3, rely=0.45, anchor=CENTER)

        key = 'port_pub_input_field'
        self.items.update({ key : Entry(self.window)})
        self.items[key].place(relx=0.5, rely=0.45, anchor=CENTER)


        #-- BUTTON
        key = 'form_connection_button'
        self.items.update({ key : Button(self.window, text = "Submit", bg = "#3F51B5", fg = "white", activebackground = "#B39DDB", command = self.launchConnection)})
        self.items[key].place(relx=0.5, rely=0.5, anchor=CENTER)

        self.window.update()

    #Connection au server : si c'est Bon on se connecte sinon on affiche l'erreur et on relance le processus
    def launchConnection(self):
        #recupération du formulaire
        self.ip = self.items['ip_input_field'].get()
        self.rep_port = self.items['port_rep_input_field'].get()
        self.pub_port = self.items['port_pub_input_field'].get()

        #suppresion des champs du formulaire 
        keys_to_delete = ['ip_input_field', 'port_rep_input_field', 'port_pub_input_field', 'form_connection_button']
        for key in keys_to_delete :
            self.removeOneItem(key)

        #affichage des valeurs entrées dans le formulaire 
        key = 'ip_display'
        self.items.update({key : Label(self.window, text = str(self.ip))})
        self.items[key].config(font = ('Arial',"12", "bold"))
        self.items[key].place(relx=0.5, rely=0.35, anchor=CENTER)

        key = 'port_rep_display'
        self.items.update({key : Label(self.window, text = str(self.rep_port))})
        self.items[key].config(font = ('Arial',"12", "bold"))
        self.items[key].place(relx=0.5, rely=0.4, anchor=CENTER)

        key = 'port_pub_display'
        self.items.update({key : Label(self.window, text = str(self.pub_port))})
        self.items[key].config(font = ('Arial',"12", "bold"))
        self.items[key].place(relx=0.5, rely=0.45, anchor=CENTER)

        #label du statut de  la connexion
        key = 'connection_status'
        self.items.update({key : Label(self.window, text="Trying to connect to " + str(self.ip), fg = "#E91E63")})
        self.items[key].config(font = ('Arial',"12"))
        self.items[key].place(relx=0.5, rely=0.8, anchor=CENTER)

        self.window.update()

        if self.connectToServer() == False :
            time.sleep(3)
            #reset de la fenetre du formulaire
            keys_to_delete = ['connection_status', 'connection_status_final', 'ip_input_label', 'port_rep_input_label', 'port_pub_input_label', 'ip_display','port_rep_display', 'port_pub_display']
            for key in keys_to_delete :
                self.removeOneItem(key)
            self.displayFormConnection()
        else :
            self.playGame()

    #Contrôle de zmq
    def connectToServer(self) :

        #instanciation de softZmq
        self.zmq = softZmq(self.ip, self.rep_port, self.pub_port)

        #affichage du statut de la connection
        key = 'connection_status_final'
        text_status = "Connection status : " + self.zmq.status
        self.items.update({key : Label(self.window, text = text_status)})
        self.items[key].config(font = ('Arial',"12"))
        self.items[key].place(relx=0.5, rely=0.9, anchor=CENTER)
        
        self.window.update()

        #Gestion du résulat de la connection
        if self.zmq.status == "connected" :
            return True
        else :
            if self.zmq.repSock != False :
                self.zmq.repSock.close()

            if self.zmq.pubSock != False :
                self.zmq.pubSock.close()
            
            if self.zmq.context != False :
                self.zmq.context.destroy()
            return False
        
    #Ajout d'un élément à la fenêtre
    def AddItemToWindow(self, key, item, x, y) :
        self.items.update({key : item})
        self.items[key].place(relx = x, rely = y, anchor = CENTER)
        self.window.update()

    #cette fonction vide la liste items qui contient tous les éléments à afficher
    def removeAllItems(self) :
        for (key, item) in self.items.items() :
            self.items[key].place_forget()
            del self.items[key]

    def removeOneItem(self, key) :
        self.items[key].place_forget()
        del self.items[key]

    def receiveDatas(self) :

        try :
            message = self.zmq.pubSock.recv_multipart()

            notification = message.pop()
            #actions = self.ai.chooseActions(notification)

            notification = json.loads(notification)
            self.mapSize = notification['data']['map_size']
            self.displayGame(notification)

            #for action in actions :
            #    txt = self.zmq.askServer(action)
            #    print (txt)
            return True
        except Exception, e :
            raise

    def ctrlThread(self) :
        i = 0
        varTest = True
        while i < 26 :
            print (i)
            varTest = self.receiveDatas()

            if varTest == False :
                return False
            i += 1

    def getPtsForPlayer(self, player) :
        plY = player['y']
        plX = player['x']

        Tx = self.canvasWidth / self.mapSize
        Ty = self.canvasHeight / self.mapSize
        
        #disposition de tous les points
        # 1 # 2 # 3
        # 8     # 4
        # 7 # 6 # 5

        #1
        p1X = Tx * 0.1 + plX * Tx
        p1Y = Ty * 0.1 + plY * Ty

        #3
        p3X = Tx * 0.9 + plX * Tx
        p3Y = Ty * 0.1 + plY * Ty

        #5
        p5X = Tx * 0.9 + plX * Tx
        p5Y = Ty * 0.9 + plY * Ty

        #7
        p7X = Tx * 0.1 + plX * Tx
        p7Y = Ty * 0.9 + plY * Ty

        #left
        if player['looking'] == 0 :
            #8
            p8X = Tx * 0.1 + plX * Tx
            p8Y = Ty * 0.5 + plY * Ty
            return [(p3X, p3Y), (p5X, p5Y), (p8X, p8Y)]
        #top
        elif player['looking'] == 1 :
            #2
            p2X = Tx * 0.5 + plX * Tx
            p2Y = Ty * 0.1 + plY * Ty
            return [(p2X, p2Y), (p5X, p5Y), (p7X, p7Y)]
        #right
        elif player['looking'] == 2 :
            #4
            p4X = Tx * 0.9 + plX * Tx
            p4Y = Ty * 0.5 + plY * Ty
            return [(p1X, p1Y), (p4X, p4Y), (p7X, p7Y)]
        #bottom
        elif player['looking'] == 3 :
            #6
            p6X = Tx * 0.5 + plX * Tx
            p6Y = Ty * 0.9 + plY * Ty
            return [(p1X, p1Y), (p3X, p3Y), (p6X, p6Y)]
    
    def displayGame(self, datas) :

        key = 'canvas'
        for (itemKey, item) in self.items.items() :
            if itemKey != key :
                self.removeOneItem(itemKey)

        self.items[key].delete("all")
        i = 0
        Tx = self.canvasWidth / self.mapSize
        Ty = self.canvasHeight / self.mapSize

        playerColors = ['#FF5722', '#FFEB3B', '#9C27B0', '#8BC34A']

        #look memo : left = 0, top = 1, right = 2, bottom = 3
        for player in datas['data']['players'] :
            pts = self.getPtsForPlayer(player)
            self.items[key].create_polygon(pts, fill=playerColors[i])
            i += 1
        for energyCell in datas['data']['energy_cells'] :
            clX = energyCell['x']
            clY = energyCell['y']
            
            p1X = Tx * 0.1 + clX * Tx
            p1Y = Ty * 0.1 + clY * Ty
            p2X = Tx * 0.9 + clX * Tx
            p2Y = Ty * 0.9 + clY * Ty

            pts = [(p1X, p1Y), (p2X, p2Y)]
            self.items[key].create_oval(pts, fill="#CDDC39", outline="")

        i = 0
        while i < self.mapSize :
            line_width = 2
            line_color = "#29B6F6"
            y_fix = 0 + i * Ty
            self.items[key].create_line(0, y_fix, self.canvasWidth, y_fix, width = line_width, fill = line_color)
            x_fix = 0 + i * Tx
            self.items[key].create_line(x_fix, 0, x_fix, self.canvasHeight, width = line_width, fill = line_color)
            i += 1
        
        #affichage des infos
        key = 'info_panel_main'
        self.items.update({key : Label(self.window, text = "INFO PANEL :")})
        self.items[key].config(font = ('Arial',"16", "bold"))
        self.items[key].place(relx=0.9, rely=0.05, anchor=CENTER)

        key = 'inf_panel_p1'
        self.items.update({key : Label(self.window, text = "player 1", fg= playerColors[0])})
        self.items[key].config(font = ('Arial',"16", "bold"))
        self.items[key].place(relx=0.9, rely=0.10, anchor=CENTER)

        key = 'inf_panel_p1'
        self.items.update({key : Label(self.window, text = "player 2", fg= playerColors[1])})
        self.items[key].config(font = ('Arial',"16", "bold"))
        self.items[key].place(relx=0.9, rely=0.15, anchor=CENTER)

        key = 'inf_panel_p1'
        self.items.update({key : Label(self.window, text = "player 3", fg= playerColors[2])})
        self.items[key].config(font = ('Arial',"16", "bold"))
        self.items[key].place(relx=0.9, rely=0.20, anchor=CENTER)

        key = 'inf_panel_p1'
        self.items.update({key : Label(self.window, text = "player 4", fg= playerColors[3])})
        self.items[key].config(font = ('Arial',"16", "bold"))
        self.items[key].place(relx=0.9, rely=0.25, anchor=CENTER)
        self.window.update()
    
    def playGame(self) :
        self.removeAllItems()
        self.ai = AI(self.zmq.id)
        self.pubThread = Thread(target = self.ctrlThread)

        key = 'canvas'
        self.window.tk_setPalette(background = "#29B6F6", foreground = "white")
        self.canvasWidth = 0.8 * 800
        self.canvasHeight = 600
        self.items.update({key : Canvas(self.window, width = self.canvasWidth, height = self.canvasHeight, background = "#039BE5")})
        self.items[key].pack(side = "left")
        
        self.pubThread.start()
        print('started')
        #self.removeAllItems()
    def on_closing(self):
        if messagebox.askokcancel("Quit", "Do you want to quit?"):
            if self.zmq != False :
                if self.zmq.repSock != False :
                    self.zmq.repSock.close()

                if self.zmq.pubSock != False :
                    self.zmq.pubSock.close()
                
                if self.zmq.context != False :
                    self.zmq.context.destroy()
            self.window.destroy()