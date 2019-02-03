#include "main_enemy.h"

//Global


//PRS
extern int shm_overview_id, shm_lastkey_id, shm_map_id;


void *main_enemy(void *param) {

    t_enemy *mind = (t_enemy *) param;
    int id = mind->id;


    //SHM ET SEM
    //MAP
    t_map_unit *shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_t *sem_map = sem_open(SHM_SEM_MAP, O_RDWR);
    //OVERVIEW
    t_overview *shared_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_t *sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);


    //Traitement

    t_pos p_app, p_param, obj;


    //recuperation de mon objectif
    sem_wait(sem_overview);
    p_app.x = shared_overview->app.x;
    p_app.y = shared_overview->app.y;
    sem_post(sem_overview);

    //Traitement en fonction des subtilités de l'énemies
    //TODO
    obj = p_app;

    //Choix de la prochaine direction, inscription dans mind
    choose_next_direction(mind, &obj, shared_map, sem_map);
    //L'énemie se déplace (dans sa tête)
    mind->pos.x += mind->vit.x;
    mind->pos.y += mind->vit.y;
    //DEVDD("[IA M] L'énemie est maintenant en %d %d", mind->pos.x, mind->pos.y);


    //Détachement de la mémoire partagée
    shmdt(shared_map);
    shmdt(shared_overview);

    //Et des sémaphores
    sem_close(sem_map);
    sem_close(sem_overview);

    //Cloture du thread
    pthread_exit(EXIT_SUCCESS);

}


t_vit *choose_next_direction(t_enemy *e, t_pos *obj, t_map map, sem_t *sem_map) {
    //Directions envisageables :
    //Un chemin
    //Le demi-tours est interdit

    //Algorithme de choix de la prochaine direction
    //A : Le vect le plus grand fait partie des destinations envisageable
    //B : Le second vecteur plus petit (si != 0, en fait partie
    //C : Il reste une direction possible

    //Vitesse à retourner
    t_vit *vit_chosen = &(e->vit);


    //random à initialiser (une seule fois)
    srand((unsigned) time(NULL));

    //Utilisation de la map
    sem_wait(sem_map);
    //DEVD("[IA] Nous sommes sur une case %d", get_pos(e->pos.x, e->pos.y, map));

    //Cas de l'enemy sur l'objectif
    if (e->pos.x == obj->x && e->pos.y == obj->y) return -1; //TODO
    //Sortie de jeu
    if (get_pos(e->pos.x, e->pos.y, map) == E_WALL) return -1; //TODO ERREUR

    //prendre en compte les directions possibles à la set_vit(0,1,v)e
    t_cross cross;
    get_crossroad(&e->pos, &cross, map);

    sem_post(sem_map);

    //DEV("[IA] Routes possibles :");
    //DEVD("[IA] H:%d",cross.top);
    //DEVD("[IA] B:%d",cross.bottom);
    //DEVD("[IA] G:%d",cross.left);
    //DEVD("[IA] D:%d",cross.right);

    //On supprime la possibilité du demi-tours
    if (e->vit.x != 0) (e->vit.x > 0) ? (cross.left = 0) : (cross.right = 0);
    else (e->vit.y > 0) ? (cross.top = 0) : (cross.bottom = 0);

    //DEV("[IA] demi-tours supprimé");
    //DEV("[IA] Routes possibles :");
    //DEVD("[IA] H:%d",cross.top);
    //DEVD("[IA] B:%d",cross.bottom);
    //DEVD("[IA] G:%d",cross.left);
    //DEVD("[IA] D:%d",cross.right);


    //repère centré sur l'ennemi
    t_pos dir;
    dir.x = obj->x - e->pos.x;
    dir.y = obj->y - e->pos.y;



    //On calcule la direction dans laquelle se situe l'app
    int ratio = abs(dir.x) - abs(dir.y);


    //Cas de la diagonale parfaite
    if (ratio == 0) {
        //DEV("[IA] Ratio nul");
        if (rand() % 2) ratio += 1;
        else ratio -= 1;
    }

    //DEVDD("[IA] Repère centré :dir_x : %ddir_y : %d",dir.x,dir.y);

    //PLAN A
    //DEV("[IA] PLAN A");
    //DEVD("[IA] ratio = %d",ratio);

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

    //DEV("[IA] PLAN B");

    //PLAN B
    if (ratio < 0) {
        if (dir.x < 0 && cross.left) return set_vit(-1, 0, vit_chosen);
        if (dir.x > 0 && cross.right) return set_vit(1, 0, vit_chosen);
    } else {
        if (dir.y < 0 && cross.top) return set_vit(0, -1, vit_chosen);
        if (dir.y > 0 && cross.bottom) return set_vit(0, 1, vit_chosen);
    }


    //DEV("[IA] PLAN C");

    //PLAN C //Il reste potentiellement 2 routes possibles
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

    //DEVD("[IA] Nombre de routes encore possibles : %d",nb_routes_possibles);


    route_choisie = (rand() % nb_routes_possibles);

    //DEVD("[IA] Route choisie : %d",route_choisie);


    return set_vit(routes_possibles[route_choisie].x, routes_possibles[route_choisie].y, vit_chosen);


}


t_vit *set_vit(int8_t x, int8_t y, t_vit *v) {
    v->x = x;
    v->y = y;

    //DEVDD("[IA] RETURN! La vitesse choisie est x:%d, y:%d", v->x, v->y);

    return v;
}
