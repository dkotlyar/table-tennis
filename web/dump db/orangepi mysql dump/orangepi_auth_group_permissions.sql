create table auth_group_permissions
(
    id            int auto_increment
        primary key,
    group_id      int not null,
    permission_id int not null,
    constraint group_id
        unique (group_id, permission_id),
    constraint auth_group_permissi_permission_id_23962d04_fk_auth_permission_id
        foreign key (permission_id) references auth_permission (id),
    constraint auth_group_permissions_group_id_58c48ba9_fk_auth_group_id
        foreign key (group_id) references auth_group (id)
);

