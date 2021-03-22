var ajaxsetup = false;

var	terminal_row_tpl, 
	trainer_position_tpl,
	position_item_tpl;

swig.setExtension('static', function (path) {
  return STATIC_URL + path;
});
swig.setTag('static', function (str, line, parser, types, options) {
  return true;
}, function (compiler, args, content, parents, options, blockName) {
  return '_output += _ext.static(' + args[0] + ');';
});

$(function(){
	$.ajaxSetup({ 
		beforeSend: function(xhr, settings) {
			if (!(/^http:.*/.test(settings.url) || /^https:.*/.test(settings.url))) {
				// Only send the token to relative URLs i.e. locally.
				xhr.setRequestHeader("X-CSRFToken", getCookie('csrftoken'));
			}
		} 
	});
	ajaxsetup = true;
	
	// Загрузка шаблонов
	$.get('/static/web/templates/terminal_row.html', {}, function(response){
		terminal_row_tpl = swig.compile(response, {filename: 'terminal-row'});
	});
	$.get('/static/web/templates/trainer_position.html', {}, function(response){
		trainer_position_tpl = swig.compile(response, {filename: 'trainer-position'});
	});
	$.get('/static/web/templates/position_item.html', {}, function(response){
		position_item_tpl = swig.compile(response, {filename: 'position-item'});
	})
	
	$(window).resize(on_resize_window);
	on_resize_window();
	
	$('select').niceSelect();
	
	$('input.jqui-slider').each(function(){
		var $this = $(this);
		var id = $this.attr('id');
		$this.after('<div/>');
		try {
			$this.next().slider({
				animate: 1000,
				range: 'min',
				step: get_data('step', 1),
				min: get_data('min', 0),
				max: get_data('max', 100),
				value: $this.val(),
				change: function(event, ui) {
					if ($this.data('onchange')) {
						$.post(API_URL + $this.data('onchange'), {
							value: ui.value
						});
					}
				},
				slide: function(event, ui) {
					$('label[for='+ id +']').find('strong').text(ui.value);
					$this.val(ui.value);
					if ($this.data('onslide')) {
						$.post(API_URL + $this.data('onslide'), {
							value: ui.value
						});
					}
				}
			});
			$this.hide();
		} catch (e) {}
		
		function get_data(name, _def) {
			return $this.data(name) || _def;
		}
	});

	$('.js-training-hide').on('click', function(){
		var $this = $(this);
		$.post(API_URL + 'training.hide', {
			training_id: $this.data('tid')
		}, function(response){
			if (response.error_code == 0) {
				$this.hide();
				$this.parent().find('.js-training-show').show();
				$this.parents('.card-wrap').addClass('card-archived');
			}
		});
	});

	$('.js-training-show').on('click', function(){
		var $this = $(this);
		$.post(API_URL + 'training.show', {
			training_id: $this.data('tid')
		}, function(response){
			if (response.error_code == 0) {
				$this.hide();
				$this.parent().find('.js-training-hide').show();
				$this.parents('.card-wrap').removeClass('card-archived');
			}
		});
	});

	var training_shots = $('.training-shots');
	if (typeof training_shots != 'undefined') {
		if (!training_shots.find('>div:eq(0)').hasClass('empty-screen')) {
			training_shots.sortable({
				axis: 'y',
				distance: 15,
				handle: '.list-item-swap',
				helper:'clone',
				opacity:0.9,
				revert: 250,
				update: function(event, ui) {
					var shot = ui.item[0];
					var shot_id = $(shot).find('input[name="shot_id"]').val()
					var position = training_shots.find('>div').index(shot)+1;
					$.post(API_URL + 'shot.update_position', {
						shot_id: shot_id,
						sort_position: position
					});
				}
			});
		}
	}
	
	if (typeof zone_input != 'undefined') {
		var tennis_table_cell = $('.js-tennis-table-cell');
		tennis_table_cell.on('click', function(){
			var cell = $(this);
			var index = tennis_table_cell.index(cell);
			
			zone_input.val(index);
			
			tennis_table_cell.removeClass('tennis-zone-selected');
			cell.addClass('tennis-zone-selected');
		});
		tennis_table_cell.eq(zone_input.val()).addClass('tennis-zone-selected');
	}
	
	$('.js-button-start-position').on('click', function(){
		var $this = $(this);
		if ($this.data('state')) {
			$.post(API_URL + 'motor.disable', {
				motor: 0
			}, function() {
				$this.text('Старт').data('state', false);
			});
		} else {
			$.post(API_URL + 'motor.enable', {
				motor: 0
			}, function() {
				$this.text('Стоп').data('state', true);
			});
		}
		return false;
	});
	$('.js-button-start-feed').on('click', function(){
		var $this = $(this);
		if ($this.data('state')) {
			$.post(API_URL + 'motor.disable', {
				motor: 5
			}, function() {
				$this.text('Запустить подачу мячей').data('state', false);
			});
		} else {
			$.post(API_URL + 'motor.enable', {
				motor: 5
			}, function() {
				$this.text('Остановить подачу мячей').data('state', true);
			});
		}
		return false;
	});
	$('.js-select-power-mode').on('change', function() {
		var power_mode = $(this).val();
		$.post(API_URL + 'training.set_mode', {
			power_mode: power_mode
		}, function(response) {
			console.log(response);
			
			if (power_mode == 0) {
				$('.js-button-start-position').show();
				$('.js-button-start-feed').show();
				$('.js-button-start-program').hide();
			} else if (power_mode == 1 || power_mode == 2) {
				$('.js-button-start-position').hide();
				$('.js-button-start-feed').hide();
				$('.js-button-start-program').show();
			//} else if (power_mode == 2) {
			//	$('.js-button-start-position').hide();
			//	$('.js-button-start-feed').show();
			//	$('.js-button-start-program').hide();
			}
		});
	});
	
	
	$('.card-archived').hide();
	var toggle_archived_card = $('.js-toggle-archived-card');
	toggle_archived_card.on('click', function(){
		var $this = $(this);
		if ($this.data('state')) {
			$('.card-archived').fadeOut(300);
			$this.text('Показать скрытые программы').data('state', false);
		} else {
			$('.card-archived').fadeIn(1000);
			$this.text('Скрыть программы').data('state', true);
		}
	});
	
	$('.js-delete-list-item').on('click', function(){
		var $this = $(this);
		var sid = $this.data('sid');
		var pid = $this.data('pid');
		var callback = function(response){
			if (response.error_code == 0) {
				$this.parents('.list-item-wrap').parent().fadeOut(500, function(){
					$(this).remove();
				});
			}
		};
		if (typeof sid != "undefined") {
			$.post(API_URL + 'shot.delete', {
				shot_id: sid
			}, callback);
		} else if (typeof pid != "undefined") {
			$.post(API_URL + 'position.delete', {
				position_id: pid
			}, callback);
		}
	}).addClass('display-none');

});

