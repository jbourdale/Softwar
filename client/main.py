# coding: utf8
import sys
import pygame

from core.core import Softwar

if __name__ == "__main__":
	if len(sys.argv) < 4:
		raise ValueError("Usage : " + sys.argv[0] + " IP PORTREQ PORTSUB")

	game = Softwar()
	game.run()

	pygame.quit()
