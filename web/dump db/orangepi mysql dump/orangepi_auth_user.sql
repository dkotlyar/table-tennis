create table auth_user
(
    id           int auto_increment
        primary key,
    password     varchar(128) not null,
    last_login   datetime     null,
    is_superuser tinyint(1)   not null,
    username     varchar(30)  not null,
    first_name   varchar(30)  not null,
    last_name    varchar(30)  not null,
    email        varchar(254) not null,
    is_staff     tinyint(1)   not null,
    is_active    tinyint(1)   not null,
    date_joined  datetime     not null,
    constraint username
        unique (username)
);

INSERT INTO orangepi.auth_user (id, password, last_login, is_superuser, username, first_name, last_name, email, is_staff, is_active, date_joined) VALUES (1, 'pbkdf2_sha256$20000$nQlU2mpuJ8j3$L0kSm7whQ42BnWgQtFSGcetIEfhkMPdIMdg7YNLJHic=', '2021-02-19 16:59:19', 1, 'admin', '', '', '', 1, 1, '1970-01-01 00:33:34');