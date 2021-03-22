FROM python:2
#FROM django:1.9.1-python2

RUN pip install django==1.9.1 psycopg2>=2.6.1 pyserial

RUN  mkdir /code
WORKDIR /code

COPY django /code

RUN cd /code/web/management/commands && \
	python setup.py build_ext --inplace

CMD python manage.py runserver 0.0.0.0:8000

