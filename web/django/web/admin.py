# -- coding: utf-8 --

from django.contrib import admin

from .models import Training, Shot, Game, Position

admin.site.register(Training)
admin.site.register(Shot)
admin.site.register(Game)
admin.site.register(Position)
