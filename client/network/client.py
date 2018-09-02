# coding: utf8
import zmq
import sys
import ipaddress
from random import randint
import json

class Client():
    def __init__(self, ip, portReq, portSub,player):
        self.req_skt = None
        self.sub_skt = None
        self.poller = None
        self.id = None
        self.energy = 50
        self.player = player

        self._validate_ip(ip)
        self._validate_port(portReq)
        self._validate_port(portSub)
        self.context = zmq.Context()
        self._open_req_skt(ip, portReq)
        self._open_sub_skt(ip, portSub)
        self._init_poller()

    def _init_poller(self):
        self.poller = zmq.Poller()
        # self.poller.register(self.req_skt, zmq.POLLIN)
        self.poller.register(self.sub_skt, zmq.POLLIN)

    def _open_req_skt(self, ip, port):
        print ("[NETWORK] Connexion au serveur... (%s:%s)" % (ip,port))
        self.req_skt = self.context.socket(zmq.REQ)
        self.req_skt.connect("tcp://" + ip + ":" + port)
        self.req_skt.RCVTIMEO = 1000
        print("[NETWORK] Connecté au router")

    def _open_sub_skt(self, ip, port):
        print ("[NETWORK] Connexion au serveur... (%s:%s)" % (ip,port))
        self.sub_skt = self.context.socket(zmq.SUB)
        self.sub_skt.setsockopt_string(zmq.SUBSCRIBE, "#global")
        self.sub_skt.connect("tcp://" + ip + ":" + port)
        print("[NETWORK] Connecté au publisher")

    def attack(self):
        response = self.send_command(b"attack")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Attack success')
            else:
                print('[COMMAND] Attack failed')

    def forward(self):
        response = self.send_command(b"forward")
        splitedResponse = self._parse_reponse(response)
        cost = 0
        if splitedResponse:
            self.player.actionpoint -= 0.5 if self.player.actionpoint > 0 else 0
            if splitedResponse[0] == "ok":
                print('[COMMAND] Forward success')
            else:
                print('[COMMAND] Forward failed')

    def backward(self):
        response = self.send_command(b"backward")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Backward success')
            else:
                print('[COMMAND] Backward failed')

    def leftfwd(self):
        response = self.send_command(b"leftfwd")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Leftfwd success')
            else:
                print('[COMMAND] Leftfwd failed')

    def rightfwd(self):
        response = self.send_command(b"rightfwd")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Rightfwd success')
            else:
                print('[COMMAND] Rightfwd failed')

    def left(self):
        response = self.send_command(b"left")
        splitedResponse = self._parse_reponse(response)
        cost = 0
        if splitedResponse:
            self.player.actionpoint -= 0.5 if self.player.actionpoint > 0 else 0
            if splitedResponse[0] == "ok":
                print('[COMMAND] Left success')
            else:
                print('[COMMAND] Left failed')


    def right(self):
        response = self.send_command(b"right")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            self.player.actionpoint -= 0.5 if self.player.actionpoint > 0 else 0
            if splitedResponse[0] == "ok":
                print('[COMMAND] Right success')
            else:
                print('[COMMAND] Right failed')


    def looking(self):
        response = self.send_command(b"looking")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Looking success')
            else:
                print('[COMMAND] Looking failed')

    def gather(self):
        response = self.send_command(b"gather")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Gather success')
                self.player.actionpoint = 0
            else:
                print('[COMMAND] Gather failed')

    def selfid(self):
        response = self.send_command(b"selfid")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Selfid success')
                # TODO: PYGAME
            else:
                print('[COMMAND] Selfid failed')
                # TODO: PRINT ERROR IN PYGAME

    def selfstats(self):
        response = self.send_command(b"selfstats")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Selfstats success')
                # TODO: PYGAME
            else:
                print('[COMMAND] Selfstats failed')
                # TODO: PRINT ERROR IN PYGAME

    def inspect(self, idToInspect):
        response = self.send_command(b"right", idToInspect)
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Right success')
                # TODO: PYGAME
            else:
                print('[COMMAND] Right failed')
                # TODO: PRINT ERROR IN PYGAME

    def next(self):
        response = self.send_command(b"next")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Next success')
                # TODO: PYGAME
            else:
                print('[COMMAND] Next failed')
                # TODO: PRINT ERROR IN PYGAME

    def watch(self):
        response = self.send_command(b"watch")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Watch success')
                # TODO: PYGAME
                return splitedResponse[1]
            else:
                print('[COMMAND] Watch failed')
                # TODO: PRINT ERROR IN PYGAME
        return None

    def jump(self):
        response = self.send_command(b"jump")
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                print('[COMMAND] Jump success')
                # TODO: PYGAME
            else:
                print('[COMMAND] Jump failed')
                # TODO: PRINT ERROR IN PYGAME

    def identify(self):
        if self.id:
            print('Identity already set')
            return
        randomId = bytes("#0x" + str(randint(0, 99)), encoding="ascii");
        response = self.send_command(b"identify", randomId)
        splitedResponse = self._parse_reponse(response)
        if splitedResponse:
            if splitedResponse[0] == "ok":
                self.id = randomId
                print('[COMMAND] Identify ok (%s)' % self.id)
            else:
                print('[COMMAND] Identify failed')
                self.identify()
        else:
            raise Exception("Can't identify ourself")
                # TODO: PRINT ERROR IN PYGAME

    def send_command(self, command, data=None):
        req = command
        if data:
            req = command + b"|" + data
        print("[COMMAND] Sending : ", command)
        try:
            self.req_skt.send(req)
            response = ""
            response = self.req_skt.recv_string()
            print("[NETWOKR] Received reply " + "[" + response + "]")
            return response
        except Exception as e:
            print('[ERREUR] : ', e)
        return ""

    def _parse_reponse(self, response):
        if not response:
            return None;
        else:
            splitedResponse = response.split('|')
            if len(splitedResponse) == 2:
                return splitedResponse
            else:
                print("No data in response")
                return None

    def _validate_ip(self, ip):
        ipaddress.ip_address(ip)

    def _validate_port(self, port):
        if type(port) is int:
            return True
        elif type(port) is str and not port.isdigit():
            raise ValueError("Le port specifié est incorrect")

    def recieve_notification(self):
        notification = self.sub_skt.recv().decode('utf8').replace("'",'"')
        # print("RECVED notification", notification)
        notification = notification.split('|')[1]
        notification = json.loads(notification)
        # print("RETURNING NOTIFICATION : ", notification)
        return notification
