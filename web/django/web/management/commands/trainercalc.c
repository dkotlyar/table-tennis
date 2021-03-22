#include <Python.h>
#include <math.h>
#include <stdio.h>

PyMODINIT_FUNC inittrainercalc(void);

// #define g 9.81 // ускорение свободного падения, м/с^2
// #define Kc 0.37 // коэф сопротивления воздуха
// #define Kw -0.0001 // коэф вращения
// #define Kalpha 0.9 // коэф влияния угла
// #define dAlpha 0.7 // погрешность угла

#define g 9.81 // ускорение свободного падения, м/с^2
#define Kc 0.38 // коэф сопротивления воздуха
#define Kw -0.0001 // коэф вращения
#define Kalpha 0.75 // коэф влияния угла
#define dAlpha 1 // погрешность угла

#define linear -0.476138
#define xLinear 1.38753
#define xQuadric -0.963364
#define exp_pow 0.460192

#define tableX 2.740 // размер стола по оси X, м
#define tableY 1.525 // размер стола по оси Y, м
#define tableHeight 0.760 // высота стола, м
#define netHeight 0.1525 // высота сетки, м
// #define netX (tableX / 2.0) // расположение сетки от края стола, м

#define net_stock 0.1 // запас высоты над сеткой, м
#define table_stock 0.05 // запас высоты над столом, м
#define allowable_error_qrt 0.05*0.05 // квадрат допустимой ошибки, м^2

// #define LINEAR_COEF 0.75

static PyObject* py_test( PyObject* self, PyObject* args ) {
	double a;
	double b;
	
	double answer;
	
	if (!PyArg_ParseTuple(args, "dd", &a, &b))
		return NULL;
	
	answer = a*b;
	
	return Py_BuildValue("(f, f, f)", answer, answer*answer, sqrt(answer));
}

static PyObject* py_calc_optimum(PyObject* self, PyObject* args) {
	double posX = 0; // расстояние тренажёра до стола
	double L; // заданное расстояние
	
	double V; // начальная скорость
	double Zstart; // начальная высота
	double Zend; // максимальная высота
	double W; // начальное вращение
	
	double Lcoef; // коэффициент линейного перемещения
	
	if (!PyArg_ParseTuple(args, "ddddddd", &posX, &L, &V, &Zstart, &Zend, &W, &Lcoef)) {
		return NULL;
	}
	
	posX = posX / 1000; // расстояние тренажёра до стола, м
	L = L / 1000; // заданное расстояние, м
	
	double netX = posX + (tableX / 2.0); // расположение от тренажёра до сетки, м
	
	double V_cur; // текущая скорость
	double Z_cur; // текущая высота
	double W_cur; // текущее вращение
	
	double angle_norm = 0;
	double V_norm = V;
	double Z_norm = Zstart;
	double W_norm = W;
	// double L_norm = 0;
	
	double min_qrt = L*L; // минимальная ошибка, м^2
	
	double dW;
	double signW;
	double dZ;
	double dV;
	double signV;
	double angle;
	
	for (dW = 0.0f; dW <= 3000.0f; dW += 500.0f) {
		for (signW = -1; signW <= 1; signW += 2) {
			W_cur = W + signW * dW;
			for (dZ = 0.0f; dZ <= (Zend - Zstart); dZ += 0.05f) {
				Z_cur = Zstart + dZ;
				for (dV = 0.0f; dV <= 15.0f; dV += 0.1f) {
					for (signV = -1; signV <= 1; signV += 2) {
						V_cur = V + signV * dV;
						if (V_cur < 0) {
							continue;
						}
						for (angle = -30.0f; angle <= 50.0f; angle += 1.0f) {
							
							// printf("V: %f, Z: %f, W: %f; a: %f\n", V_cur, Z_cur, W_cur, angle);
							
							double angle_rad = (angle + dAlpha) * Kalpha / 180.0 * M_PI;
							double speed_z = V_cur * Kc * sin(angle_rad) + W_cur * Kw;
							double speed_x = V_cur * Kc * cos(angle_rad) + W_cur * Kw;
							double desc = speed_z*speed_z + 2 * g * Z_cur;
							
							if (desc < 0) { // отрицательный дескриминант, мяч никогда не достигнет высоты стола
								continue;
							}
							
							double L_cur = speed_x * ((speed_z + sqrt(desc)) / g); // расчётное теоретическое расстояние, м
							L_cur = linear + xLinear*L_cur + xQuadric*L_cur*L_cur + exp_pow*pow(M_E, L_cur);
							L_cur = L_cur / Lcoef;
							
							
							double tNet = netX / speed_x;
							double overNet = (Z_cur + speed_z * tNet - (g * (tNet*tNet)) / 2) - netHeight; // запас над сеткой, м
							//overNet = overNet * Lcoef;
							
							double tTable = posX / speed_x;
							double overTable = (Z_cur + speed_z * tTable - (g * (tTable*tTable)) / 2); // запас над столом, м
							//overTable = overTable * Lcoef;
							
							// должен выполняться запас высоты над сеткой 
							if (overNet < net_stock) {
								continue;
							}
							// должен выполняться запас высоты над столом 
							if (overTable < table_stock) { 
								continue;
							}
							
							double eps_qrt = (L_cur - L)*(L_cur - L); // квадрат ошибки, кв.мм
								
							if (eps_qrt < min_qrt) {
								min_qrt = eps_qrt;
								
								angle_norm = angle;
								V_norm = V_cur;
								Z_norm = Z_cur;
								W_norm = W_cur;
								// L_norm = L_cur;
								
								// параметры находятся в допустимых пределах, дальше считать нет смысла
								if (eps_qrt < allowable_error_qrt) {
									// printf("FINISH\n");
									// printf("V: %f, Z: %f, W: %f; a: %f\n", V_norm, Z_norm, W_norm, angle_norm);
									// printf("L_cur: %f\n", L_norm);
									return Py_BuildValue("(f,f,f,f, b)", V_norm, Z_norm, W_norm, angle_norm, 1);
								}
							}
						}
					}
				}
			}
		}
	}
	
	// printf("No match\n");
	// printf("V: %f, Z: %f, W: %f; a: %f\n", V_norm, Z_norm, W_norm, angle_norm);
	// printf("L_cur: %f\n", L_norm);
	return Py_BuildValue("(f,f,f,f, b)", V_norm, Z_norm, W_norm, angle_norm, 0);
	
}

static PyMethodDef trainercalc_methods[] = {
	{ "test", py_test, METH_VARARGS, "Function test work" },
	{ "calc_optimum", py_calc_optimum, METH_VARARGS, "Function calc optimum parameters to shot" },
	{ NULL, NULL }
};

PyMODINIT_FUNC inittrainercalc() {
	(void)Py_InitModule( "trainercalc", trainercalc_methods );
}