# -- coding: utf-8 --

import time

from django.core.management.base import BaseCommand, CommandError
from django.core.exceptions import ObjectDoesNotExist
from web.models import Game, Position

from trainercalc import calc_optimum
import math

tableHeight = 760 # высота стола, мм

LINEAR_COEF = 0.85 # 0.82
ANGLE_COEF = 0.9 # 0.85

class Command(BaseCommand):
    args = '<none>'
    help = 'Control training starting'

    def handle(self, *args, **options):
		print "Start calc positions"
		
		games = Game.objects.filter(state=6) #  # получение игр, в которых нужен расчёт позиций
		
		for game in games:
			game.state = 0
			game.save()
		
		while (True):
			try:
				game = Game.objects.get(state=6) # получение игр, в которых нужен расчёт позиций
				print "Calc position for game with ID: %d" % game.pk
		
				# if (game.training == None):
					# game.state = 7
					# continue
					
				for shot in game.training.shots():
					position = Position(user=game.user)
					position.user = game.user
					position.game = game
					position.name = shot.name
					position.sort_position = shot.sort_position
					position.before_delay = shot.before_delay
					
					A = float(1600 + 460 * (shot.zone / 4) + game.position_x)
					B = float(570 - 380 * (shot.zone % 4) - game.position_y) * ANGLE_COEF
					L = math.sqrt(A*A + B*B) # длина полёта мяча, мм

					Zstart = 0 # высота от пола, мм
					Zend = 0   # предельная высота от пола, мм
					
					max_angle = 50
					
					if (shot.shot_type == 'under-table'):
						Zstart = 600
						Zend = 750
					elif (shot.shot_type == 'over-table'):
						Zstart = 800
						Zend = 1100
					elif (shot.shot_type == 'over-net'):
						Zstart = 1000
						Zend = 1250
					else:
						Zstart = 1300
						Zend = 1360
						# max_angle = 0
						
					Z0 = (Zstart - tableHeight) / 1000.0 # высота над столом, м
					Zmax = (Zend - tableHeight) / 1000.0 # максимально допустимая высота над столом, м

					rotation = int(round(math.asin(B/L) / math.pi * 180))
					rollrotation = shot.leteral_rotation * shot.rotation_type * 45

					V = shot.ball_speed / 3.6 # м/с
					W = shot.rotation_type * math.copysign(shot.ball_rotation, rollrotation) # скорость вращения
					
					V_norm, Z_norm, W_norm, angle_norm, matched = calc_optimum(game.position_x, L, V, Z0, Zmax, W, LINEAR_COEF) # расчёт параметров выстрела
					if (not matched):
						print "%s not matched" % (shot.name)
						position.matched = 'f'
					
					# print V_norm
					# print Z_norm
					# print W_norm
					# print angle_norm
					
					if W_norm == 0:
						rollrotation = 0
					
					position.rotation = rotation
					position.rollrotation = rollrotation
					position.lift = Z_norm * 1000 + tableHeight
					position.incline = angle_norm
					
					linear_speed = V_norm / (2 * math.pi * 0.025) * 60 # линейная сокрость
					angular_speed = W_norm
					position.top_roll_speed = (linear_speed + angular_speed) / 2
					position.bottom_roll_speed = linear_speed - position.top_roll_speed
					
					position.save()
					
				game.state = 7
				# game.training.state = 7
				# game.training.save()
				game.save()
				
				print "FINISH. Game with ID: %d is ready" % game.pk
			
			except ObjectDoesNotExist, ex:
				time.sleep(1)
				
			except Exception, ex:
				print ex
				time.sleep(1)
			
