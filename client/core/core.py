# coding: utf8
import sys
import zmq
import json
import pygame
from time import sleep

from network.client import Client
from gui.init import GUI
from core.ia import IA


class EnergyCell():

	def __init__(self):
		self.x = -1
		self.y = -1
		self.energy = 0

	def __repr__(self):
		return "[%s, %s] : %s" % (self.x, self.y, self.energy)


class Player():

	def __init__(self):
		self.name = None
		self.img = None
		self.x = -1
		self.y = -1
		self.energy = -1
		self.maxEnergy = 100
		self.stunned = 0
		self.turn_over = 0
		self.looking = 0
		self.actionpoint = 0

	def update_from_json(self, json):
		self.name = json["name"]
		self.x = json["x"]
		self.y = json["y"]
		self.energy = json["energy"]
		self.looking = json["looking"]
		self.actionpoint = json["actionpoint"] / 2.0
		self.stunned = json["stunned"]

	def __repr__(self):
		return "name : %s, x : %s, y : %s, energy: %s, stunned: %s, turn_over: %s, looking: %s, actionpoint: %s" % (self.name, self.x, self.y, self.energy, self.stunned, self.turn_over, self.looking, self.actionpoint)


class Softwar():

	def __init__(self):
		self.map_size = 5
		self.game_status = 0
		self.me = Player()
		self.ia = IA(self.me, self)
		self.enemies = []
		self.energy_cells = []
		self.networking = Client(sys.argv[1], sys.argv[2], sys.argv[3], self.me)
		self.gui = GUI("Softwar client", (1024, 1024), self.networking)

	def update_game_from_notification(self, notification):
		print("[NOTIFIACTION] RECIEVE : \n", json.dumps(notification, indent=4, sort_keys=True))
		if notification["notification_type"] == 0:
			data = notification["data"]
			for player in data["players"]:
				self.update_player_from_game_status(player)
			self.energy_cells = []
			for energy_cell in data["energy_cells"]:
				self.update_energy_cell_from_game_status(energy_cell)
			self.map_size = data["map_size"]
			self.game_status = data["game_status"]
			if self.me.energy > 0 and self.game_status == 1:
				self.ia.main_ia()

		elif notification["notification_type"] == 2:
			print("[GAME] FIN DE LA PARTIE")
			self.game_status = 2
			# pygame.quit()
			# sys.exit(0)
		elif notification["notification_type"] == 3:
			self.game_status = 2
		elif notification["notification_type"] == 4:
			self.game_status = 2
		elif notification["notification_type"] == 5:
			print("[GAME] NOUVEAU JOUEUR CONNECTÉ")
			data = notification["data"]
			self.map_size = data["map_size"]
			self.add_new_connected_player(data["players"])

	def add_new_connected_player(self, players):
		self.enemies = []
		# print("HANDLING NEW PLAYERS NOTIFIACTION : ", players)
		for player in players:
			# print("HANDLING JSON PLAYER : %s" % (player))
			player["name"] = bytes(player["name"], encoding="ascii")
			if player["name"] == self.me.name:
				self.me.update_from_json(player)
			else:
			 	self.enemies.append(Player())
			 	self.enemies[-1].update_from_json(player)
			self.gui.init_players(self)
		# print("ENEMIES : ", self.enemies)

	def update_player_from_game_status(self, player):
		if not player:
			return
		player_to_update = None
		player["name"] = bytes(player["name"], encoding="ascii")
		if player["name"] == self.me.name:
			player_to_update = self.me
		else:
			# If game not started, if it's the first game_status notification
			if self.game_status == 0:
				# select a enemi in self.enemies that havn't been initialized
				# print("self.enemies : %s" % (self.enemies))
				# player_to_update = [enemi for enemi in self.enemies if not enemi.name][0]
				player_to_update = self.enemies[-1]
			else:
				# select the enemi with the same name
				player_to_update = [enemi for enemi in self.enemies if enemi.name == player["name"]][0]
		player_to_update.update_from_json(player)

	def update_energy_cell_from_game_status(self, json_energy_cell):
		energy_cell = EnergyCell()
		energy_cell.x = json_energy_cell["x"]
		energy_cell.y = json_energy_cell["y"]
		energy_cell.energy = json_energy_cell["energy"]
		self.energy_cells.append(energy_cell)

	def connection(self):
		# Afficher la GUI de loading
		try:
			self.networking.identify()
			self.me.name = self.networking.id
		except Exception as e:
			return False
		return True

	def wait_game_start(self):
		# Waiting for game start notification
		notification = None
		running = True
		while (notification is None or notification["notification_type"] != 1) and running:
			socks = dict(self.networking.poller.poll(0))
			if socks.get(self.networking.sub_skt) == zmq.POLLIN:
				notification = self.networking.recieve_notification()
				self.update_game_from_notification(notification)
				self.gui.init_map(self)
				self.gui.display(self)
			running = self.gui.handle_events()
		self.game_status = 1
		# Todo : init game gui

	def disconnect(self):
		pass

	def run(self):
		if not self.connection():
			print("[NETWORK] IMPOSSIBLE DE SE CONNECTER AU SERVEUR")
			return
		self.gui.init_map(self)
		self.wait_game_start()
		print("[NETWORK] LA PARTIE SE LANCE...")
		self.gui.display(self)
		running = True
		sleep(1);
		while running:
			socks = dict(self.networking.poller.poll(0))
			if socks.get(self.networking.sub_skt) == zmq.POLLIN:
				notification = self.networking.recieve_notification()
				self.update_game_from_notification(notification)
				self.gui.display(self)
			running = self.gui.handle_events()
			# TODO: IA
		self.disconnect()
	def __str__(self):
		return "map_size : %s, game_status: %s, enemies: %s, me: %s" % (self.map_size, self.game_status, self.enemies, self.me)
