# -*- coding: utf-8 -*- 

def get_position_parameters(game, shot):
	import math
	
	g = 9.81 # ускорение свободного падени€, м/с^2

	Kc = 0.37 # коэф сопротивлени€ воздуха
	Kw = -0.0001 # коэф вращени€
	Kalpha = 0.9 # коэф вли€ни€ угла
	dAlpha = 0.7 # погрешность угла
	
	tableX = 2740 # размер стола по оси X, мм
	tableY = 1525 # размер стола по оси Y, мм
	tableHeight = 760 # высота стола, мм
	netHeight = 152.5 # высота сетки, мм
	netX = (tableX / 2.0 + game.position_x) # расположение сетки, мм

	allowable_error = 0.05 # допустима€ погрешность, м (5см)
	net_stock = 50 # запас высоты над сеткой, мм
	table_stock = 50 # запас высоты над столом, мм

	A = float(1600 + 460 * (shot.zone / 4) + game.position_x)
	B = float(570 - 380 * (shot.zone % 4) - game.position_y)

	L = math.sqrt(A**2 + B**2) # длина полЄта м€ча, мм

	Zstart = 0 # высота от пола, мм
	Zend = 0   # предельна€ высота от пола, мм
	
	max_angle = 50
	
	if (shot.shot_type == 'under-table'):
		Zstart = 600
		Zend = 750
	elif (shot.shot_type == 'over-table'):
		Zstart = 800
		Zend = 1100
	elif (shot.shot_type == 'over-net'):
		Zstart = 1000
		Zend = 1250
	else:
		Zstart = 1200
		Zend = 1360
		# max_angle = 0
		
	Z0 = (Zstart - tableHeight) / 1000 # высота над столом, м

	rotation = int(round(math.asin(B/L) / math.pi * 180))
	rollrotation = shot.leteral_rotation * shot.rotation_type * 45

	V = shot.ball_speed / 3.6 # м/с
	W = shot.rotation_type * math.copysign(shot.ball_rotation, rollrotation) # скорость вращени€

	min_qrt = L**2 # минимальна€ ошибка, кв.мм
	angle_norm = 0 # угол, при котором минимальна ошибка
	V_norm = V
	W_norm = W
	Z_norm = Z0

	for dW in range(0, 3000, 500) + [3000,]:
		for signW in (1, -1):
			W_cur = W + signW*dW
			for dZ in range(0, (Zend - Zstart), 5):
				Z_cur = Z0 + dZ / 1000
				for dV in range(0, 100, 1):
					for signV in (1, -1):
						V_cur = V + signV * dV / 10
						for angle in range(-30, 50) + [50,]:
							angle_rad = (angle + dAlpha) * Kalpha / 180 * math.pi
							speed_z = V_cur * Kc * math.sin(angle_rad) + W_cur * Kw
							speed_x = V_cur * Kc * math.cos(angle_rad) + W_cur * Kw
							desc = speed_z**2 + 2 * g * Z_cur
							
							if (desc < 0): # отрицательный дескриминант, м€ч никогда не достигнет высоты стола
								continue
							
							t = (speed_z + math.sqrt(desc)) / g
							L_cur = speed_x * t * 1000 # расчЄтное теоретическое рассто€ние, мм
							
							tNet = (netX / 1000) / speed_x
							tTable = (game.position_x / 1000) / speed_x
							overNet = (Z_cur + speed_z * tNet - (g * (tNet**2)) / 2) * 1000 - netHeight # запас над сеткой, мм
							overTable = (Z_cur + speed_z * tTable - (g * (tTable**2)) / 2) * 1000 # запас над столом, мм
							
							# должен выполн€тьс€ запас высоты над сеткой 
							if (overNet < net_stock):
								continue
							# должен выполн€тьс€ запас высоты над столом 
							if (overTable < table_stock):
								continue
							
							eps = L_cur - L # ошибка, мм
							eps_qrt = eps**2 # квадрат ошибки, кв.мм
							
							if (eps_qrt < min_qrt):
								min_qrt = eps_qrt
								
								angle_norm = angle
								V_norm = V_cur
								Z_norm = Z_cur
								W_norm = W_cur
								
								# параметры наход€тс€ в допустимых пределах, дальше считать нет смысла
								if (eps_qrt < allowable_error**2):
									return {
										'rotation': rotation,
										'rollrotation': rollrotation,
										'speed': V_norm,
										'angular': W_norm,
										'angle': angle_norm,
										'Z': Z_norm * 1000 + tableHeight,
									}
	return {
		'rotation': rotation,
		'rollrotation': rollrotation,
		'speed': V_norm,
		'angular': W_norm,
		'angle': angle_norm,
		'Z': Z_norm * 1000 + tableHeight,
	}
								