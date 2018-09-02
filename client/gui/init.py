# coding: utf8
import math
import random
import pygame

class GUI():
	def __init__(self, caption_name, resolution, network_client):
		pygame.init()
		self.network_client = network_client
		self.resolution = resolution
		self.init_window(caption_name, resolution)
		self.render()

	def init_window(self, caption_name, resolution):
		pygame.display.set_caption(caption_name)
		self.window = pygame.display.set_mode(resolution)
		bg = pygame.image.load("./ressources/img/loading.png")
		self.window.blit(bg, (0,0))		

	def init_map(self, game):
		self.cell_size = math.floor(self.resolution[0] / game.map_size)
		self.offset_size = math.floor((self.resolution[0] - (self.cell_size * game.map_size)) / 2)
		self.cell_img = []
		for i in range(4):
			img = pygame.image.load('./ressources/img/cell'+ str(i+1) +'.png')
			img = pygame.transform.scale(img, (self.cell_size, self.cell_size))
			self.cell_img.append(img)
		self.cell_map = [[random.randint(0, 3) for i in range(game.map_size)] for j in range(game.map_size)]

		players = game.enemies.copy()
		players.append(game.me)
		for i, player in enumerate(players):
			img = pygame.image.load('./ressources/img/character'+str(i+1) + '.png')
			img = pygame.transform.scale(img, (math.floor(self.cell_size * 0.6), math.floor(self.cell_size * 0.6)))
			player.img = img

		self.info_bar_img = pygame.image.load("./ressources/img/infobar.png")
		ratio = self.cell_size / 400 
		self.info_bar_img = pygame.transform.scale(self.info_bar_img, (math.floor(400 * ratio), math.floor(100 * ratio)))
		self.info_bar_heart_pos = (100 * ratio, 15 * ratio)

		self.name_font = pygame.font.SysFont("monospace", math.floor(40 / (game.map_size / 5)))
		self.info_bar_font = pygame.font.SysFont("monospace", math.floor(40/ (game.map_size / 5)))

		self.energy_cell_img = pygame.image.load('./ressources/img/energycell.png')
		self.energy_cell_img = pygame.transform.scale(self.energy_cell_img, (self.cell_size, self.cell_size))

		self.dead_img = pygame.image.load("./ressources/img/dead.png")
		self.stunned_img = pygame.image.load("./ressources/img/stunned.png")
		self.stunned_img = pygame.transform.scale(self.stunned_img, (math.floor(self.cell_size * 0.6), math.floor(self.cell_size * 0.6)))

	def init_players(self, game):
		players = game.enemies.copy()
		players.append(game.me)
		for i, player in enumerate(players):
			if player.img is None:
				img = pygame.image.load('./ressources/img/character'+str(i+1) + '.png')
				img = pygame.transform.scale(img, (math.floor(self.cell_size * 0.6), math.floor(self.cell_size * 0.6)))
				player.img = img

	def display(self, game):
		self.display_map(game)
		self.display_energy_cells(game)
		self.display_players(game)
		if game.game_status == 2:
			self.end_game(game.me.energy > 0)
		if game.me.energy <= 0 and game.game_status != 0:
			self.display_dead()
		self.render()

	def display_map(self, game):
		for x in range(game.map_size):
			for y in range(game.map_size):
				cell_pos = [self.offset_size + (self.cell_size * x), self.offset_size + (self.cell_size * y)]
				# print("TRYING TO DISPLAY POS (%s, %s) WHICH IS %s" % (x, y, self.cell_map[x][y]))
				img = self.cell_img[self.cell_map[x][y]]
				self.window.blit(img, cell_pos)

	def display_players(self, game):
		players = game.enemies.copy()
		players.append(game.me)
		players = (player for player in players if player.energy > 0)
		for player in players:
			player_pos = [self.offset_size + (self.cell_size * player.x), self.offset_size + (self.cell_size * player.y)]
			player_pos[0] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
			player_pos[1] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
			if player.looking == 0:
				img = pygame.transform.flip(player.img, 1, 0)
				stunned_img = pygame.transform.flip(self.stunned_img, 1, 0)
			elif player.looking == 1:
				img = player.img
				stunned_img = self.stunned_img
			elif player.looking == 2:
				img = player.img
				stunned_img = self.stunned_img
			elif player.looking == 3:
				img = pygame.transform.flip(player.img, 0, 1)
				stunned_img = pygame.transform.flip(self.stunned_img, 0, 1)
			self.window.blit(img, player_pos)
			if player.stunned > 0:
				stunned_pos = [self.offset_size + (self.cell_size * player.x), self.offset_size + (self.cell_size * player.y)]
				stunned_pos[0] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
				stunned_pos[1] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.5))) / 2)
				self.window.blit(stunned_img, stunned_pos)
			self.display_player_info(game, player)

	def display_player_info(self, game, player):
		#display name
		identity_pos = [self.offset_size + (self.cell_size * player.x), self.offset_size + (self.cell_size * player.y)]
		identity_pos[0] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
		identity_pos[1] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
		identity_pos[1] -= math.floor(self.cell_size * 0.15)
		
		color = (255,255,255)
		if player == game.me:
			color = (0, 255, 0)
		label = self.name_font.render(player.name, 1, color)
		
		info_bar_pos = [self.offset_size + (self.cell_size * player.x), self.offset_size + (self.cell_size * player.y)]
		info_bar_pos[1] += math.floor((self.cell_size - (math.floor(self.cell_size * 0.6))) / 2)
		info_bar_pos[1] += math.floor(self.cell_size * 0.6)

		info_bar_heart_label_pos = (info_bar_pos[0] + self.info_bar_heart_pos[0], info_bar_pos[1] + self.info_bar_heart_pos[1])
		heart_label = self.info_bar_font.render(str(player.energy), 1, (255,0,0))

		self.window.blit(self.info_bar_img, info_bar_pos)
		self.window.blit(heart_label, info_bar_heart_label_pos)
		self.window.blit(label, identity_pos)

	def display_energy_cells(self, game):
		for energy_cell in game.energy_cells:
			cell_pos = [self.offset_size + (self.cell_size * energy_cell.x), self.offset_size + (self.cell_size * energy_cell.y)]
			self.window.blit(self.energy_cell_img, cell_pos)

	def display_dead(self):
		self.window.blit(self.dead_img, (824, 949))

	def handle_events(self):
		event = pygame.event.poll()
		if event.type == pygame.QUIT:
			return False
		return True

	def end_game(self, win):
		if win:
			img = pygame.image.load('./ressources/img/endgame_win.png')
		else:
			img = pygame.image.load('./ressources/img/endgame_lose.png')
		pos = (212, 370)
		self.window.blit(img, pos)

	def render(self):
		pygame.display.flip()
