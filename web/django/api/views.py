# -- coding: utf-8 --

from django.http import JsonResponse
from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse

from web.models import Training, Shot, Game, Position, TRAINING_STATES
from control.models import Message

from web.forms import TrainingUpdateForm, ShotUpdateForm
from control.forms import MessageForm

from control.protocol import value_to_byte_array as val2byte

from api.http import get_prop
		
import time

GOOD_JR = JsonResponse({'error_code': 0})
ERROR_JR = JsonResponse({'error_code': 1})

#WEBAPI
def access_denied(request):
	data = {
		'error_code': 1,
		'error_message': 'Access denied',
	}
	return JsonResponse(data)
	
def shutdown(request):
	message = Message()
	message.specific_command('set-mode', [0])
	message.save()
	
	message = Message()
	message.specific_command('disable-motor', [0])
	message.description = 'Включить все двигатели'
	message.save()
	
	message = Message()
	message.specific_command('top-roll-disable', [])
	message.save()
	
	message = Message()
	message.specific_command('bottom-roll-disable', [])
	message.save()
	
	message = Message()
	message.specific_command('display-change', map(ord, 'PowerOff'))
	message.save()

	import time
	import os
	
	time.sleep(2)
	os.system("echo orangepi | sudo -S shutdown now -h")
	
	return GOOD_JR

@login_required(login_url='/api/access-denied/')
def trainings_all(request):
	try:
		trainings = []
		for t in Training.objects.filter(user=request.user):
			trainings.append(t.json())
		result = {
			'error_code': 0,
			'trainings': trainings
		}
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	return JsonResponse(result)
	
def training_get(request):
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
		result = {
			'error_code': 0,
			'training': training.json()
		}
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return JsonResponse(result)


@login_required
def training_edit(request):
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		if request.method == 'POST':
			form = TrainingUpdateForm(request.POST)
			if (form.is_valid()):
				training.prop_update(form.cleaned_data)
		return GOOD_JR

	
@login_required
def training_hide(request):
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
	except:
		return ERROR_JR
	else:
		training.archived = True
		training.save()
		return GOOD_JR

@login_required
def training_show(request):
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
	except:
		return ERROR_JR
	else:
		training.archived = False
		training.save()
		return GOOD_JR

@login_required		
def training_start(request):		
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
		game = training.get_current_game()
		
		if game == None:
			game = Game(user=request.user)
			game.training = training
			game.state = 4
			game.repeat_count = training.repeat
			game.save()
			
			message = Message()
			message.specific_command('trainer-position', [])
			message.save()
		else:
			if (time.time() - time.mktime(training.edit_dt.timetuple())) > 10:
				game.state = 5
				game.save()
				
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		result = {
			'd': time.time() - time.mktime(training.edit_dt.timetuple()),
			'error_code': 0,
			'training': training.json(),
			'state': game.state,
			'state_print': TRAINING_STATES[game.state][1],
			'game_id': game.pk,
			'game_url': reverse('web:game-start', args=(game.pk,)),
		}
		return JsonResponse(result)
		
def trainer_position(request):		
	try:
		training = Training.objects.get(user=request.user, pk=get_prop(request, 'training_id'))
		game = training.get_current_game()
		
		if game == None:
			raise Exception("Не найдена игра")
		elif game.state == 5:
			game.position_x = int(get_prop(request, 'pos_x'))
			game.position_y = int(get_prop(request, 'pos_y'))
			game.state = 6
			game.save()
			
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return JsonResponse({
			'error_code': 0,
			'training': training.json(),
			'state': game.state,
			'state_print': TRAINING_STATES[game.state][1],
			'game_id': game.pk,
			'game_url': reverse('web:game-start', args=(game.pk,)),
		})
		
def game_stop(request):		
	try:
		game = Game.objects.get(pk=get_prop(request, 'game_id'))
		game.stop()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def game_pause(request):
	try:
		game = Game.objects.get(pk=get_prop(request, 'game_id'))
		game.pause()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def game_start(request):
	try:
		game = Game.objects.get(pk=get_prop(request, 'game_id'))
		game.start()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def game_status(request):
	try:
		game = Game.objects.get(pk=get_prop(request, 'game_id'))
		
		positions = []
		for pos in game.positions():
			positions.append(pos.json())
			
		if (game.state == 7):
			game.load_to_trainer()
			
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return JsonResponse({
			'error_code': 0,
			'state': game.state,
			'state_print': TRAINING_STATES[game.state][1],
			'game_id': game.pk,
			'positions': positions,
		})

