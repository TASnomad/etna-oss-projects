from threading import Thread
import zmq
import random
import json

class Client :
    def __init__(self, reqIp, pubIp) :
        self.ctx = zmq.Context()
        self.repTimeout = 2500
        self.repSock = self.ctx.socket(zmq.REQ)
        self.repSock.setsockopt(zmq.RCVTIMEO, 2500)
        self.pubSock = self.ctx.socket(zmq.SUB)
        self.reqIp = reqIp
        self.pubIp = pubIp
        self.id = "#0x0"
        self.pubThread = Thread(target = self.getPub)

        self.identify()
        self.initPub()
        self.pubThread.start()

    def identify(self) :
        rand_id = random.randint(1, 9)
        self.id = self.id + str(rand_id)
        cmd = 'identify|' + self.id
        self.repSock.setsockopt(zmq.IDENTITY, self.id)
        print 'Connection attempt with ID: ' + self.repSock.getsockopt(zmq.IDENTITY)
        self.repSock.connect("tcp://" + self.reqIp)
        try:
            self.repSock.send(cmd)
        except zmq.Again as e:
            self.id = ""
            self.repSock.close()
            self.identify()

        response = self.repSock.recv()
        print "Connection status: " + response
        if response.startswith('ok|', 0) :
            print "Connected with ID: " + self.id
            return 0
        else :
            print "Trying a new ID"
            self.id = ""
            self.repSock.close()
            self.identify()

    def initPub(self) :
        self.pubSock.connect("tcp://" + self.pubIp)
        self.pubSock.setsockopt(zmq.SUBSCRIBE, "Global")
        self.pubSock.setsockopt(zmq.SUBSCRIBE, self.id)

    def cmd(self, command):
        print command
        msg = command
        self.repSock.send(msg)
        response = self.repSock.recv()

        return response

    def getPub(self):
        print "Waiting Pub notifications"
        for i in range(0, 5) :
            message = self.pubSock.recv_multipart()
            print("Forward check: " + self.cmd("forward|null"))
            print("Backward check: " + self.cmd("backward|null"))
            print("Watch check: " + self.cmd("watch|null"))
            notif = json.loads(message.pop()) # JSON message is always the last element
            print "Notification:\n "
            print notif["notification_type"]
            print json.dumps(notif)
            #print("Look check: " + self.cmd("looking|null"))




def main():
    player = Client("localhost:4242", "localhost:4243")

    #pub_thread = Thread(target = getPub, args = (pubSock, ))

    #repSock.setsockopt(zmq.RCVTIMEO, 2500)
    #pubSock.connect("tcp://localhost:4243")
    commands_to_test = [ "selfid|null", "selfstats|null", "commands|null" ]#, "looking|null" ]

    #pub_thread.start()

    for i in commands_to_test :
        print("Sending: {} command".format(i))
        msg = player.cmd(i)
        print("Received: {}".format(msg))
    exit

main()