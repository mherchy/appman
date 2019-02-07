#include "main_enemy.h"

/**
 * SHM
 * @see shm_ini.c
 */
extern int shm_overview_id, shm_lastkey_id, shm_map_id;

/**
 * Fonction principale d'un thread IA
 * Sa durée de vie est de 1 rafraichissement
 * @param param
 * @return
 */
void *main_enemy(void *param) {

    /**
     * INITIALISATION
     */
    t_enemy *mind = (t_enemy *) param;
    int id = mind->id;


    //SHM ET SEM
    //MAP
    t_map_unit *shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_t *sem_map = sem_open(SHM_SEM_MAP, O_RDWR);
    //OVERVIEW
    t_overview *shared_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_t *sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);


    /**
     * TRAVAIL
     */

    //Déclarations de t_pos
    t_pos pos_app, pos_obj;


    //recuperation de la position de l'app
    sem_wait(sem_overview);
    pos_app.x = shared_overview->app.x;
    pos_app.y = shared_overview->app.y;
    sem_post(sem_overview);

    //Traitement en fonction des subtilités de l'énemies
    //TODO : ajouter des subtilités pour chaque énemie
    pos_obj = pos_app;

    //Choix de la prochaine direction, inscription dans mind
    choose_next_direction(mind, &pos_obj, shared_map, sem_map);

    //L'énemie se déplace (dans sa tête)
    mind->pos.x += mind->vit.x;
    mind->pos.y += mind->vit.y;


    /**
     * END
     */


    //Détachement de la mémoire partagée
    shmdt(shared_map);
    shmdt(shared_overview);

    //Et des sémaphores
    sem_close(sem_map);
    sem_close(sem_overview);

    //Cloture du thread
    pthread_exit(EXIT_SUCCESS);

}

/**
 * CHOIX DE LA PROCHAINE DIRECTION
 * Directions envisageables : un chemin uniquement
 *
 * Le demi-tours est interdit
 *
 * Algorithme de choix de la prochaine direction :
 * Calcul d'un vecteur de déplacement idéale vers l'objectif
 * Calcul ensuite des 2 projetés en x et y
 *
 * Plusieurs Plans possibles, plusieurs directions :
 * A : Le projeté le plus grand, s'il fait partie des destinations envisageable
 * B : Le second projeté plus petit
 * C : Une des autres directions possibles
 *
 * @param e
 * @param obj
 * @param map
 * @param sem_map
 * @return t_vit *
 */
t_vit *choose_next_direction(t_enemy *e, t_pos *obj, t_map map, sem_t *sem_map) {


    //Vitesse à retourner
    t_vit *vit_chosen = &(e->vit);


    //random à initialiser (une seule fois)
    srand((unsigned) time(NULL));




    /**
     * OPERATIONS PRELIMINAIRES
     */


    //Utilisation de la map
    sem_wait(sem_map);

    //Cas de l'enemy sur l'objectif
    if (e->pos.x == obj->x && e->pos.y == obj->y) return -1; //TODO
    //Sortie de jeu
    if (get_pos(e->pos.x, e->pos.y, map) == E_WALL) return -1; //TODO ERREUR

    //prendre en compte les directions possibles à la set_vit(0,1,v)e
    t_cross cross;
    get_crossroad(&e->pos, &cross, map);

    sem_post(sem_map);




    //On supprime la possibilité du demi-tours
    if (e->vit.x != 0) (e->vit.x > 0) ? (cross.left = 0) : (cross.right = 0);
    else (e->vit.y > 0) ? (cross.top = 0) : (cross.bottom = 0);



    //repère centré sur l'ennemi
    t_pos dir;
    dir.x = obj->x - e->pos.x;
    dir.y = obj->y - e->pos.y;





    //On calcule la direction dans laquelle se situe l'app
    int ratio = abs(dir.x) - abs(dir.y);


    //Cas de la diagonale parfaite
    if (ratio == 0) {
        if (rand() % 2) ratio += 1;
        else ratio -= 1;
    }




    /**
     * PLAN A
     */


    //l'app est sur l'horizontale
    if (ratio > 0) {
        if (dir.x < 0 && cross.left) return set_vit(-1, 0, vit_chosen);
        if (dir.x > 0 && cross.right) return set_vit(1, 0, vit_chosen);
    }
        //l'app est sur la verticale
    else {
        if (dir.y < 0 && cross.top) return set_vit(0, -1, vit_chosen);
        if (dir.y > 0 && cross.bottom) return set_vit(0, 1, vit_chosen);
    }




    /**
     * PLAN B
     */


    if (ratio < 0) {
        if (dir.x < 0 && cross.left) return set_vit(-1, 0, vit_chosen);
        if (dir.x > 0 && cross.right) return set_vit(1, 0, vit_chosen);
    } else {
        if (dir.y < 0 && cross.top) return set_vit(0, -1, vit_chosen);
        if (dir.y > 0 && cross.bottom) return set_vit(0, 1, vit_chosen);
    }





    /**
     * PLAN C
     */

    t_vit routes_possibles[2];
    int nb_routes_possibles = 0;
    int route_choisie;

    if (cross.top) {
        routes_possibles[nb_routes_possibles].x = 0;
        routes_possibles[nb_routes_possibles].y = -1;
        nb_routes_possibles++;
        //return set_vit(0,-1,v);
    }
    if (cross.bottom) {
        routes_possibles[nb_routes_possibles].x = 0;
        routes_possibles[nb_routes_possibles].y = 1;
        nb_routes_possibles++;
        //return set_vit(0,1,v);
    }
    if (cross.left) {
        routes_possibles[nb_routes_possibles].x = -1;
        routes_possibles[nb_routes_possibles].y = 0;
        nb_routes_possibles++;
        //return set_vit(-1,0,v);
    }
    if (cross.right) {
        routes_possibles[nb_routes_possibles].x = 1;
        routes_possibles[nb_routes_possibles].y = 0;
        nb_routes_possibles++;
        //return set_vit(1,0,v);
    }


    route_choisie = (rand() % nb_routes_possibles);


    return set_vit(routes_possibles[route_choisie].x, routes_possibles[route_choisie].y, vit_chosen);


}

/**
 * Ecriture x et y dans le t_vit v
 * @param x
 * @param y
 * @param v
 * @return
 */
t_vit *set_vit(int8_t x, int8_t y, t_vit *v) {
    v->x = x;
    v->y = y;
    return v;
}
