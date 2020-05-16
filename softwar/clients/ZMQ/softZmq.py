#-*- coding: utf-8 -*-
import zmq

class softZmq(object) :
    def __init__(self, ip, portRepSock, portPubSock) :
        #contexte zmq
        self.context        = zmq.Context()
        
        #definition et reglage des sockets
        self.repSock        = self.context.socket(zmq.REQ)
        self.pubSock        = self.context.socket(zmq.SUB)

        self.repSock.setsockopt(zmq.RCVTIMEO, 2500)
        self.pubSock.setsockopt(zmq.SUBSCRIBE, "Global")
        
        #stockage ip et ports 
        self.ip             = ip
        self.portRepSock    = portRepSock
        self.portPubSock    = portPubSock

        #preparation des sockets
        self.repSock = self.prepareRepSock()
        if self.repSock != False :
            self.preparePubSock()

    #le pub sock contient le jeu et recois le jeu
    def preparePubSock(self) :
        self.pubSock.connect("tcp://" + self.ip + ":" + self.portPubSock)
        
    def prepareRepSock(self, id = 1) :
        #si trop aurevoir
        if id > 4 :
            self.status = "Connection rejected : the game is full"
            return False
        try :
            #else on tente de se connecter
            base_name = "0x0"
            full_id = base_name + str(id)
            identity = 'identify|' + full_id
            #on lui dit qu'on est la pour se connecter
            self.repSock.setsockopt(zmq.IDENTITY, full_id)
            #then on se connecte et on balance
        
            self.repSock.connect("tcp://" + self.ip + ":" + self.portRepSock)
            self.repSock.send(identity)
            
            #wait de la reponse
            msg = self.repSock.recv()
            print(msg)

            #si on peut jouer on quitte sinon on retente
            if msg.startswith('ok|', 0):
                self.status = "connected"
                self.id = self.repSock.getsockopt(zmq.IDENTITY)
                return self.repSock
            else:
                return self.prepareRepSock(int(id) + 1)
        except Exception, e:
            self.status = str(e)
            return False

    #envoyer une requete au worker
    def askServer(self, str) :
        self.repSock.send(str)
        res = self.repSock.recv()
        return res
