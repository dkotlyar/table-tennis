# -*- coding: utf-8 -*- 

def get_position_parameters(game, shot):
	import math
	
	g = 9.81 # ��������� ���������� �������, �/�^2

	Kc = 0.37 # ���� ������������� �������
	Kw = -0.0001 # ���� ��������
	Kalpha = 0.9 # ���� ������� ����
	dAlpha = 0.7 # ����������� ����
	
	tableX = 2740 # ������ ����� �� ��� X, ��
	tableY = 1525 # ������ ����� �� ��� Y, ��
	tableHeight = 760 # ������ �����, ��
	netHeight = 152.5 # ������ �����, ��
	netX = (tableX / 2.0 + game.position_x) # ������������ �����, ��

	allowable_error = 0.05 # ���������� �����������, � (5��)
	net_stock = 50 # ����� ������ ��� ������, ��
	table_stock = 50 # ����� ������ ��� ������, ��

	A = float(1600 + 460 * (shot.zone / 4) + game.position_x)
	B = float(570 - 380 * (shot.zone % 4) - game.position_y)

	L = math.sqrt(A**2 + B**2) # ����� ����� ����, ��

	Zstart = 0 # ������ �� ����, ��
	Zend = 0   # ���������� ������ �� ����, ��
	
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
		
	Z0 = (Zstart - tableHeight) / 1000 # ������ ��� ������, �

	rotation = int(round(math.asin(B/L) / math.pi * 180))
	rollrotation = shot.leteral_rotation * shot.rotation_type * 45

	V = shot.ball_speed / 3.6 # �/�
	W = shot.rotation_type * math.copysign(shot.ball_rotation, rollrotation) # �������� ��������

	min_qrt = L**2 # ����������� ������, ��.��
	angle_norm = 0 # ����, ��� ������� ���������� ������
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
							
							if (desc < 0): # ������������� ������������, ��� ������� �� ��������� ������ �����
								continue
							
							t = (speed_z + math.sqrt(desc)) / g
							L_cur = speed_x * t * 1000 # ��������� ������������� ����������, ��
							
							tNet = (netX / 1000) / speed_x
							tTable = (game.position_x / 1000) / speed_x
							overNet = (Z_cur + speed_z * tNet - (g * (tNet**2)) / 2) * 1000 - netHeight # ����� ��� ������, ��
							overTable = (Z_cur + speed_z * tTable - (g * (tTable**2)) / 2) * 1000 # ����� ��� ������, ��
							
							# ������ ����������� ����� ������ ��� ������ 
							if (overNet < net_stock):
								continue
							# ������ ����������� ����� ������ ��� ������ 
							if (overTable < table_stock):
								continue
							
							eps = L_cur - L # ������, ��
							eps_qrt = eps**2 # ������� ������, ��.��
							
							if (eps_qrt < min_qrt):
								min_qrt = eps_qrt
								
								angle_norm = angle
								V_norm = V_cur
								Z_norm = Z_cur
								W_norm = W_cur
								
								# ��������� ��������� � ���������� ��������, ������ ������� ��� ������
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
								