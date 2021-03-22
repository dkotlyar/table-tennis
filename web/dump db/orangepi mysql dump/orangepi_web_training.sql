create table web_training
(
    id       int auto_increment
        primary key,
    name     varchar(50) not null,
    edit_dt  datetime    not null,
    creat_dt datetime    not null,
    archived tinyint(1)  not null,
    user_id  int         not null,
    `repeat` int         not null,
    role_max varchar(4)  not null,
    role_min varchar(4)  not null,
    run_dt   datetime    not null,
    constraint web_training_user_id_311249c4_fk_auth_user_id
        foreign key (user_id) references auth_user (id)
);

INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (1, 'Тестовая программа', '1970-01-01 00:20:55', '2016-10-20 12:41:22', 1, 1, 2, 'none', 'none', '2016-10-20 12:41:22');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (2, 'Тест зон попадания', '1970-01-01 00:01:42', '2016-10-28 06:52:28', 1, 1, 1, 'none', 'none', '2016-10-28 06:52:28');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (3, 'Правый накат', '1970-01-01 00:01:56', '1970-01-01 01:18:31', 0, 1, 3, 'none', 'none', '1970-01-01 01:18:31');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (4, 'Мощная тренировка', '1970-01-01 00:11:50', '1970-01-01 00:05:16', 0, 1, 3, 'none', 'none', '1970-01-01 00:05:16');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (5, 'Программа тестирования', '1970-01-01 00:02:33', '1970-01-01 00:02:59', 1, 1, 1, 'none', 'none', '1970-01-01 00:02:59');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (6, 'Пример', '1970-01-01 00:10:44', '1970-01-01 00:04:19', 0, 1, 1, 'none', 'none', '1970-01-01 00:04:19');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (7, 'Треугольник простой', '1970-01-01 00:09:54', '1970-01-01 00:02:38', 0, 1, 3, 'none', 'none', '1970-01-01 00:02:38');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (8, 'Треугольник сложный', '2021-02-19 17:37:59', '1970-01-01 00:04:04', 0, 1, 3, 'none', 'none', '1970-01-01 00:04:04');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (9, 'Расстрел', '1970-01-01 00:48:40', '1970-01-01 00:15:51', 0, 1, 6, 'none', 'none', '1970-01-01 00:15:51');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (10, 'Топ-спин', '1970-01-01 00:11:17', '1970-01-01 00:52:01', 0, 1, 1, 'none', 'none', '1970-01-01 00:52:01');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (11, 'Демо', '1970-01-01 00:11:34', '1970-01-01 00:08:26', 0, 1, 5, 'none', 'none', '1970-01-01 00:08:26');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (12, 'Test', '1970-01-01 00:04:02', '1970-01-01 00:03:35', 0, 1, 1, 'none', 'none', '1970-01-01 00:03:35');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (13, 'Иосиф Виссарионович', '1970-01-01 00:26:57', '1970-01-01 00:26:57', 0, 1, 1, 'none', 'none', '1970-01-01 00:26:57');
INSERT INTO orangepi.web_training (id, name, edit_dt, creat_dt, archived, user_id, `repeat`, role_max, role_min, run_dt) VALUES (14, 'Тест1', '1970-01-01 00:42:25', '1970-01-01 00:42:25', 0, 1, 1, 'none', 'none', '1970-01-01 00:42:25');