/*
** EPITECH PROJECT, 2024
** distance
** File description:
** distance
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "distance.h"

list_t *alloc_list(int nbr, list_t *left, list_t *right)
{
    list_t *list = malloc(sizeof(list_t));

    if (list == NULL)
        return NULL;
    list->location_id = nbr;
    list->left = left;
    list->right = right;
    return list;
}

location_list_t *alloc_loc_lis(void)
{
    location_list_t *list = malloc(sizeof(location_list_t));

    if (list == NULL)
        return NULL;
    list->left_list = alloc_list(__INT32_MAX__, NULL, NULL);
    list->right_list = alloc_list(__INT32_MAX__, NULL, NULL);
    list->distance = 0;
    return list;
}

void sort_in_list(list_t **list, int nbr)
{
    list_t *new_node = NULL;

    if (!list || !(*list))
        return;
    while ((*list)->right != NULL) {
        if ((*list)->location_id >= nbr)
            break;
        (*list) = (*list)->right;
    }
    new_node = alloc_list(nbr, (*list)->left, (*list));
    if ((*list)->left != NULL)
        (*list)->left->right = new_node;
    (*list)->left = new_node;
    while ((*list)->left != NULL)
        (*list) = (*list)->left;
}

void add_to_list(list_t **l_list, list_t **r_list, char *line)
{
    char *left_str = NULL;
    char *right_str = NULL;
    int left_nbr = 0;
    int right_nbr = 0;

    left_str = strtok(line, " ");
    right_str = strtok(NULL, "\n") + 2;
    left_nbr = atoi(left_str);
    right_nbr = atoi(right_str);
    sort_in_list(l_list, left_nbr);
    sort_in_list(r_list, right_nbr);
}

location_list_t *parse_location_list(char *filepath)
{
    FILE *fp = fopen(filepath, "r");
    char *line = NULL;
    size_t len = 0;
    location_list_t *list = alloc_loc_lis();

    if (!fp || !list || !list->left_list || !list->right_list)
        return NULL;
    while (getline(&line, &len, fp) > 0)
            add_to_list(&list->left_list, &list->right_list, line);
    free(line);
    fclose(fp);
    return list;
}

unsigned int find_distance(location_list_t *list)
{
    int sub = 0;
    unsigned int distance = 0;

    while (list->left_list->location_id != __INT32_MAX__) {
        sub = list->left_list->location_id - list->right_list->location_id;
        distance = abs(sub);
        list->left_list = list->left_list->right;
        list->right_list = list->right_list->right;
    }
    return distance;
}

void free_list(list_t *list) {
    if (!list)
        return;
    while (list->left != NULL) {
        list = list->left;
        free(list->right);
    }
    free(list);
}

void free_loc_list(location_list_t *list) {
    free_list(list->left_list);
    free_list(list->right_list);
    free(list);
}

int main(int argc, char **argv)
{
    location_list_t *loc_list = parse_location_list(argv[1]);
    unsigned int distance = find_distance(loc_list);
    printf("The total distance between your list is : [%i]\n", distance);
    free_loc_list(loc_list);
    return 0;
}