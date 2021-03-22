# -- coding: utf-8 --

from django.db import models

import protocol

DIRECTION_TYPE = [
	('rx', 'Приём'),
	('tx', 'Передача'),
]
			
class Message(models.Model):
	hexcode = models.CharField('Код сообщения', max_length=128, default='')
	bytecode = models.CharField('Посылка', max_length=128, default='')
	description = models.CharField('Описание', max_length=255, blank=True)
	timestamp = models.DateTimeField('Время отправки', auto_now_add=True)
	direction = models.CharField('Направление', max_length=2, choices=DIRECTION_TYPE, default='tx')
	
	def __unicode__(self):
		if (self.description != ''):
			return self.description
		else:
			return self.hexcode
			
	def compile(self, msg):
		self.bytecode = ','.join(format("%d" % x) for x in msg)
		self.hexcode = ' '.join(format("%02X" % x) for x in msg)
		self.save()

	def parse(self, message):
		from re import findall
		msg = map(lambda x: int(x, 0), findall(r'(-{0,1}0x[a-fA-F\d]+|-{0,1}0o[\d]+|-{0,1}0b[\d]+|-{0,1}[\d]+)', message))
		self.compile(msg)
		
	def specific_command(self, command, operands):
		cmd = protocol.find_command(command)
		msg = [cmd,] + operands
		self.description = protocol.PROTOCOL_COMMANDS[cmd][0]
		self.compile(msg)
		self.save()

	def get_msg(self):
		return map(lambda x: int(x, 0), self.bytecode.split(','))
