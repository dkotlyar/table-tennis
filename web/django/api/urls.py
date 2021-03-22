from django.conf.urls import include, url

from . import views

urlpatterns = [
	url(r'^shutdown$', views.shutdown),

	url(r'^training.all$', views.trainings_all),
	url(r'^training.get$', views.training_get),
	url(r'^training.edit$', views.training_edit, name='training-edit'),
	url(r'^training.hide$', views.training_hide),
	url(r'^training.show$', views.training_show),

	url(r'^shot.get$', views.shot_get),
	url(r'^shot.edit$', views.shot_edit, name='shot-edit'),
	url(r'^shot.update_position$', views.shot_update_position, name='shot-update-position'),
	url(r'^shot.delete', views.shot_delete),
	
	url(r'^game.stop$', views.game_stop, name='game-stop'),
	url(r'^game.start$', views.game_start, name='game-start'),
	url(r'^game.pause$', views.game_pause, name='game-pause'),
	url(r'^game.status$', views.game_status, name='game-status'),
	
	url(r'^position.delete$', views.position_delete),
	
	url(r'^terminal.add$', views.terminal_add, name='terminal-add'),
	url(r'^terminal.getlast', views.terminal_get_last),
	url(r'^terminal.getnew', views.terminal_get_new),
	
	url(r'^training.start$', views.training_start, name='training-start'),
	url(r'^trainer.position$', views.trainer_position, name='trainer-position'),
	url(r'^training.set_mode$', views.training_set_mode),

	url(r'^motor.enable$', views.motor_enable, name='motor-enable'),
	url(r'^motor.disable$', views.motor_disable, name='motor-disable'),
	
	url(r'^lift.set$', views.lift_set),
	url(r'^rotation.set$', views.rotation_set),
	url(r'^incline.set$', views.incline_set),
	url(r'^rollrotation.set$', views.rollrotation_set),
	url(r'^toproll.speed$', views.toproll_speed),
	url(r'^bottomroll.speed$', views.bottomroll_speed),
]
