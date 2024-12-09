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
#define HASHMAP_KEY 256

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

hashmap_t **alloc_hashmap(unsigned int key)
{
    hashmap_t **map = malloc(sizeof(hashmap_t *) * (key + 1));

    if (!map)
        return NULL;
    printf("%i\n", key);
    for (int i = 0; i < key; i++) {
        map[i] = malloc(sizeof(hashmap_t));
        map[i]->list = alloc_list(__INT32_MAX__, NULL, NULL);
    }
    map[key] = NULL;
    return map;
}

void add_in_hashmap(hashmap_t **map, unsigned int key, int nbr)
{
    unsigned int index = nbr % key;

    if (!map)
        return;
    sort_in_list(&map[index]->list, nbr);
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

void add_to_list(hashmap_t **map, list_t **list, char *line)
{
    char *left_str = NULL;
    char *right_str = NULL;
    int left_nbr = 0;
    int right_nbr = 0;

    left_str = strtok(line, " ");
    right_str = strtok(NULL, "\n") + 2;
    left_nbr = atoi(left_str);
    right_nbr = atoi(right_str);
    add_in_hashmap(map, HASHMAP_KEY, right_nbr);
    sort_in_list(list, left_nbr);
    // sort_in_list(l_list, left_nbr);
    // sort_in_list(r_list, right_nbr);
}

unsigned int find_similarity(hashmap_t **map, list_t *list)
{
    unsigned int index = 0;
    unsigned int sim = 0;
    int occ = 0;
    void * save_ptr = NULL;

    while (list->location_id != __INT32_MAX__) {
        index = list->location_id % HASHMAP_KEY;
        save_ptr = map[index]->list;
        while (map[index]->list->location_id != __INT32_MAX__) {
            if (map[index]->list->location_id == list->location_id)
                occ++;
            map[index]->list = map[index]->list->right;
        }
        sim += list->location_id * occ;
        occ = 0;
        map[index]->list = save_ptr;
        list = list->right;
    }
    printf("the number of similaritude is : [%i]\n", sim);
    return sim;
}

hashmap_t **parse_location_list(char *filepath)
{
    FILE *fp = fopen(filepath, "r");
    char *line = NULL;
    size_t len = 0;
    hashmap_t **map =alloc_hashmap(256);
    list_t *left_list = alloc_list(__INT32_MAX__, NULL, NULL);

    if (!fp || !map)
        return NULL;
    while (getline(&line, &len, fp) > 0)
            add_to_list(map, &left_list, line);
    find_similarity(map, left_list);
    free(line);
    fclose(fp);
    return map;
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
    /*location_list_t *loc_list*/hashmap_t **map = parse_location_list(argv[1]);
    // unsigned int distance = find_distance(loc_list);
    // printf("The total distance between your list is : [%i]\n", distance);
    // free_loc_list(loc_list);
    for (int i = 0; map[i]; i++) {
        printf("map[%i] :", i);
        while (map[i]->list->location_id != __INT32_MAX__) {
            printf("-> [%i] ", map[i]->list->location_id);
            map[i]->list = map[i]->list->right;
        }
        printf("\n");
    }
    return 0;
}