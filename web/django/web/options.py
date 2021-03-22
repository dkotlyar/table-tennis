from .models import Option

def get_option(name, default):
	try:
		option = Option.objects.get(name=name)
		return option.value
	except:
		return default
		
def set_option(name, value):
	option,created = Option.objects.get_or_create(name=name)
	option.value = value
	option.save()

# ОПИСАНИЕ КОНСТАНТ ТРЕНАЖЁРА

# Реальные параметры устройств
ROLL_DIAMETER = 50			# диаметр роликов, мм
MAX_ROLL_ROTATION = 10800	# максимальная скорость вращения роликов, об/мин
MIN_ROLL_ROTATION = -10800	# минимальная скорость вращения роликов, об/мин
MAX_ROTATION = 60			# максимальный угол поворота пушки, градусов
MIN_ROTATION = -60			# минимальный угол поворота пушки, градусов
MAX_ROLL_ROTATION = 45		# максимальный угол поворота роликов, градусов
MIN_ROLL_ROTATION = -45		# минимальный угол поворота роликов, градусов
MAX_LIFT = 1000				# максимальная высота тренажёра, мм от уровня пола
MIN_LIFT = 1000				# минимальная высота тренажёра, мм от уровня пола

# Интерфейсные параметры устройств (для режима ручного управления)
#ROLL_COUNT = 2				# 1 - только верхний ролик, 2 - верхний и нижний ролик
#MAX_ROLL_ROTATION = 99		# максимальная скорость вращения роликов, об/мин
#MIN_ROLL_ROTATION = -99	# минимальная скорость вращения роликов, об/мин
#MAX_ROTATION = 99			# максимальный угол поворота пушки, градусов
#MIN_ROTATION = -99			# минимальный угол поворота пушки, градусов
#MAX_ROLL_ROTATION = 99		# максимальный угол поворота роликов, градусов
#MIN_ROLL_ROTATION = -99	# минимальный угол поворота роликов, градусов
#MAX_LIFT = 0				# максимальная высота тренажёра, мм от уровня пола
#MIN_LIFT = 0				# минимальная высота тренажёра, мм от уровня пола

# Интерфейсные константы
BALL_SPEED_UNIT = 'км/ч'	# единицы измерения скорости вылета мяча
BALL_SPEED_COEF = 3.6		# коэффициент скорости вылета (3.6 - для км/ч, 1 - для м/с)
BALL_SPEED_STEP = 10		# шаг изменения скорости вылета мяча

BALL_ROTATION_UNIT = 'об/мин'	# единицы измерения скорости вращения мяча
BALL_ROTATION_COEF = 60			# коэффициент скорости вращения (60 - для об/мин, 1 - для об/с)
BALL_ROTATION_STEP = 30			# шаг изменения скорости вращения мяча