import random

class IA():
	def __init__(self, player, softwar):
		self.player = player
		self.softwar = softwar

	def __repr__(self):
		return "%s" % (self.player)

	def element_manhattan_distance(self, elements):
		nearestElement = None
		lowestDistance = None

		for element in elements:
			distance = (element.x - self.player.x) + (element.y - self.player.y)

			if nearestElement is None or abs(distance) < lowestDistance:
				if element.energy is None or (element.energy is not None and element.energy > 0):
					nearestElement = element
					lowestDistance = abs(distance)

		return {'object': nearestElement, 'distance': lowestDistance}

	def manhattan_distance(self):
		nearestEnergyCell = self.element_manhattan_distance(self.softwar.energy_cells)

		return nearestEnergyCell

	def is_enemy_in_sight(self):
		cases = self.softwar.networking.watch()
		if cases:
			for enemy in self.softwar.enemies:
				if enemy.stunned == 0:
					if cases.find(enemy.name.decode('utf-8')) != -1:
						return True
		return False


	def main_ia(self):
		isEnemyInSight = self.is_enemy_in_sight()
		if isEnemyInSight == True:
			self.softwar.networking.attack()

		nearestEnergyCell = self.manhattan_distance()

		# if nearestEnergyCell exist
		if nearestEnergyCell['object'] is not None and self.player.actionpoint > 0:
			horizontalDirection = nearestEnergyCell['object'].x - self.player.x
			verticalDirection = nearestEnergyCell['object'].y - self.player.y

			# Element is located on left
			if horizontalDirection < 0:
				if self.player.looking == 1:
					self.softwar.networking.left()
				elif self.player.looking == 2:
					self.softwar.networking.left()
					self.softwar.networking.left()
				elif self.player.looking == 3:
					self.softwar.networking.right()
			# Element is located on right
			elif horizontalDirection > 0:
				if self.player.looking == 0:
					self.softwar.networking.right()
					self.softwar.networking.right()
				elif self.player.looking == 1:
					self.softwar.networking.right()
				elif self.player.looking == 3:
					self.softwar.networking.left()

			if self.player.energy > 2 and horizontalDirection >= 3:
				self.softwar.networking.jump()

			while self.player.actionpoint > 0 and horizontalDirection != 0:
				self.softwar.networking.forward()
				horizontalDirection += 1 if horizontalDirection < 0 else -1

			if self.player.actionpoint > 0:
				# Element is located on up
				if verticalDirection < 0:
					if self.player.looking == 0:
						self.softwar.networking.right()
					elif self.player.looking == 2:
						self.softwar.networking.left()
					elif self.player.looking == 3:
						self.softwar.networking.right()
						self.softwar.networking.right()
				# Element is located on down
				elif verticalDirection > 0:
					if self.player.looking == 0:
						self.softwar.networking.left()
					elif self.player.looking == 1:
						self.softwar.networking.left()
						self.softwar.networking.left()
					elif self.player.looking == 2:
						self.softwar.networking.right()

				if self.player.energy > 2 and horizontalDirection >= 3:
					self.softwar.networking.jump()

				while self.player.actionpoint > 0 and verticalDirection != 0:
					self.softwar.networking.forward()
					verticalDirection += 1 if verticalDirection < 0 else -1

			if self.player.actionpoint == 1 and abs(horizontalDirection) == 0 and abs(verticalDirection) == 0:
				self.softwar.networking.gather()
		else:
			random_action = random.randint(1, 4)
			actions = {
				1: self.softwar.networking.forward,
				2: self.softwar.networking.backward,
				3: self.softwar.networking.leftfwd,
				4: self.softwar.networking.rightfwd,
			}

			actions[random_action]()
