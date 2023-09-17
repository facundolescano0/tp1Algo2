#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define MAX_LONG 20
#define MAX_ATAQUES 3
#define MAX_LINEA 50

struct pokemon {
	char nombre[MAX_LONG];
	enum TIPO tipo;
	struct ataque ataques[MAX_ATAQUES];
	int cant_ataques;
};

struct info_pokemon {
	pokemon_t *pokemon;
	int cant_poke;
};

enum TIPO definir_tipo(char type)
{
	if (type == 'N')
		return NORMAL;
	else if (type == 'F')
		return FUEGO;
	else if (type == 'P')
		return PLANTA;
	else if (type == 'E')
		return ELECTRICO;
	else if (type == 'R')
		return ROCA;
	else if (type == 'A')
		return AGUA;
	else
		return -1;
}

informacion_pokemon_t *crear_info()
{
	return (informacion_pokemon_t *)calloc(1,
					       sizeof(informacion_pokemon_t));
}

pokemon_t *llenar_pokemon(const char *string, pokemon_t *pokemon)
{
	if (!string || !pokemon)
		return NULL;
	char type;
	int cant_leidos = sscanf(string, "%[^;];%c", pokemon->nombre, &type);
	if (cant_leidos != 2) {
		return NULL;
	}
	pokemon->tipo = definir_tipo(type);
	return pokemon;
}

struct ataque *cargar_ataque(const char *string, pokemon_t *poke)
{
	if (!string || !poke)
		return NULL;
	struct ataque *atq = &(poke->ataques[poke->cant_ataques]);
	char type;
	int cant_leidos = sscanf(string, "%[^;];%c;%u", atq->nombre, &type,
				 &(atq->poder));
	if (cant_leidos != 3) {
		return NULL;
	}

	atq->tipo = definir_tipo(type);
	if (!atq) {
		free(poke);
		return NULL;
	}

	return atq;
}

pokemon_t *agrandar_poke(informacion_pokemon_t *info, pokemon_t *poke_aux)
{
	if (!info)
		return NULL;
	unsigned long int pokes;
	pokes = (unsigned long int)(info->cant_poke + 1);
	pokemon_t *pokemon =
		(pokemon_t *)realloc(info->pokemon, sizeof(pokemon_t) * pokes);
	if (!pokemon) {
		free(info->pokemon);
		return NULL;
	}
	info->pokemon = pokemon;
	info->pokemon[info->cant_poke] = *poke_aux;

	return info->pokemon;
}

informacion_pokemon_t *subir_pokemon(informacion_pokemon_t *info,
				     pokemon_t *poke_aux)
{
	if (!poke_aux || !info)
		return NULL;
	pokemon_t *poke = agrandar_poke(info, poke_aux);
	if (!poke)
		return info;
	info->pokemon[info->cant_poke] = *poke_aux;
	return info;
}

pokemon_t *inicializar_poke()
{
	return (pokemon_t *)calloc(1, sizeof(pokemon_t));
}
struct ataque *inicializar_at()
{
	return calloc(1, sizeof(struct ataque));
}

void pasamanos(pokemon_t *poke_valido, pokemon_t *poke_aux)
{
	poke_valido = poke_aux;
	poke_aux = inicializar_poke(poke_aux);
}

informacion_pokemon_t *ordenar_pokemones(informacion_pokemon_t *info)
{
	for (int j = 0; j < info->cant_poke - 1; j++) {
		for (int i = 0; i < info->cant_poke - 1 - j; i++) {
			if (strcmp(info->pokemon[i].nombre,
				   info->pokemon[i + 1].nombre) > 0) {
				pokemon_t *auxiliar = &(info->pokemon[i + 1]);
				info->pokemon[i + 1] = info->pokemon[i];
				info->pokemon[i] = *auxiliar;
			}
		}
	}
	return info;
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	FILE *archivo = fopen(path, "r");
	if (!archivo)
		return NULL;

	char linea[MAX_LINEA];
	int contador_lineas = 0;
	informacion_pokemon_t *info = crear_info();
	pokemon_t *poke_aux =
		inicializar_poke(); //->poke_aux apunta a tal direc

	while (fgets(linea, sizeof(linea), archivo) != NULL) {
		if (linea == NULL) {
			return info;
		}
		if (contador_lineas % 4 == 0) {
			poke_aux = llenar_pokemon(linea, poke_aux);
			if (!poke_aux) {
				free(poke_aux);
				return info;
			}
		} else {
			struct ataque *atq = cargar_ataque(linea, poke_aux);
			if (!atq)
				return info;

			if (poke_aux->cant_ataques == 3) {
				subir_pokemon(info, poke_aux);
				poke_aux = inicializar_poke(poke_aux);
				info->cant_poke++;
			}
		}
		contador_lineas++;

		info = ordenar_pokemones(info);
	}
	fclose(archivo);

	if (!info) {
		free(info);
		return NULL;
	}
	return info;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (!ip || !nombre)
		return NULL;

	for (int i = 0; i < ip->cant_poke; i++) {
		if (strcmp(ip->pokemon[i].nombre, nombre) == 0)
			return &(ip->pokemon[i]);
	}
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (!ip)
		return 0;

	return ip->cant_poke;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (!pokemon)
		return NULL;
	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (!pokemon)
		return NORMAL;
	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (!pokemon || !nombre)
		return NULL;
	for (int i = 0; i < pokemon->cant_ataques; i++) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0)
			return &(pokemon->ataques[i]);
	}
	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (!ip || !aux)
		return 0;

	for (int i = 0; i < ip->cant_poke; i++) {
		(*f)(&(ip->pokemon[i]), aux);
	}

	return ip->cant_poke;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (!pokemon || !aux)
		return 0;

	for (int i = 0; i < pokemon->cant_ataques; i++)
		(*f)(&(pokemon->ataques[i]), aux);
	return pokemon->cant_ataques;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
}
