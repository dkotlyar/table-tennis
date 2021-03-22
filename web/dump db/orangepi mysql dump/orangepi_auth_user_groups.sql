create table auth_user_groups
(
    id       int auto_increment
        primary key,
    user_id  int not null,
    group_id int not null,
    constraint user_id
        unique (user_id, group_id),
    constraint auth_user_groups_group_id_30a071c9_fk_auth_group_id
        foreign key (group_id) references auth_group (id),
    constraint auth_user_groups_user_id_24702650_fk_auth_user_id
        foreign key (user_id) references auth_user (id)
);

