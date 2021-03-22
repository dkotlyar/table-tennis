# -- coding: utf-8 --

from django import forms
from django.forms import widgets
from django.forms.widgets import HiddenInput

from .models import Message

class MessageForm(forms.ModelForm):
	class Meta:
		model = Message
		fields = ('bytecode', )
		labels = {
			'bytecode': u'Байт-код',
		}
