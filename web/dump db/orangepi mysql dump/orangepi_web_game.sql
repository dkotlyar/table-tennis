create table web_game
(
    id           int auto_increment
        primary key,
    position_x   int      not null,
    position_y   int      not null,
    run_dt       datetime not null,
    stop_dt      datetime not null,
    state        int      not null,
    training_id  int      null,
    user_id      int      not null,
    repeat_count int      not null,
    constraint web_game_training_id_7814d27d_fk_web_training_id
        foreign key (training_id) references web_training (id),
    constraint web_game_user_id_39183cdc_fk_auth_user_id
        foreign key (user_id) references auth_user (id)
);

create index web_game_e160a0b9
    on web_game (training_id);

create index web_game_e8701ad4
    on web_game (user_id);

INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (25, 150, 0, '1970-01-01 00:09:26', '1970-01-01 00:09:32', 1, 7, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (26, 150, 0, '1970-01-01 00:09:48', '1970-01-01 00:09:54', 1, 7, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (27, 150, 0, '1970-01-01 00:10:07', '1970-01-01 00:10:15', 1, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (28, 150, 0, '1970-01-01 00:10:35', '1970-01-01 00:10:44', 1, 6, 1, 1);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (29, 150, 0, '1970-01-01 00:11:12', '1970-01-01 00:11:17', 1, 10, 1, 1);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (30, 150, 0, '1970-01-01 00:11:29', '1970-01-01 00:11:34', 1, 11, 1, 5);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (31, 150, 0, '1970-01-01 00:11:45', '1970-01-01 00:11:50', 1, 4, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (32, 150, 0, '2021-02-19 17:06:03', '2021-02-19 17:24:17', 0, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (33, 150, 0, '2021-02-19 17:09:03', '2021-02-19 17:24:17', 0, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (34, 150, 0, '2021-02-19 17:27:07', '2021-02-19 17:27:22', 1, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (35, 150, 0, '2021-02-19 17:28:37', '2021-02-19 17:28:45', 1, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (36, 150, 0, '2021-02-19 17:31:35', '2021-02-19 17:32:53', 0, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (37, 150, 0, '2021-02-19 17:37:31', '2021-02-19 17:37:48', 0, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (38, 150, 0, '2021-02-19 17:37:52', '2021-02-19 17:37:59', 1, 8, 1, 3);
INSERT INTO orangepi.web_game (id, position_x, position_y, run_dt, stop_dt, state, training_id, user_id, repeat_count) VALUES (39, 150, 0, '2021-02-19 17:40:23', '2021-02-19 17:40:26', 6, 8, 1, 3);