create table auth_group
(
    id   int auto_increment
        primary key,
    name varchar(80) not null,
    constraint name
        unique (name)
);

