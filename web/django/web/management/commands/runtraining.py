# -- coding: utf-8 --

import serial
import time

from django.core.management.base import BaseCommand, CommandError

from web.models import Training
from handcontrol.models import Message, Protocol

def send_request_position():	
	try:
		protocol = Protocol.objects.get(char_command='')
		message = Message()
	except:
		return False

class Command(BaseCommand):
    args = '<none>'
    help = 'Control training starting'

    def handle(self, *args, **options):
		while (True):
			try:
				training = Training.objects.order_by('-state')[0]
			except:
				training = None
			else:
				#TODO: организовать исполнение программы
				print training.state
				time.sleep(1)

				## Поставлена на исполнение
				#if (training.state == 1):
					#send_request_position()
					#last_message_id = Message.objects.order_by('-pk')[0].pk
					#training.state = 2
				
				## Ожидание координат от тренажёра
				#elif (training.state == 2):
					#wait_flag = True
					#positive_response = Protocol.objects.get(char_command='trainer-position')
					#negative_response = Protocol.objects.get(char_command='trainer-position-error')
					
					#while wait_flag:
						#for message in Message.objects.filter(pk__gt=last_message_id).order_by('pk'):
							#msg = map(lambda x: int(x, 0), message.bytecode.split(','))
							#if (msg[0] == positive_response.command_byte):
								##TODO: координаты тренажёра куда-то записать
								#training.state = 4
								#wait_flag = False
							#elif (msg[0] == negative_response.command_byte):
								##TODO: запросить координаты тренажёра у пользователя
								#training.state = 3
								#wait_flag = False
								
				## Ожидание координат от пользователя
				#elif (training.state == 3):
					
				## Расчёт позиций ударов по заданным координатам
				#elif (training.state == 4):
					
				## Загрузка ударов в тренажёр
				#elif (training.state == 5):
					
				## Исполнение программы (просто ждём)
				#elif (training.state == 6):
						
