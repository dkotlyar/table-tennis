# -- coding: utf-8 --

import serial
import time

from django.core.management.base import BaseCommand, CommandError

from control.models import Message
from control import protocol

from web.models import Game

START_BYTE = 0xF0

STATUS_WAIT_START = 0
STATUS_WAIT_LENGTH = 1
STATUS_WAIT_MESSAGE = 2
STATUS_WAIT_CHECKSUM = 3

class Command(BaseCommand):
    args = '<none>'
    help = ''

    def handle(self, *args, **options):
		port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=0)
		self.stdout.write("Serial initialized")
		
		last_id = Message.objects.order_by('-pk')[0].pk

		receive_buffer = []
		last_buffer_length = 0
		status = STATUS_WAIT_START
		checksum = 0
		length = 0

		while True:
			data = port.read(32)
			
			if (len(data) > 0):
				receive_buffer.extend(map(ord, data))
				
			if (len(receive_buffer) != last_buffer_length):
				last_buffer_length = len(receive_buffer)
				if (status == STATUS_WAIT_START):
					while (len(receive_buffer) > 0 and status == STATUS_WAIT_START):
						if (receive_buffer[0] == START_BYTE):
							status = STATUS_WAIT_LENGTH
						else:
							del receive_buffer[0]
				
				if (len(receive_buffer) >= 2 and status == STATUS_WAIT_LENGTH):
					status = STATUS_WAIT_CHECKSUM
					length = receive_buffer[1]
				
				if (len(receive_buffer) >= length + 3 and status == STATUS_WAIT_CHECKSUM):
					status = STATUS_WAIT_START
					checksum = 0
					for x in receive_buffer[2:length+2]:
						checksum = (checksum + x) & 0xFF
					
					if (receive_buffer[length+2] == checksum):
						msg = receive_buffer[:length+3]
						
						message = save_message(msg[2:-1]);
					
						del receive_buffer[:length+3]

						self.stdout.write("Receive: %s" % message.hexcode)
					else:
						del receive_buffer[0]
			
			messages = Message.objects.filter(pk__gt=last_id, direction='tx').order_by('pk')
			
			for message in messages:
				msg = message.get_msg()
				checksum = 0
				for x in msg:
					checksum = (checksum + x)
				checksum = checksum & 0xFF
				msg = [START_BYTE, len(msg), ] + msg + [checksum,]
				port.write(''.join(map(chr, msg)))
				last_id = message.pk
				self.stdout.write("Transmit: %s" % message.hexcode)

		port.close()

def save_message(msg):
	message = Message()
	message.compile(msg)
	message.direction = 'rx'
	
	cmd = protocol.PROTOCOL_COMMANDS[msg[0]]
	if cmd[3] != 'none' and cmd[3] != '':
		message.description = '%s %d %s' % (cmd[0], protocol.byte_array_to_value('uint16', msg[1:3]), cmd[2])
	else:
		message.description = cmd[0]
	
	message.save()
	
	handle_message(msg)
	
	return message
	
def handle_message(msg):
	cmd = protocol.PROTOCOL_COMMANDS[msg[0]]
	if cmd[1] == 'error':
		handle_error_message(msg[1:])
	elif cmd[1] == 'trainer-position':
		game = Game.objects.get(state=4)
		game.position_x = protocol.byte_array_to_value('int16', msg[1:3])
		game.position_y = protocol.byte_array_to_value('int16', msg[3:5])
		game.state = 6
		# if game.training != None:
			# game.training.state = 6
			# game.training.save()
		game.save()
	elif cmd[1] == 'stop-program':
		game = Game.objects.get(state=8)
		game.state = 2
		# if game.training != None:
			# game.training.state = 0
			# game.training.save()
		game.save()
		
		
def handle_error_message(msg):
	cmd = protocol.PROTOCOL_COMMANDS[msg[0]]
	if (cmd[1] == 'trainer-position'):
		try:
			game = Game.objects.get(state=4)
			game.state = 5
			# if (game.training != None):
				# game.training.state = 5
				# game.training.save()
			game.save()
		except:
			for game in Game.objects.filter(state__gt=0):
				game.state = 0
				# if (game.training != None):
					# game.training.state = 0
					# game.training.save()
				game.save()