def position_delete(request):
	try:
		position = Position.objects.get(pk=get_prop(request, 'position_id'))
		position.delete()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def shot_get(request):
	try:
		shot = Shot.objects.get(user=request.user, pk=get_prop(request, 'shot_id'))
		result = {
			'error_code': 0,
			'shot': shot.json()
		}
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return JsonResponse(result) 
		
def shot_edit(request):
	try:
		shot = Shot.objects.get(user=request.user, pk=get_prop(request, 'shot_id'))
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		if request.method == 'POST':
			form = ShotUpdateForm(request.POST)
			if (form.is_valid()):
				shot.prop_update(form.cleaned_data)
		return GOOD_JR
	
def shot_update_position(request):		
	try:		
		shot = Shot.objects.get(pk=get_prop(request, 'shot_id'))
		shot.set_position(get_prop(request, 'sort_position'))
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
	
def shot_delete(request):
	try:
		shot = Shot.objects.get(user=request.user, pk=get_prop(request, 'shot_id'))
		shot.delete()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR

def terminal_add(request):
	if (request.method == 'POST'):
		form = MessageForm(request.POST)
		if form.is_valid():
			message = Message()
			message.parse(form.cleaned_data['bytecode'])
			message.save()
			return GOOD_JR
	return ERROR_JR
	
def terminal_get_last(request):
	try:
		last_id = Message.objects.all().order_by('-pk')[0].pk
		return JsonResponse({
			'error_code': 0,
			'last_id': last_id,
		})
	except Exception, e:
		return JsonResponse({
			'error_code': 0,
			'error_message': str(e),
		})
	
def terminal_get_new(request):
	try:
		last_id = int(request.POST['last_id'])
		messages = Message.objects.filter(pk__gt=last_id).order_by('pk')
		msgs = []
		for msg in messages:
			if msg.pk > last_id:
				last_id = msg.pk
			msgs.append({
				'hexcode': msg.hexcode,
				'timestamp': msg.timestamp,
				'direction': msg.direction,
				'description': msg.description,
			})
		result = {
			'error_code': 0,
			'elements_count': messages.count(),
			'last_id': last_id,
			'messages': msgs,
		}
		return JsonResponse(result)
	except Exception, e:
		return JsonResponse({
			'error_code': 0,
			'error_message': str(e),
		})

#CONTROL API
def training_set_mode(request):
	try:
		power_mode = int(get_prop(request, 'power_mode'))
		message = Message()
		message.specific_command('set-mode', [power_mode])
		message.description = 'Установить режим работы №%d' % power_mode
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR

def motor_enable(request):
	try:
		motor_id = int(get_prop(request, 'motor'))
		message = Message()
		message.specific_command('enable-motor', [motor_id])
		if motor_id == 0:
			message.description = 'Включить все двигатели'
		else:
			message.description = 'Включить двигатель №%d' % motor_id
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
	
def motor_disable(request):
	try:
		motor_id = int(get_prop(request, 'motor'))
		message = Message()
		message.specific_command('disable-motor', [motor_id])
		if motor_id == 0:
			message.description = 'Отключить все двигатели'
		else:
			message.description = 'Отключить двигатель №%d' % motor_id
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def lift_set(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('lift', val2byte('uint16', value))
		message.description = ('Установить высоту пушки %d мм' % value)
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def rotation_set(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('rotation', val2byte('int16', value))
		message.description = ('Установить поворот пушки %d °' % value)
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def incline_set(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('incline', val2byte('int16', value))
		message.description = 'Установить наклон ствола %d °' % value
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def rollrotation_set(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('rollrotation', val2byte('int16', value))
		message.description = 'Установить поворот ствола %d °' % value
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def toproll_speed(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('top-roll-speed', val2byte('int16', value))
		message.description = 'Установить скорость верхнего ролика %d об/мин' % value
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
		
def bottomroll_speed(request):
	try:
		value = int(get_prop(request, 'value'))
		message = Message()
		message.specific_command('bottom-roll-speed', val2byte('int16', value))
		message.description = 'Установить скорость нижнего ролика %d об/мин' % value
		message.save()
	except Exception, e:
		return JsonResponse({
			'error_code': 1,
			'error_message': str(e),
		})
	else:
		return GOOD_JR
