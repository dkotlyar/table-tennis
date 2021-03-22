create table django_migrations
(
    id      int auto_increment
        primary key,
    app     varchar(255) not null,
    name    varchar(255) not null,
    applied datetime     not null
);

INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (1, 'contenttypes', '0001_initial', '1970-01-01 00:24:17');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (2, 'auth', '0001_initial', '1970-01-01 00:24:28');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (3, 'admin', '0001_initial', '1970-01-01 00:24:29');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (4, 'contenttypes', '0002_remove_content_type_name', '1970-01-01 00:24:29');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (5, 'auth', '0002_alter_permission_name_max_length', '1970-01-01 00:24:30');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (6, 'auth', '0003_alter_user_email_max_length', '1970-01-01 00:24:30');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (7, 'auth', '0004_alter_user_username_opts', '1970-01-01 00:24:30');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (8, 'auth', '0005_alter_user_last_login_null', '1970-01-01 00:24:30');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (9, 'auth', '0006_require_contenttypes_0002', '1970-01-01 00:24:30');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (10, 'control', '0001_initial', '1970-01-01 00:24:31');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (11, 'control', '0002_auto_20160808_1515', '1970-01-01 00:24:31');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (12, 'sessions', '0001_initial', '1970-01-01 00:24:31');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (13, 'web', '0001_initial', '1970-01-01 00:24:32');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (14, 'web', '0002_shot', '1970-01-01 00:24:34');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (15, 'web', '0003_trainingoption', '1970-01-01 00:24:36');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (16, 'web', '0004_auto_20160318_0642', '1970-01-01 00:24:37');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (17, 'web', '0005_auto_20160318_0649', '1970-01-01 00:24:37');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (18, 'web', '0006_remove_trainingoption_user', '1970-01-01 00:24:37');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (19, 'web', '0007_auto_20160318_0658', '1970-01-01 00:24:38');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (20, 'web', '0008_auto_20160318_0659', '1970-01-01 00:24:43');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (21, 'web', '0009_auto_20160329_1150', '1970-01-01 00:24:43');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (22, 'web', '0010_shot_sort_position', '1970-01-01 00:24:43');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (23, 'web', '0011_auto_20160329_2025', '1970-01-01 00:24:44');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (24, 'web', '0012_auto_20160330_0609', '1970-01-01 00:24:45');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (25, 'web', '0013_auto_20160330_1254', '1970-01-01 00:24:46');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (26, 'web', '0014_auto_20160330_1255', '1970-01-01 00:24:46');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (27, 'web', '0015_auto_20160421_1334', '1970-01-01 00:24:51');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (28, 'web', '0016_auto_20160421_1429', '1970-01-01 00:25:34');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (29, 'web', '0017_auto_20160421_1430', '1970-01-01 00:25:35');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (30, 'web', '0018_option', '1970-01-01 00:25:39');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (31, 'web', '0019_auto_20160425_1201', '1970-01-01 00:25:42');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (32, 'web', '0020_auto_20160616_0625', '1970-01-01 00:25:45');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (33, 'web', '0021_auto_20160616_1049', '1970-01-01 00:25:45');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (34, 'web', '0022_auto_20160617_1457', '1970-01-01 00:25:49');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (35, 'web', '0023_shot_zone', '1970-01-01 00:25:49');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (36, 'web', '0024_shot_rotation_type', '1970-01-01 00:25:50');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (37, 'web', '0025_auto_20160625_1043', '1970-01-01 00:25:51');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (38, 'web', '0026_position_name', '1970-01-01 00:25:53');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (39, 'web', '0027_position_game', '1970-01-01 00:25:53');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (40, 'web', '0028_auto_20160809_1449', '1970-01-01 00:25:55');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (41, 'web', '0029_position_before_delay', '1970-01-01 00:25:55');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (42, 'web', '0030_auto_20160815_1359', '1970-01-01 00:25:56');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (43, 'web', '0031_auto_20160816_1006', '1970-01-01 00:25:56');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (44, 'web', '0032_auto_20160816_1007', '1970-01-01 00:25:56');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (45, 'web', '0033_remove_training_state', '2016-11-23 06:56:43');
INSERT INTO orangepi.django_migrations (id, app, name, applied) VALUES (46, 'web', '0034_auto_19700101_0428', '1970-01-01 01:28:37');