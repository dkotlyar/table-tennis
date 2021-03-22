var slider_standart = {
	start: function($this, pos_x, percent) {
		$this.addClass('slider_active');
	},
	move: function($this, pos_x, percent){
		$this.parent().find('.slider_fill').css({
			width: pos_x + $this.width()/2,
			left: 0
		});
	},
	end: function($this, pos_x, percent){
		$this.parent().find('.slider_fill').css({
			width:pos_x + $this.width()/2,
			left: 0
		});
		$this.removeClass('slider_active');
	}
};

var slider_default_param = {
	bgcolor:	'#9E9E9E',
	fillcolor:	'#2196F3',
	pointcolor:	'#2196F3',
	
	margin: '10px 0',
	
	min: 0,
	max: 1
};

(function($){
	$.fn.slider = function(param) {

		this.each(function(){
			var mousedown = false;
			var $this = $(this);

			var parent = $this.parent();
			var offset_left = parent.offset().left;
			var offset_top = parent.offset().top;

			var position_x_percent = param.percent;
			var position_x_min = 0;
			var position_x_max = parent.width() - $this.width();
			var position_x = position_x_max * position_x_percent;
			$this.css('left', position_x);

			$(window).resize(function(){
				position_x_max = parent.width() - $this.width();
				position_x = position_x_percent * position_x_max;
				offset_left = parent.offset().left;
				offset_top = parent.offset().top;
				$this.css('left', position_x);
				
				if (param.end)
					param.end($this, position_x, position_x_percent, position_x_max);
			});

			if (param.init)
				param.init($this, position_x, position_x_percent, position_x_max);
			
			$this.on('mousedown touchstart', function() {
				if (mousedown) return false;
				
				mousedown = true;
				if (param.start)
					param.start($this, position_x, position_x_percent);
				
				$(document).on('mousemove.startslide', function(e){
					changePosition(e);
				}).on('touchmove.startslide', function(e){
					changePosition(e.originalEvent.touches[0]);
				});
			});
			$(document).on('mouseup touchend', function() {
				$(document).off('mousemove.startslide').off('touchmove.startslide');
				if (!mousedown) return false;
				
				mousedown = false;
				if (param.end)
					param.end($this, position_x, position_x_percent, position_x_max);
			});

			function changePosition(e) {
				//console.log(position_x);
				//if (!mousedown) return false;
				
				position_x = e.clientX - offset_left - $this.width()/2;
				if (position_x < position_x_min) position_x = position_x_min;
				if (position_x > position_x_max) position_x = position_x_max;
				position_x_percent = position_x / position_x_max;
				//$this.css('left', position_x);
				if (param.move)
					param.move($this, position_x, position_x_percent);
				
				
				e = (e || window.event);
				if (!e) return false;
				while (e.originalEvent) {
					e = e.originalEvent;
				}
				if (e.preventDefault) e.preventDefault();
				if (e.stopPropagation) e.stopPropagation();
				e.cancelBubble = true;
				e.returnValue = false;
				
				return false;
			}
		});
	};

	$.fn.toSlider = function(param) {
		var input = $(this);
		if (!input.is('input')) return false;
		
		if (param == undefined) param = slider_default_param;
		
		param.bgcolor		= param.bgcolor		!= undefined ? param.bgcolor	: slider_default_param.bgcolor;
		param.fillcolor		= param.fillcolor	!= undefined ? param.fillcolor	: slider_default_param.fillcolor;
		param.pointcolor	= param.pointcolor	!= undefined ? param.pointcolor	: slider_default_param.pointcolor;
		param.margin		= param.margin		!= undefined ? param.margin		: slider_default_param.margin;
		
		param.min = param.min != undefined ? param.min : slider_default_param.min;
		param.max = param.max != undefined ? param.max : slider_default_param.max;

		input_value = parseInt(input.val());
		input_value = isNaN(input_value) ? 0 : input_value;
		param.cur = param.cur != undefined ? param.cur : input_value;
		if (param.cur > param.max) param.cur = param.max;
		else if (param.cur < param.min) param.cur = param.min;
		param.percent = (param.cur - param.min) / (param.max - param.min);
		input.val(param.cur);	
		
		input.attr('type', 'hidden');
		input.after('<div></div>');
		
		var container = input.next();
		container.append('<div class="slider_wrap"><div class="slider_background"></div>' +
			'<div class="slider_fill"></div><div class="slider_point"><div></div></div></div>');
		
		container.css({
			margin: param.margin
		});		
		container.find('.slider_background').css({
			backgroundColor: param.bgcolor
		});
		container.find('.slider_fill').css({
			backgroundColor: param.fillcolor
		});
		container.find('.slider_point > div').css({
			backgroundColor: param.pointcolor
		});
		
		checkReadyInterval = setInterval(function() {
			if (container.width() == 0) return;
			clearInterval(checkReadyInterval);
			container.find('.slider_point').slider({
				start: function(point, pos_x, percent) {
					slider_standart.start(point, pos_x, percent);
					if (param.start)
						param.start(input, point, pos_x, percent);
				},
				move: function(point, pos_x, percent){
					slider_standart.move(point, pos_x, percent);
					param.cur = Math.round((param.max - param.min) * percent + param.min);
					input.val(param.cur);
					if (param.move)
						param.move(input, point, pos_x, percent);
				},
				end: function(point, pos_x, percent, pos_x_max) {
					param.cur = Math.round((param.max - param.min) * percent + param.min);
					param.percent = (param.cur - param.min) / (param.max - param.min);
					input.val(param.cur);

					pos_x = pos_x_max * param.percent;
					point.css({left: pos_x});
					
					slider_standart.end(point, pos_x, param.percent);
					if (param.end)
						param.end(input, point, pos_x, percent);
				},
				init: function(point, pos_x, percent) {
					slider_standart.end(point, pos_x, percent);
					if (param.init)
						param.init(input, point, pos_x, percent);
				},
				percent: isNaN(param.percent) ? 0 : param.percent
			});
		}, 10);
	}

})(jQuery);