function getCookie(name) {
	var cookieValue = null;
	if (document.cookie && document.cookie != '') {
		var cookies = document.cookie.split(';');
		for (var i = 0; i < cookies.length; i++) {
			var cookie = jQuery.trim(cookies[i]);
			// Does this cookie string begin with the name we want?
			if (cookie.substring(0, name.length + 1) == (name + '=')) {
				cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
				break;
			}
		}
	}
	return cookieValue;
}

function on_resize_window(){
	if ($(this).width() < 800) {
		$('body').addClass('hide-menu');
	} else {
		$('body').removeClass('hide-menu');
	}
}

function open_menu() {
	$('body').addClass('show-menu');
}

function close_menu() {
	$('body').removeClass('show-menu');
}

function toogle_list_sort() {
	$('.list-item-wrap').toggleClass('swap');
}

function toogle_list_item_delete() {
	$('.js-delete-list-item').toggleClass('display-none');
}

function shutdown() {
	$('.dialog-progress').removeClass('display-none');
	$('.dialog-buttons').addClass('display-none');
	
	$('.dialog h3').text('Выключение тренажёра');
	var maxValue = 30; // 30 секунд.
	var currentValue = 0;
	var shutdownProgressInterval = setInterval(function(){
		currentValue += 1;
		$('.progress-value').css('width', (currentValue / maxValue) * 100 + '%');
		if (currentValue >= maxValue) {
			$('.dialog-progress').addClass('display-none');
			clearInterval(shutdownProgressInterval);
			$('.dialog h3').text('Тренажёр выключен');
		}
	}, 1000);
	
	$.post(API_URL + 'shutdown');
}


