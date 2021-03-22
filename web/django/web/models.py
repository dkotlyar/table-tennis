# -*- coding: utf-8 -*-

from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
from django.db.models import F

from control.models import Message

from control.protocol import value_to_byte_array as val2byte

TRAINING_STATES = [
	(0, 'Остановлена'),
	(1, 'Программа остановлена пользователем'),
	(2, 'Программа завершена'),
	(3, 'Программа приостановлена'),
	(4, 'Ожидание отклика от тренажёра'),
	(5, 'Ожидание ввода положения тренажёра'),
	(6, 'Расчёт позиций'),
	(7, 'Загрузка программы'),
	(8, 'Исполнение программы'),
]

SPORTSMEN_ROLES = [
	('none', u'Отсутствует'),
	('3 jn', u'3 юношеский'),
	('2 jn', u'2 юношеский'),
	('1 jn', u'1 юношеский'),
	('3 sn', u'3 взрослый'),
	('2 sn', u'2 взрослый'),
	('1 sn', u'1 взрослый'),
	('CMS', u'КМС'),
	('MS', u'МС'),
	('IMS', u'МСМК'),
]

ROTATION_TYPES = [
	(1, u'Верхнее'),
	(-1, u'Нижнее'),
]

LETERAL_ROTATIONS = [
	(2, u'Отсутствует'),
	(1, u'Правое'),
	(-1, u'Левое'),
]

SHOT_TYPES = [
	('attack-shot', 'Удар с высокой точки'),
	('over-net', 'Удар над сеткой'),
	('over-table', 'Удар над столом'),
	('under-table', 'Топ-спин'),
]

POSITION_MATCHES = [
	('f', 'Удар не возможен'),
	('t', 'Удар возможен')
]

class Training(models.Model):
	user = models.ForeignKey(User, related_name='+', verbose_name="Имя пользователя")
	name = models.CharField("Имя программы тренировки", max_length=50)
	
	repeat = models.IntegerField("Количество повторений", default=1)
	role_min = models.CharField("Минимальный ранг спортсмена", max_length=4, choices=SPORTSMEN_ROLES, default='none')
	role_max = models.CharField("Максимальный ранг спортсмена", max_length=4, choices=SPORTSMEN_ROLES, default='none')
	
	edit_dt = models.DateTimeField("Время редактирования", auto_now=True)
	creat_dt = models.DateTimeField("Время добавления", auto_now_add=True)
	run_dt = models.DateTimeField("Время последнего запуска программы тренировки", default=timezone.now)
	
	archived = models.BooleanField("Архивировано?", default=False)
	
	# state = models.IntegerField("Состояние", choices=TRAINING_STATES, default=0)
	
	def __unicode__(self):
		return self.name
		
	def shots(self):
		return Shot.objects.filter(training=self).order_by('sort_position')
	
	def get_current_game(self):
		games = Game.objects.filter(training=self, state__gte=3)
		count = len(games)
		if count == 0:
			return None
		elif count == 1:
			return games[0]
		else:
			for game in games:
				game.state = 0
			raise Exception("Более одной активной игры")
	
	def prop_update(self, prop):
		updated = {}
		if 'name' in prop:
			self.name = prop['name']
			updated['name'] = self.name
		if 'repeat' in prop:
			self.repeat = prop['repeat']
			updated['repeat'] = self.repeat
		if 'role_min' in prop:
			self.role_min = prop['role_min']
			updated['role_min'] = self.role_min
		if 'role_max' in prop:
			self.role_max = prop['role_max']
			updated['role_max'] = self.role_max
		self.save()
		return updated
		
	def json(self):
		shots = []
		for shot in self.shots():
			shots.append(shot.json())
		return {
			'id': self.pk,
			'name': self.name,
			'repeat': self.repeat,
			'role_min': self.role_min,
			'role_max': self.role_max,
			'archived': self.archived,
			# 'state': self.state,
			'shots': shots,
		}

