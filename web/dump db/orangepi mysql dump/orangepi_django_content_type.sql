create table django_content_type
(
    id        int auto_increment
        primary key,
    app_label varchar(100) not null,
    model     varchar(100) not null,
    constraint django_content_type_app_label_3ec8c61c_uniq
        unique (app_label, model)
);

INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (1, 'admin', 'logentry');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (3, 'auth', 'group');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (2, 'auth', 'permission');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (4, 'auth', 'user');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (5, 'contenttypes', 'contenttype');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (11, 'control', 'message');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (6, 'sessions', 'session');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (9, 'web', 'game');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (10, 'web', 'position');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (8, 'web', 'shot');
INSERT INTO orangepi.django_content_type (id, app_label, model) VALUES (7, 'web', 'training');