var last_terminal_id = 0;
var load_terminal_period_min = 1000;
var load_terminal_period_max = 5000;
var load_terminal_period = 2000;

function load_new_terminal_message() {
	if (typeof ajaxsetup == 'undefined' || !ajaxsetup) {
		setTimeout(load_new_terminal_message, load_terminal_period_min);		
	} else if (last_terminal_id == 0) {
		try {
			$.post(API_URL + 'terminal.getlast', {}, function(response){
				last_terminal_id = response.last_id;
				setTimeout(load_new_terminal_message, load_terminal_period_min);			
			});
		} catch (e) {
			setTimeout(load_new_terminal_message, load_terminal_period_max);	
			console.log(e);
		}
	} else {
		try {
			$.post(API_URL + 'terminal.getnew', {
				last_id: last_terminal_id
			}, function(response){
				if (response.elements_count == 0) {
					load_terminal_period += 500;
					if (load_terminal_period > load_terminal_period_max)
						load_terminal_period = load_terminal_period_max;
				} else {
					load_terminal_period = load_terminal_period_min;					
					last_terminal_id = response.last_id;
					var $thead = $('table thead');
					for (key in response.messages) {
						var msg = response.messages[key];
						$thead.after(terminal_row_tpl({
							msg: msg
						}));
					}
				}
				
				setTimeout(load_new_terminal_message, load_terminal_period);
			});
		} catch (e) {
			setTimeout(load_new_terminal_message, load_terminal_period_max);
			console.log(e);
		}
	}
}

function training_handle(tid) {
	setTimeout(function(){
		try {
			$.post(API_URL + 'training.start', {
				training_id: tid
			}, function(response){				
				if (response.error_code == 0) {
					$('.training-status span').text(response.state_print);
					
					if (response.state == 4) {
						training_handle(tid);
					} else if (response.state == 5) {
						$('#overpage > div').html(
							trainer_position_tpl({
								training_id: tid
							})
						);
						$('#overpage').show().addClass('darkness');
					} else if (response.game_url != undefined) {
						location.href = response.game_url;
					}
					
				} else {
					training_handle(tid);
				}
			});
		} catch (e) {
			training_handle(tid);
		}
	}, 1000);
}

function trainer_position_send(tid, x, y) {
	$.post(API_URL + 'trainer.position', {
		pos_x: x,
		pos_y: y,
		training_id: tid
	}, function(response){
		console.log(response);
		
		if (response.error_code == 0) {		
			if (response.game_url != undefined) {
				location.href = response.game_url;
			} else {
				$('#overpage').hide();
				training_handle(tid);
			}
		} else {
			setTimeout(function(){
				trainer_position_send(tid, x, y);
			}, 1000);
		}
	})
}

function game_play(gid) {
	$.post(API_URL + 'game.start', {
		game_id: gid
	});		
}

function game_pause(gid) {
	$.post(API_URL + 'game.pause', {
		game_id: gid
	});	
}

function game_stop(gid) {
	$.post(API_URL + 'game.stop', {
		game_id: gid
	});
}

function game_update(gid) {
	var btn_play = $('.js-button-play');
	var btn_pause = $('.js-button-pause');
	var btn_stop = $('.js-button-stop');
	var btn_replay = $('.js-button-replay');
	
	$.post(API_URL + 'game.status', {
		game_id: gid
	}, function(response){
		if (response.error_code == 0) {			
			$('.game-status span').text(response.state_print);
			
			console.log(response.positions);
			
			$('.positions').html('');
			for (key in response.positions) {
				var pos = response.positions[key];
				console.log(pos);
				$('.positions').append(position_item_tpl({
					position: pos
				}));
			}
			
			if (response.state <= 2) {
				btn_play.hide();
				btn_pause.hide();
				btn_stop.hide();
				
				btn_replay.show();
			} else {
				if (response.state == 3) {
					btn_pause.hide();
					
					btn_play.show();
					btn_stop.show();
				} else if (response.state == 8) {
					btn_play.hide();
					
					btn_stop.show();
					btn_pause.show();
				} else {
					btn_play.hide();
					btn_stop.hide();
					btn_pause.hide();
				}
				setTimeout(function(){game_update(gid)}, 1000);
			}
		} else {
			setTimeout(function(){game_update(gid)}, 1000);
		}
	})
}