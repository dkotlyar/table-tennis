create table web_shot
(
    id               int auto_increment
        primary key,
    name             varchar(50) not null,
    edit_dt          datetime    not null,
    create_dt        datetime    not null,
    archived         tinyint(1)  not null,
    training_id      int         not null,
    user_id          int         not null,
    sort_position    int         not null,
    ball_rotation    int         not null,
    ball_speed       int         not null,
    before_delay     int         not null,
    leteral_rotation int         not null,
    shot_type        varchar(16) not null,
    zone             int         not null,
    rotation_type    int         not null,
    constraint web_shot_training_id_38db85e3_fk_web_training_id
        foreign key (training_id) references web_training (id),
    constraint web_shot_user_id_29009e42_fk_auth_user_id
        foreign key (user_id) references auth_user (id)
);

INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (1, 'Новый удар', '1970-01-01 00:02:16', '2016-10-20 12:41:39', 0, 1, 1, 1, 0, 50, 1000, 2, 'over-net', 2, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (3, 'Тестовый удар', '1970-01-01 00:51:59', '2016-10-28 06:52:38', 0, 2, 1, 1, 0, 110, 200, 2, 'attack-shot', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (4, 'Второй удар', '1970-01-01 00:10:51', '2016-11-23 07:49:01', 0, 2, 1, 2, 700, 100, 400, 2, 'over-net', 4, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (6, 'Правый накат', '1970-01-01 00:01:07', '1970-01-01 01:18:34', 0, 3, 1, 1, 500, 60, 200, 2, 'over-net', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (7, 'Новый удар', '1970-01-01 00:02:06', '1970-01-01 00:17:15', 0, 3, 1, 2, 500, 60, 200, 2, 'over-net', 5, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (8, 'Новый удар', '1970-01-01 00:02:21', '1970-01-01 00:05:25', 0, 4, 1, 1, 2400, 150, 200, 1, 'attack-shot', 6, -1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (9, 'Первый', '1970-01-01 00:26:22', '1970-01-01 00:05:32', 0, 5, 1, 1, 5000, 150, 200, 1, 'over-net', 11, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (10, 'Нижнее', '1970-01-01 00:05:17', '1970-01-01 00:04:22', 0, 6, 1, 1, 5000, 50, 200, 2, 'over-net', 5, -1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (11, 'Верхнее', '1970-01-01 00:05:07', '1970-01-01 00:04:50', 0, 6, 1, 2, 5000, 50, 200, 2, 'over-net', 5, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (12, 'Плоский', '1970-01-01 00:05:39', '1970-01-01 00:05:21', 0, 6, 1, 3, 0, 150, 200, 2, 'over-net', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (13, '7 зона', '1970-01-01 00:03:05', '1970-01-01 00:02:44', 0, 7, 1, 1, 0, 50, 500, 2, 'over-net', 5, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (14, '1 зона', '1970-01-01 00:03:24', '1970-01-01 00:03:09', 0, 7, 1, 2, 0, 50, 500, 2, 'over-net', 11, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (15, '4 зона', '1970-01-01 00:03:43', '1970-01-01 00:03:29', 0, 7, 1, 3, 0, 50, 500, 2, 'over-net', 8, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (16, '7 зона', '1970-01-01 00:10:18', '1970-01-01 00:04:12', 0, 8, 1, 1, 3500, 100, 200, -1, 'over-net', 5, -1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (17, '5 зона', '1970-01-01 03:22:02', '1970-01-01 00:04:53', 0, 8, 1, 2, 5000, 70, 200, 1, 'attack-shot', 10, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (18, '4 зона', '1970-01-01 00:13:24', '1970-01-01 00:05:24', 0, 8, 1, 3, 0, 150, 200, 1, 'over-net', 8, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (19, '6 зона', '1970-01-01 00:17:18', '1970-01-01 00:15:54', 0, 9, 1, 1, 0, 120, 200, 2, 'over-net', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (20, 'Топ-спин', '1970-01-01 00:53:30', '1970-01-01 00:52:05', 0, 10, 1, 1, 5000, 24, 1000, 2, 'under-table', 10, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (21, 'Легкий удар', '1970-01-01 00:08:47', '1970-01-01 00:08:32', 0, 11, 1, 1, 0, 40, 1000, 2, 'over-net', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (22, 'Новый удар', '1970-01-01 00:03:52', '1970-01-01 00:03:40', 0, 12, 1, 1, 1000, 70, 1000, 2, 'over-net', 6, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (23, 'Расстрелять ', '1970-01-01 00:28:03', '1970-01-01 00:27:14', 0, 13, 1, 1, 5000, 150, 200, 1, 'over-net', 8, 1);
INSERT INTO orangepi.web_shot (id, name, edit_dt, create_dt, archived, training_id, user_id, sort_position, ball_rotation, ball_speed, before_delay, leteral_rotation, shot_type, zone, rotation_type) VALUES (24, 'Новый удар', '1970-01-01 00:43:26', '1970-01-01 00:42:33', 0, 14, 1, 1, 5000, 150, 200, 1, 'over-net', 8, 1);