# -- coding: utf-8 --

from django import forms
from django.forms import widgets
from django.forms.widgets import HiddenInput

from .models import Training, Shot, Position

class SliderWidget(widgets.NumberInput):
	def __init__(self, attrs=None, data=None):
		if attrs == None:
			attrs = {}
		if 'class' in attrs:
			attrs['class'] += ' jqui-slider'
		else:
			attrs.update({'class': 'jqui-slider'})
		for key,value in data.items():
			attrs.update({'data-%s' %key : value})
		super(SliderWidget, self).__init__(attrs)


class TrainingUpdateForm(forms.ModelForm):
    class Meta:
		model = Training
		fields = ('name', 'repeat', 'role_min', 'role_max')
		widgets = {
			'repeat': SliderWidget(data={
				'min': 1,
				'max': 30,
				'step': 1,
			}),
		}

class TrainingAddForm(forms.ModelForm):
	class Meta:
		model = Training
		fields = ('name', )

class ShotUpdateForm(forms.ModelForm):
	class Meta:
		model = Shot
		fields = ('name', 'before_delay', 'ball_speed', 'ball_rotation', \
			'rotation_type', 'leteral_rotation', 'shot_type', 'zone')
		widgets = {
			'ball_speed': SliderWidget(data={
				'min': 0,
				'max': 150,
				'step': 2,
			}),
			'ball_rotation': SliderWidget(data={
				'min': 0,
				'max': 5000,
				'step': 100
			}),
			'before_delay': SliderWidget(data={
				'min': 200,
				'max': 5000,
				'step': 50
			}),
			'zone': HiddenInput(),
		}
		
class ShotAddForm(forms.ModelForm):
	class Meta:
		model = Shot
		fields = ('name', )
		
class PositionAddForm(forms.ModelForm):
	class Meta:
		model = Position
		fields = ('lift', 'rotation', 'incline', 'rollrotation', 
			'top_roll_speed', 'bottom_roll_speed', 'before_delay',)
		widgets = {
			'lift': SliderWidget(data={
				'min': 550,
				'max': 1400,
				'step': 10,
				'onchange': 'lift.set',
			}),
			'rotation': SliderWidget(data={
				'min': -120,
				'max': 120,
				'step': 1,
				'onchange': 'rotation.set',
			}),
			'incline': SliderWidget(data={
				'min': -30,
				'max': 50,
				'step': 1,
				'onchange': 'incline.set',
			}),
			'rollrotation': SliderWidget(data={
				'min': -120,
				'max': 120,
				'step': 10,
				'onchange': 'rollrotation.set',
			}),
			'top_roll_speed': SliderWidget(data={
				'min': -10800,
				'max': 10800,
				'step': 100,
				'onchange': 'toproll.speed',
			}),
			'bottom_roll_speed': SliderWidget(data={
				'min': -10800,
				'max': 10800,
				'step': 100,
				'onchange': 'bottomroll.speed',
			}),
			'before_delay': SliderWidget(data={
				'min': 100,
				'max': 3000,
				'step': 50,
			}),
		}
