create table auth_user_user_permissions
(
    id            int auto_increment
        primary key,
    user_id       int not null,
    permission_id int not null,
    constraint user_id
        unique (user_id, permission_id),
    constraint auth_user_user_perm_permission_id_3d7071f0_fk_auth_permission_id
        foreign key (permission_id) references auth_permission (id),
    constraint auth_user_user_permissions_user_id_7cd7acb6_fk_auth_user_id
        foreign key (user_id) references auth_user (id)
);

