from django.shortcuts import render

def registration(request):
	return render(request, 'accounts/registration.html')

def profile(request):
	return render(request, 'accounts/profile.html')
