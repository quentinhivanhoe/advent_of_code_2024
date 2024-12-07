/*
** EPITECH PROJECT, 2024
** distance
** File description:
** distance
*/

#ifndef DISTANCE_H_
    #define DISTANCE_H_

typedef struct list_s {
    int location_id;
    struct list_s *left;
    struct list_s *right;
} list_t;

typedef struct location_list_s {
    list_t *left_list;
    list_t *right_list;
    unsigned int distance;
} location_list_t;

#endif /* !DISTANCE_H_ */
