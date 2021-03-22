# -- coding: utf-8 --

from struct import unpack, pack

OPERAND_TYPE = {
	'none':		('none', 'Нет операнда'),
	'int8':		('<b', '8 бит со знаком'),
	'uint8':	('<B', '8 бит без знака'),
	'int16':	('<h', '16 бит со знаком'),
	'uint16':	('<H', '16 бит без знака'),
	'int32':	('<i', '32 бита со знаком'),
	'uint32':	('<I', '32 бита без знака'),
	# ('<f', '4 бит вещественный'),
	# ('<d', '8 бит вещественный'),
}

PROTOCOL_COMMANDS = {
	# КОД: Имя, Идентификатор, Единицы измерения, Тип операнда
	0x00: ('Пустая команда', 'empty-command', '', ''),
	
	0x01: ('Управление светодиодом', 'led', '', '<B'),
	
	0x02: ('Установить режим работы', 'set-mode', '', '<B'),
	0x03: ('Запустить программу', 'start-program', '', ''),
	0x04: ('Остановить программу', 'stop-program', '', ''),
	0x05: ('Приостановить программу', 'pause-program', '', ''),
	0x06: ('Очистить текущую программу тренировки', 'clear-program', '', ''),
	0x07: ('Добавить удар в конец программы', 'append-shot', '', '<B'),
	0x08: ('Обновить удар', 'update-shot', '', '<B'),
	0x09: ('Количество повторений', 'repeat-count', '', '<B'),
	
	0x0A: ('Позиция тренажёра относительно стола', 'trainer-position', 'мм', ['<h','<h']),
	0x0B: ('Позиция удара мяча об стол', 'ball-shot-position', 'мм', ['<h', '<h']),
	0x0C: ('Скорость вылета мяча', 'ball-speed', 'мм/с', '<H'),
	
	0x20: ('Установить высоту пушки', 'lift', 'мм', '<H'),
	0x21: ('Повернуть пушку', 'rotation', '°', '<h'),
	0x22: ('Наклонить ствол', 'incline', '°', '<b'),
	0x23: ('Повернуть ствол', 'rollrotation', '°', '<h'),
	
	0x25: ('Установить скорость верхнего ролика', 'top-roll-speed', 'об/мин', '<h'),
	0x26: ('Установить скорость верхнего ролика', 'top-roll-width', '%', '<h'),
	0x27: ('Остановить верхний ролик', 'top-roll-stop', '', ''),
	0x28: ('Выключить верхний ролик', 'top-roll-disable', '', ''),
	0x29: ('Установить скорость нижнего ролика', 'bottom-roll-speed', 'об/мин', '<h'),
	0x2A: ('Установить скорость нижнего ролика', 'bottom-roll-width', '%', '<h'),
	0x2B: ('Остановить нижний ролик', 'bottom-roll-stop', '', ''),
	0x2C: ('Выключить нижний ролик', 'bottom-roll-disable', '', ''),
	
	0x30: ('Включить двигатель', 'enable-motor', '', '<B'),
	#0x31: ('Включить все двигатели', 'enable-all-motors', '', ''),
	0x32: ('Выключить двигатель', 'disable-motor', '', '<B'),
	#0x33: ('Выключить все двигатели', 'disable-all-motors', '' ,''),
	0x34: ('Позиционировать двигатель', 'positioning-motor', '', '<B'),
	#0x35: ('Позиционировать все двигатели', 'positioning-all-motors', '', ''),
	0x35: ('Сохранить нулевое положение двигателя', 'save-zero-motor', '', ''),
	0x36: ('Очистить нулевое положение двигателя', 'clear-zero-motor', '' ,''),
	
	0x51: ('Вывести на экран', 'display-change', '', ''),
	0x52: ('Добавить в конец строки', 'display-append', '', ''),
	0x53: ('Добавить в произвольное место', 'display-append-index', '', ''),
	0x54: ('Скорость прокрутки строки', 'display-speed', '', '<B'),
	
	0xF1: ('Команда для модуля №1', 'to-module-1', '', ''),
	0xF2: ('Команда для модуля №2', 'to-module-2', '', ''),
	0xF3: ('Команда для модуля №3', 'to-module-3', '', ''),
	0xF4: ('Команда для модуля №4', 'to-module-4', '', ''),
	0xF5: ('Команда для модуля №5', 'to-module-5', '', ''),
	0xF6: ('Команда для модуля №6', 'to-module-6', '', ''),
	0xF7: ('Команда для модуля №7', 'to-module-7', '', ''),
	
	0xFD: ('Команда запроса', 'get', '', ''),
	0xFE: ('Команда из расширенного набора', 'extended', '', ''),
	0xFF: ('Ошибка', 'error', '', ''),
}

def find_command(command):
	for key,value in PROTOCOL_COMMANDS.items():
		if value[1] == command:
			return key
	return 0
		
def byte_array_to_value(operand_type, operand):
	try:
		operand_type = str(OPERAND_TYPE[operand_type][0])
	except:	
		operand_type = str(operand_type)
	try:
		# TODO: chr не умеет в отрицательные числа
		packed = ''.join(map(chr, operand))
		return unpack(operand_type, packed)[0]
	except:
		return 0

def value_to_byte_array(operand_type, operand):
	try:
		operand_type = str(OPERAND_TYPE[operand_type][0])
	except:	
		operand_type = str(operand_type)
		
	try:
		packed = pack(operand_type, float(operand))
	except Exception, e:
		return []
	else:
		return map(ord, packed)
