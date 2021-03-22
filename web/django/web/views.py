# -- coding: utf-8 --

from django.shortcuts import get_object_or_404, get_list_or_404, render
from django.http import HttpResponseRedirect, JsonResponse, Http404
from django.core.urlresolvers import reverse
from django.views.decorators.cache import cache_control
from django.contrib.auth.decorators import login_required

from .models import Training, Shot, Position, Game
from control.models import Message
from control.protocol import value_to_byte_array as val2byte

from .forms import TrainingUpdateForm, TrainingAddForm, ShotUpdateForm, PositionAddForm
from control.forms import MessageForm

def index(request):
	return render(request, 'web/index.html')
	
def dev(request):
	return render(request, 'web/dev.html')

@login_required
def trainings(request):
	try:
		training_list = Training.objects.filter(user=request.user).order_by('-run_dt', '-edit_dt')
	except:
		raise Http404
	else:
		return render(request, 'web/trainings.html', {'training_list': training_list})

@login_required
def training_edit(request, training_id):
	try:
		training = Training.objects.get(user=request.user, pk=training_id)
	except:
		raise Http404
	else:
		if request.method == 'POST':
			form = TrainingUpdateForm(request.POST)
			if (form.is_valid()):
				training.prop_update(form.cleaned_data)
				return HttpResponseRedirect(reverse('web:training-edit', args=(training.id,)))
		else:
			form = TrainingUpdateForm(instance=training)
		return render(request, 'web/training_edit.html', { 'form': form, 'training': training })
	
@login_required
def training_add(request):
	if request.method == 'POST':
		form = TrainingAddForm(request.POST)
		if (form.is_valid()):
			training = Training(name=form.cleaned_data['name'], user=request.user)
			training.save()
			return HttpResponseRedirect(reverse('web:training-edit', args=(training.id,)))
	else:
		form = TrainingAddForm()
	return render(request, 'web/training_add.html', { 'form': form })

@login_required
def training_start(request, training_id):
	try:
		training = Training.objects.get(user=request.user, pk=training_id)
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
		elif game.state == 3:
			return HttpResponseRedirect(reverse('web:game-start', args=(game.id,)))
		
	except Exception, e:
		return render(request, 'web/error.html', { 'errmsg': e.message })
	else:
		return render(request, 'web/training_start.html', { 'training': training })

@login_required
def shots(request):
	try:
		training_list = Training.objects.filter(user=request.user).order_by('-run_dt', '-edit_dt')
	except:
		raise Http404
	else:
		return render(request, 'web/shots.html', {'training_list': training_list})
	
@login_required
def shots_add_or_clone(request, training_id):
	try:
		training_list = Training.objects.filter(user=request.user).order_by('-run_dt', '-edit_dt')
	except:
		raise Http404
	else:
		return render(request, 'web/shots_add_or_clone.html', {
			'training_list': training_list, 
			'training_id': training_id
		})

@login_required
def shot_edit(request, shot_id):
	try:
		shot = Shot.objects.get(user=request.user, pk=shot_id)
	except:
		raise Http404
	else:
		if request.method == 'POST':
			form = ShotUpdateForm(request.POST)
			if (form.is_valid()):
				shot.prop_update(form.cleaned_data)
				return HttpResponseRedirect(reverse('web:shot-edit', args=(shot.id,)))
		else:
			form = ShotUpdateForm(instance=shot)
		return render(request, 'web/shot_edit.html', { 'form': form, 'shot': shot })
		
@login_required
def shot_copy(request, shot_id, training_id):
	try:
		shot = Shot.objects.get(user=request.user, pk=shot_id)
		training = Training.objects.get(user=request.user, pk=training_id)
	except:
		raise Http404
	else:
		shot.clone(training)
		return HttpResponseRedirect(reverse('web:shot-edit', args=(shot.id,)))

@login_required
def shot_add(request, training_id):
	try:
		training = Training.objects.get(user=request.user, pk=training_id)
	except:
		raise Http404
	else:
		shot = Shot(name='Новый удар', user=request.user, training=training)
		shot.sort_position = Shot.objects.filter(training=training).count()+1
		shot.save()
		return HttpResponseRedirect(reverse('web:shot-edit', args=(shot.id,)))

@login_required
def game_create(request):
	try:
		game = Game(user=request.user)
		game.state = 7
		game.repeat_count = 255
		game.save()
		
		for pos in Position.objects.filter(user=request.user, game=None):
			pos.pk = None
			pos.game = game
			pos.save()
	except:
		raise Http404
	else:
		return HttpResponseRedirect(reverse('web:game-start', args=(game.pk,)))
		
@login_required
def game_start(request, game_id):
	try:
		game = Game.objects.get(user=request.user, pk=game_id)
		
		if (game.state == 3):
			game.start()
		
		if (game.state == 7):
			game.load_to_trainer()
		
	except:
		raise Http404
	else:
		return render(request, 'web/game_start.html', { 'game': game })
		
@login_required
def positions(request):
	if request.method == 'POST':
		form = PositionAddForm(request.POST)
		if (form.is_valid()):
			try:
				last_sort_position = Position.objects.filter(user=request.user, game=None).\
					order_by('-sort_position')[0].sort_position + 1
			except:
				last_sort_position = 1
				
			position = Position(user=request.user, name='Удар №%d' % last_sort_position)
			position.sort_position = last_sort_position
			position.save()
			position.update(form.cleaned_data)
	else:
		form = PositionAddForm()
		
	positions = Position.objects.filter(user=request.user, game=None).order_by('sort_position')
	return render(request, 'web/positions.html', { 'form': form, 'positions': positions })

def terminal(request):
	if (request.method == 'POST'):
		form = MessageForm(request.POST)
		if form.is_valid():
			message = Message()
			message.parse(form.cleaned_data['bytecode'])
			message.save()
	else:
		form = MessageForm()
	messages = Message.objects.all().order_by('-timestamp')
	return render(request, 'web/terminal.html', { 'messages': messages, 'form': form })
	
def poweroff(request):
	return render(request, 'web/poweroff.html')
