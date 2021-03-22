create table auth_permission
(
    id              int auto_increment
        primary key,
    name            varchar(255) not null,
    content_type_id int          not null,
    codename        varchar(100) not null,
    constraint content_type_id
        unique (content_type_id, codename),
    constraint auth_permissi_content_type_id_51277a81_fk_django_content_type_id
        foreign key (content_type_id) references django_content_type (id)
);

INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (1, 'Can add log entry', 1, 'add_logentry');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (2, 'Can change log entry', 1, 'change_logentry');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (3, 'Can delete log entry', 1, 'delete_logentry');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (4, 'Can add permission', 2, 'add_permission');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (5, 'Can change permission', 2, 'change_permission');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (6, 'Can delete permission', 2, 'delete_permission');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (7, 'Can add group', 3, 'add_group');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (8, 'Can change group', 3, 'change_group');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (9, 'Can delete group', 3, 'delete_group');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (10, 'Can add user', 4, 'add_user');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (11, 'Can change user', 4, 'change_user');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (12, 'Can delete user', 4, 'delete_user');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (13, 'Can add content type', 5, 'add_contenttype');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (14, 'Can change content type', 5, 'change_contenttype');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (15, 'Can delete content type', 5, 'delete_contenttype');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (16, 'Can add session', 6, 'add_session');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (17, 'Can change session', 6, 'change_session');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (18, 'Can delete session', 6, 'delete_session');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (19, 'Can add training', 7, 'add_training');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (20, 'Can change training', 7, 'change_training');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (21, 'Can delete training', 7, 'delete_training');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (22, 'Can add shot', 8, 'add_shot');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (23, 'Can change shot', 8, 'change_shot');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (24, 'Can delete shot', 8, 'delete_shot');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (25, 'Can add game', 9, 'add_game');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (26, 'Can change game', 9, 'change_game');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (27, 'Can delete game', 9, 'delete_game');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (28, 'Can add position', 10, 'add_position');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (29, 'Can change position', 10, 'change_position');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (30, 'Can delete position', 10, 'delete_position');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (31, 'Can add message', 11, 'add_message');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (32, 'Can change message', 11, 'change_message');
INSERT INTO orangepi.auth_permission (id, name, content_type_id, codename) VALUES (33, 'Can delete message', 11, 'delete_message');