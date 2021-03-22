from distutils.core import setup, Extension

module1 = Extension( 'trainercalc', sources = ['trainercalc.c'] )

setup(	name = 'trainercalc',
		version = '1.0',
		description = 'Module calc shot to table tennis trainer',
		ext_modules = [module1]
	)