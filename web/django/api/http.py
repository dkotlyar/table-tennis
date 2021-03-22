# -- coding: utf-8 --

def get_prop(request, prop, default = None):
	if prop in request.POST:
		return request.POST[prop]
	elif prop in request.GET:
		return request.GET[prop]
	else:
		if (default != None):
			return default
		else:
			raise Exception('No property %s' % prop)