class Shot(models.Model):
	user = models.ForeignKey(User, related_name='+', verbose_name="Имя пользователя")
	training = models.ForeignKey(Training, verbose_name="Программа тренировки")
	name = models.CharField("Название удара", max_length=50)
	
	sort_position = models.IntegerField("Позиция удара в программе тренировки", default=0)
	before_delay = models.IntegerField("Предварительная задержка удара", default=1000) # мс
	
	ball_speed = models.IntegerField("Скорость мяча", default=0) #км/ч
	ball_rotation = models.IntegerField("Вращение мяча", default=0) #об/мин
	rotation_type = models.IntegerField("Тип вращения", default=1, choices=ROTATION_TYPES)
	leteral_rotation = models.IntegerField("Боковое вращение", choices=LETERAL_ROTATIONS, default=2)
	shot_type = models.CharField("Тип удара", max_length=16, choices=SHOT_TYPES, default='over-net')
	
	zone = models.IntegerField("Зона удара", default=6)
	
	edit_dt = models.DateTimeField("Время редактировния", auto_now=True)
	create_dt = models.DateTimeField("Время создания", auto_now_add=True)
	archived = models.BooleanField("Архивировано?", default=False)
	
	def __unicode__(self):
		return self.name
		
	def prop_update(self, prop):
		updated = {}
		if 'name' in prop:
			self.name = prop['name']
			updated['name'] = self.name
		if 'before_delay' in prop:
			self.before_delay = prop['before_delay']
			updated['before_delay'] = self.before_delay
		if 'ball_speed' in prop:
			self.ball_speed = prop['ball_speed']
			updated['ball_speed'] = self.ball_speed
		if 'ball_rotation' in prop:
			self.ball_rotation = prop['ball_rotation']
			updated['ball_rotation'] = self.ball_rotation
		if 'rotation_type' in prop:
			self.rotation_type = prop['rotation_type']
			updated['rotation_type'] = self.rotation_type
		if 'leteral_rotation' in prop:
			self.leteral_rotation = prop['leteral_rotation']
			updated['leteral_rotation'] = self.leteral_rotation
		if 'shot_type' in prop:
			self.shot_type = prop['shot_type']
			updated['shot_type'] = self.shot_type
		if 'zone' in prop:
			self.zone = prop['zone']
			updated['zone'] = self.zone
		self.save()
		return updated
		
	def set_position(self, sort_position):
		last_position = self.sort_position
		if sort_position < last_position:
			Shot.objects.filter(training=self.training, \
				sort_position__lt=last_position, sort_position__gte=sort_position)\
				.update(sort_position=F('sort_position') + 1)
		elif last_position < sort_position:
			Shot.objects.filter(training=self.training, \
				sort_position__lte=sort_position, sort_position__gt=last_position)\
				.update(sort_position=F('sort_position') - 1)
		
		self.sort_position = sort_position
		self.save()
		
	def clone(self, training):
		self.pk = None
		self.training = training
		self.sort_position = Shot.objects.filter(training=self.training).count()+1
		self.name = u'%s (копия)' % self.name
		self.save() 
		
	def json(self):
		return {
			'id': self.pk,
			'name': self.name,
			'sort_position': self.sort_position,
			'before_delay': self.before_delay,
			'ball_speed': self.ball_speed,
			'ball_rotation': self.ball_rotation,
			'rotation_type': self.rotation_type,
			'leteral_rotation': self.leteral_rotation,
			'shot_type': self.shot_type,
			'zone': self.zone,
		}

class Game(models.Model):
	user = models.ForeignKey(User, related_name='+', verbose_name="Имя пользователя")
	training = models.ForeignKey(Training, verbose_name="Программа тренировки", blank=True, null=True)
	
	position_x = models.IntegerField("Позиция тренажёра по X", default=0)
	position_y = models.IntegerField("Позиция тренажёра по Y", default=0)
	
	run_dt = models.DateTimeField("Время начала игры", auto_now_add=True)
	stop_dt = models.DateTimeField("Время окончания игры", auto_now=True)
	
	state = models.IntegerField("Состояние программы", choices=TRAINING_STATES, default=0)
	
	repeat_count = models.IntegerField("Количество повторений", default=0)

	def __unicode__(self):
		if (self.training == None):
			return u'Новая игра (%s)' % (self.run_dt)
		else:
			return '%s (%s)' % (self.training.name, self.run_dt)
		
	def positions(self):
		return Position.objects.filter(game=self).order_by('sort_position')
		
	def calc(self):
		pass
		
	def load_to_trainer(self):
		pass
		message_clear = Message()
		message_clear.specific_command('clear-program', [])
		message_clear.save()
		
		message_repeat = Message()
		message_repeat.specific_command('repeat-count', val2byte('uint8', self.repeat_count))
		message_repeat.save()
		
		for position in self.positions():
			message_pos = Message()
			operands =	val2byte('int16', position.lift) + \
						val2byte('int16', position.rotation) + \
						val2byte('int8', position.incline) + \
						val2byte('int16', position.rollrotation) + \
						val2byte('int16', position.top_roll_speed) + \
						val2byte('int16', position.bottom_roll_speed) + \
						val2byte('int16', position.before_delay)
			message_pos.specific_command('append-shot', operands)
			message_pos.description = u'Добавить удар "%s" в конец программы' % position.name
			message_pos.save()
		
		self.start()
	
	def start(self):
		if self.state == 3 or self.state == 7:
			message = Message()
			message.specific_command('set-mode', [1]) # Установить режим работы №1 (исполнение программы тренировки)
				
			message = Message()
			message.specific_command('start-program', [])
			
			self.state = 8
			if (self.training != None):
				self.training.state = 8
				self.training.save()
			self.save()
	
	def stop(self):
		if self.state == 8 or self.state == 3:
			message = Message()
			message.specific_command('stop-program', [])
			message.save()
			
			self.state = 1
			if (self.training != None):
				self.training.state = 0
				self.training.save()
			self.save()
			
	def pause(self):
		if self.state == 8:
			message = Message()
			message.specific_command('pause-program', [])
			message.save()
			
			self.state = 3
			if (self.training != None):
				self.training.state = 3
				self.training.save()
			self.save()
			
	def json(self):
		positions = []
		for pos in self.positions():
			positions.append(pos.json())
		return {
			'id': self.pk,
			'name': self,
			'position_x': self.position_x,
			'position_y': self.position_y,
			'state': self.state,
			'repeat_count': self.repeat_count,
		}
	
