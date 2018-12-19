#include "c_burglar.h"

void choose_next_direction(t_vit * v)
{
	//Directions envisageables :
	//Un chemin
	//Le demi-tours est interdit

	//Algorithme de choix de la prochaine direction
	//A : Le vect le plus grand fait partie des destinations envisageable
	//B : Le second vecteur plus petit (si != 0, en fait partie
	//C : Il reste une direction possible

	//fantom
	//TODO: Définir
	t_enemy e;
	enemy_ask(&e);

	//objectif
	//TODO: Définir
	t_pos obj;
	objectif_ask(&obj);


	//random à initialiser
	srand((unsigned) time(NULL));


	DEV("\n\nNous sommes sur une case %d\n", get_pos(e.pos.x, e.pos.y));

	//Cas de l'enemy sur l'objectif
	if(e.pos.x == obj.x && e.pos.y == obj.y) return;
	//Sortie de jeu
	if(get_pos(e.pos.x, e.pos.y) == E_WALL) return;

	//prendre en compte les directions possibles à la set_vit(0,1,v)e
	t_cross cross;
	get_crossroad(&e.pos, &cross);

	DEV("\n\nRoutes possibles :\nH:%d\nB:%d\nG:%d\nD:%d\n\n",cross.top,cross.bottom,cross.left,cross.right);

	//On supprime la possibilité du demi-tours
	if(e.vit.x != 0) (e.vit.x > 0) ? (cross.left = 0) : (cross.right = 0);
	else (e.vit.y > 0) ? (cross.top = 0) : (cross.bottom = 0);

	DEV("demi-tours supprimé\n");
	DEV("\n\nRoutes possibles :\nH:%d\nB:%d\nG:%d\nD:%d\n\n",cross.top,cross.bottom,cross.left,cross.right);


	//repère centré sur l'ennemi
	t_pos dir;
	dir.x = obj.x - e.pos.x;
	dir.y = obj.y - e.pos.y;



	//On calcule la direction dans laquelle se situe l'app
	int ratio = abs(dir.x) - abs(dir.y);

	
	//Cas de la diagonale parfaite
	if(ratio == 0)
	{
		DEV("\nRatio nul\n");
      	if(rand() % 2)	ratio += 1;
      	else ratio -= 1;
   	}

   	DEV("\nRepère centré :\ndir_x : %d\ndir_y : %d\n",dir.x,dir.y);

   	//PLAN A
   	DEV("\n\nPLAN A\n");
   	DEV("ratio = %d\n",ratio);

   	//l'app est sur l'horizontale
	if(ratio > 0)
	{
		if(dir.x < 0 && cross.left) return set_vit(-1,0,v);
		if(dir.x > 0 && cross.right) return set_vit(1,0,v);
	}
	//l'app est sur la verticale
	else
	{
		if(dir.y < 0 && cross.top) return set_vit(0,-1,v);
		if(dir.y > 0 && cross.bottom) return set_vit(0,1,v);
	}

	DEV("\n\nPLAN B\n");

	//PLAN B
	if(ratio < 0)
	{
		if(dir.x < 0 && cross.left) return set_vit(-1,0,v);
		if(dir.x > 0 && cross.right) return set_vit(1,0,v);
	}
	else
	{
		if(dir.y < 0 && cross.top) return set_vit(0,-1,v);
		if(dir.y > 0 && cross.bottom) return set_vit(0,1,v);
	}


	DEV("\n\nPLAN C\n");

	//PLAN C //Il reste potentiellement 2 routes possibles
	t_vit routes_possibles[2];
	int nb_routes_possibles = 0;
	int route_choisie;

	if(cross.top)
	{
		routes_possibles[nb_routes_possibles].x = 0;
		routes_possibles[nb_routes_possibles].y = -1;
		nb_routes_possibles++;
		//return set_vit(0,-1,v);
	}
	if(cross.bottom)
	{
		routes_possibles[nb_routes_possibles].x = 0;
		routes_possibles[nb_routes_possibles].y = 1;
		nb_routes_possibles++;
		//return set_vit(0,1,v);
	}
	if(cross.left)
	{
		routes_possibles[nb_routes_possibles].x = -1;
		routes_possibles[nb_routes_possibles].y = 0;
		nb_routes_possibles++;
		//return set_vit(-1,0,v);
	}
	if(cross.right)
	{
		routes_possibles[nb_routes_possibles].x = 1;
		routes_possibles[nb_routes_possibles].y = 0;
		nb_routes_possibles++;
		//return set_vit(1,0,v);
	}

		DEV("\nNombre de routes encore possibles : %d\n",nb_routes_possibles);


	route_choisie = (rand() % nb_routes_possibles);

			DEV("\nRoute choisie : %d\n",route_choisie);


	return set_vit(routes_possibles[route_choisie].x, routes_possibles[route_choisie].y, v);


}


void set_vit(int8_t x, int8_t y, t_vit * v)
{
	v->x = x;
	v->y = y;
}


void enemy_fixture(t_enemy * e)
{
	e->pos.x = 1;
	e->pos.y = 5;
	e->vit.x = 0;
	e->vit.y = 1;

	DEV("Enemy : %dx et %dy  | Vitesse : %dx et %dy\n",e->pos.x, e->pos.y, e->vit.x, e->vit.y);

}

void enemy_ask(t_enemy * e)
{
	DEV("Position Enemy\nx?");
	scanf("%hhu",&(e->pos.x));
	DEV("y?");
	scanf("%hhu",&(e->pos.y));
	DEV("\nVitesse\nx?");
	scanf("%hhu",&(e->vit.x));
	DEV("y?");
	scanf("%hhu",&(e->vit.y));

	DEV("\n\nEnemy : %dx et %dy  | Vitesse : %dx et %dy\n",e->pos.x, e->pos.y, e->vit.x, e->vit.y);

}

void objectif_fixture(t_pos * o)
{
	o->x = 20;
	o->y = 1;
	DEV("objectif %dx et %dy\n",o->x, o->y);
}


void objectif_ask(t_pos * o)
{
	DEV("\n\nObjectif\nx?");
	scanf("%hhu",&(o->x));
	DEV("y?");
	scanf("%hhu",&(o->y));

	DEV("\nobjectif %dx et %dy\n",o->x, o->y);
}


int main()
{

	t_vit v;

	choose_next_direction(&v);

	printf("X : %d\nY : %d\n", v.x,v.y);
}
