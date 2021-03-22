from django.conf.urls import include, url
from . import views, api

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^dev/$', views.dev, name='dev'),
    
    
    url(r'^trainings/$', views.trainings, name='trainings'),
    url(r'^training/(\d+)/$', views.training_edit, name='training-edit'),
    url(r'^training/add/$', views.training_add, name='training-add'),
    url(r'^training/start/(\d+)/$', views.training_start, name='training-start'),
    url(r'^shots/$', views.shots, name='shots'),
    url(r'^shots/add-or-clone/(\d+)/$', views.shots_add_or_clone, name='shots-add-or-clone'),
    url(r'^shot/(\d+)/$', views.shot_edit, name='shot-edit'),
    url(r'^shot/copy/(\d+)/to/(\d+)/$', views.shot_copy, name='shot-copy'),
    url(r'^shot/add/(\d+)/$', views.shot_add, name='shot-add'),
    
	url(r'^game/start/(\d+)$', views.game_start, name='game-start'),
	url(r'^game/create/$', views.game_create, name='game-create'),
	
    url(r'^positions/$', views.positions, name='positions'),
    url(r'^terminal/$', views.terminal, name='terminal'),
	
	url(r'^poweroff/$', views.poweroff, name='poweroff'),
]