class Position(models.Model):
	user = models.ForeignKey(User, related_name='+', verbose_name="Имя пользователя")
	name = models.CharField("Название позиции", max_length=50, default='')
	game = models.ForeignKey(Game, related_name='+', verbose_name="Игра", blank=True, null=True)
	
	sort_position = models.IntegerField("Позиция удара", default=0)
	before_delay = models.IntegerField("Предварительная задержка удара", default=500) # мс
	
	lift = models.IntegerField("Положение пушки по высоте", default=1000)
	rotation = models.IntegerField("Поворот пушки", default=0)
	incline = models.IntegerField("Наклон ствола", default=0)
	rollrotation = models.IntegerField("Поворот ствола", default=0)
	
	top_roll_speed = models.IntegerField("Скорость верхнего ролика", default=0)
	bottom_roll_speed = models.IntegerField("Скорость нижнего ролика", default=0)
	
	matched = models.CharField("Удар подсчитан", max_length=1, default='t', choices=POSITION_MATCHES)
	
	def __unicode__(self):
		return self.name
	
	def set_position(self, sort_position):
		last_position = self.sort_position
		if sort_position < last_position:
			Position.objects.filter(user=self.user, \
				sort_position__lt=last_position, sort_position__gte=sort_position)\
				.update(sort_position=F('sort_position') + 1, name=('Удар №%d' % F('sort_position') + 1))
		elif last_position < sort_position:
			Position.objects.filter(user=self.user, \
				sort_position__lte=sort_position, sort_position__gt=last_position)\
				.update(sort_position=F('sort_position') - 1, name=('Удар №%d' % F('sort_position') - 1))
		
		self.sort_position = sort_position
		self.name = u'Удар №%d' % sort_position
		self.save()
		
	def update(self, prop):
		updated = {}
		if 'lift' in prop:
			self.lift = prop['lift']
			updated['lift'] = self.lift
		if 'rotation' in prop:
			self.rotation = prop['rotation']
			updated['rotation'] = self.rotation
		if 'incline' in prop:
			self.incline = prop['incline']
			updated['incline'] = self.incline
		if 'rollrotation' in prop:
			self.rollrotation = prop['rollrotation']
			updated['rollrotation'] = self.rollrotation
		if 'top_roll_speed' in prop:
			self.top_roll_speed = prop['top_roll_speed']
			updated['top_roll_speed'] = self.top_roll_speed
		if 'bottom_roll_speed' in prop:
			self.bottom_roll_speed = prop['bottom_roll_speed']
			updated['bottom_roll_speed'] = self.bottom_roll_speed
		if 'before_delay' in prop:
			self.before_delay = prop['before_delay']
			updated['before_delay'] = self.before_delay
		self.save()
		return updated
		
	def json(self):
		return {
			'id': self.pk,
			'name': self.name,
			'sort_position': self.sort_position,
			'before_delay': self.before_delay,
			'lift': self.lift,
			'rotation': self.rotation,
			'incline': self.incline,
			'rollrotation': self.rollrotation,
			'top_roll_speed': self.top_roll_speed,
			'bottom_roll_speed': self.bottom_roll_speed,
			'matched': self.matched,
		}
		
