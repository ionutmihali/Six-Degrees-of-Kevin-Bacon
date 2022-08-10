#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int visited[50] = {0};

typedef struct actor {
	char nume[30];
	int x;
	int y;
	int numar_filme;
	char *filme[50];
} actor;

typedef struct distributie {
	int **Matrix;
	int numNodes;
} distributie;

int numar_actori;
actor *read_actor(const char *f, actor *persoana)
{
	FILE *g = fopen(f, "r");
	char c[20];
	fgets(c, 20, g);
	numar_actori = atoi(c);
	char buffer[256];
	int indice = 0;
	int ok = 0;
	persoana = (actor *)malloc(sizeof(actor) * numar_actori);
	while (fgets(buffer, 256, g) && ok < numar_actori) {
		char *p = strtok(buffer, " (),\n\r");
		while (p != NULL) {
			strcpy(persoana[indice].nume, p);
			p = strtok(NULL, " (),\n\r");
			persoana[indice].x = atoi(p);
			p = strtok(NULL, " (),\n\r");
			persoana[indice].y = atoi(p);
			p = strtok(NULL, " (),\n\r");
			persoana[indice].numar_filme = atoi(p);
			p = strtok(NULL, " (),\n\r");
			for (int i = 0; i < persoana[indice].numar_filme; i++) {
				persoana[indice].filme[i] = (char *)malloc(sizeof(char) * strlen(p));
				strcpy(persoana[indice].filme[i], p);
				p = strtok(NULL, " (),\n\r");
			}
		}
		indice++;
		ok++;
	}

	/* for (int i = 0; i < indice; i++) {
		printf("%s %d %d %d ", persoana[i].nume, persoana[i].x, persoana[i].y, persoana[i].numar_filme);
		for (int j = 0; j < persoana[i].numar_filme; j++)
			printf("%s ", persoana[i].filme[j]);
		printf("\n");
	} */

	return persoana;
}

int comparare(actor persoana1, actor persoana2)
{
	for (int i = 0; i < persoana1.numar_filme; i++)
		for (int j = 0; j < persoana2.numar_filme; j++)
			if (strcmp(persoana1.filme[i], persoana2.filme[j]) == 0)
				return 1;
	return 0;
}

void completare_matrice(actor *persoana, distributie **nume)
{
	(*nume)->Matrix = (int **)malloc(sizeof(int *) * numar_actori);
	for (int i = 0; i < numar_actori; i++)
		(*nume)->Matrix[i] = (int *)malloc(sizeof(int) * numar_actori);

	for (int i = 0; i < numar_actori; i++)
		for (int j = 0; j < numar_actori; j++)
			(*nume)->Matrix[i][j] = 0;

	for (int i = 0; i < numar_actori; i++)
		for (int l = 0; l < numar_actori; l++) {
			if (i == l)
				continue;
			if (comparare(persoana[i], persoana[l]) == 1)
				(*nume)->Matrix[i][l] = 1;
		}

	/* for (int i = 0; i < numar_actori; i++) {
		for (int j = 0; j < numar_actori; j++)
			printf("%d ", nume->Matrix[i][j]);
		printf("\n");
	} */

	(*nume)->numNodes = numar_actori;
}

void listCast(char *nume, actor *persoana, int numar_persoane, FILE *f)
{
	for (int i = 0; i < numar_persoane; i++)
		for (int j = 0; j < persoana[i].numar_filme; j++)
			if (strcmp(nume, persoana[i].filme[j]) == 0)
				fprintf(f, "%s ", persoana[i].nume);
	fprintf(f, "\n");
}

typedef struct nodeDP {
	int node;
	int d;
	int parent;
	char *nume;
} nodeDP;

int dijsktra(distributie *graph, actor *persoana, char *nume)
{
	int source = 0, poz_actor = 0;
	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, "Kevin_Bacon") == 0) {
			source = i;
			break;
		}

	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume) == 0) {
			poz_actor = i;
			break;
		}

	nodeDP *NDP = (nodeDP *)malloc(sizeof(nodeDP) * graph->numNodes);
	if (NDP == NULL)
		return 0;
	for (int i = 0; i < graph->numNodes; i++) {
		NDP[i].node = i;
		NDP[i].d = 1000;
		NDP[i].parent = -1;
	}
	NDP[source].d = 0;
	int position = 0;
	int *visited = (int *)calloc(sizeof(int), graph->numNodes);
	int mindistance = 0, nextnode = 0, i = 0;

	for (i = 0; i < graph->numNodes; i++)
		visited[i] = 0;

	int **costMatrix = (int **)malloc(sizeof(int *) * numar_actori);
	for (int i = 0; i < numar_actori; i++)
		costMatrix[i] = (int *)malloc(sizeof(int) * numar_actori);

	for (int i = 0; i < numar_actori; i++)
		for (int j = 0; j < numar_actori; j++)
			if (graph->Matrix[i][j] == 1)
				costMatrix[i][j] = 1;
			else
				costMatrix[i][j] = 1000;

	while (position < graph->numNodes - 1) {
		mindistance = 1000;
		for (i = 0; i < graph->numNodes; i++)
			if (NDP[i].d < mindistance && !visited[i]) {
				mindistance = NDP[i].d;
				nextnode = i;
			}

		visited[nextnode] = 1;
		for (i = 0; i < graph->numNodes; i++)
			if (!visited[i])
				if (mindistance + costMatrix[nextnode][i] < NDP[i].d) {
					NDP[i].d = mindistance + costMatrix[nextnode][i];
					NDP[i].parent = nextnode;
				}
		position++;
	}

	return NDP[poz_actor].d;
}

void computeBaconNum(distributie *graph, char *nume_persoana, actor *persoana, FILE *g1)
{
	int valoare = dijsktra(graph, persoana, nume_persoana);
	fprintf(g1, "%d\n", valoare);
}

void listBaconNum(distributie *graph, char *nume, actor *persoana, FILE *f)
{
	int source = 0, poz_actor = 0;
	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, "Kevin_Bacon") == 0) {
			source = i;
			break;
		}

	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume) == 0) {
			poz_actor = i;
			break;
		}

	nodeDP *NDP = (nodeDP *)malloc(sizeof(nodeDP) * graph->numNodes);
	if (NDP == NULL)
		return;
	for (int i = 0; i < graph->numNodes; i++) {
		NDP[i].node = i;
		NDP[i].d = 1000;
		NDP[i].parent = -1;
		NDP[i].nume = (char *)malloc(strlen(persoana[i].nume));
		strcpy(NDP[i].nume, persoana[i].nume);
	}
	NDP[source].d = 0;
	int position = 0;
	int *visited = (int *)calloc(sizeof(int), graph->numNodes);
	int mindistance = 0, nextnode = 0, i = 0;

	for (i = 0; i < graph->numNodes; i++)
		visited[i] = 0;

	int **costMatrix = (int **)malloc(sizeof(int *) * numar_actori);
	for (int i = 0; i < numar_actori; i++)
		costMatrix[i] = (int *)malloc(sizeof(int) * numar_actori);

	for (int i = 0; i < numar_actori; i++)
		for (int j = 0; j < numar_actori; j++)
			if (graph->Matrix[i][j] == 1)
				costMatrix[i][j] = 1;
			else
				costMatrix[i][j] = 1000;

	while (position < graph->numNodes - 1) {
		mindistance = 1000;
		for (i = 0; i < graph->numNodes; i++)
			if (NDP[i].d < mindistance && !visited[i]) {
				mindistance = NDP[i].d;
				nextnode = i;
			}

		visited[nextnode] = 1;
		for (i = 0; i < graph->numNodes; i++)
			if (!visited[i])
				if (mindistance + costMatrix[nextnode][i] < NDP[i].d) {
					NDP[i].d = mindistance + costMatrix[nextnode][i];
					NDP[i].parent = nextnode;
				}
		position++;
	}

	int oprire = NDP[poz_actor].d;
	for (int i = 0; i < oprire; i++) {
		int pozitie = 0;
		fprintf(f, "%s was in [ ", NDP[poz_actor].nume);
		for (int j = 0; j < numar_actori; j++)
			if (j == NDP[poz_actor].parent)
				pozitie = j;

		int ok = 0;
		for (int l = 0; l < persoana[pozitie].numar_filme; l++)
			for (int m = 0; m < persoana[poz_actor].numar_filme; m++)
				if (strcmp(persoana[poz_actor].filme[m], persoana[pozitie].filme[l]) == 0) {
					if (ok == 0) {
						fprintf(f, "%s ", persoana[poz_actor].filme[m]);
						ok = 1;
					} else
						fprintf(f, ", %s ", persoana[poz_actor].filme[m]);
				}

		fprintf(f, "] with %s \n", NDP[pozitie].nume);

		poz_actor = pozitie;
	}
}

int distanta_euclidiana(actor persoana1, actor persoana2)
{
	int distanta = 0;
	distanta = (int)sqrt((persoana2.x - persoana1.x) * (persoana2.x - persoana1.x) + (persoana2.y - persoana1.y) * (persoana2.y - persoana1.y));
	return distanta;
}

void completare_matrice_drum(int **matrice_drum, actor *persoana)
{
	for (int i = 0; i < numar_actori; i++)
		for (int j = 0; j < numar_actori; j++)
			matrice_drum[i][j] = 0;

	for (int i = 0; i < numar_actori; i++)
		for (int j = 0; j < numar_actori; j++)
			if (comparare(persoana[i], persoana[j]) == 1)
				matrice_drum[i][j] = distanta_euclidiana(persoana[i], persoana[j]);
			else {
				matrice_drum[i][j] = 1000;
			}
	/* for (int i = 0; i < numar_actori; i++) {
		for (int j = 0; j < numar_actori; j++)
			printf("%d\t", matrice_drum[i][j]);
		printf("\n");
	} */
}

int dijsktra2(distributie *graph, actor *persoana, char *nume, char *nume2)
{
	int **matrice_drum = (int **)malloc(sizeof(int *) * numar_actori);
	for (int i = 0; i < numar_actori; i++)
		matrice_drum[i] = (int *)malloc(sizeof(int) * numar_actori);

	completare_matrice_drum(matrice_drum, persoana);

	int poz_actor1 = 0, poz_actor2 = 0;
	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume) == 0) {
			poz_actor1 = i;
			break;
		}

	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume2) == 0) {
			poz_actor2 = i;
			break;
		}

	nodeDP *NDP = (nodeDP *)malloc(sizeof(nodeDP) * graph->numNodes);
	if (NDP == NULL)
		return 0;
	for (int i = 0; i < graph->numNodes; i++) {
		NDP[i].node = i;
		NDP[i].d = 1000;
		NDP[i].parent = -1;
	}
	NDP[poz_actor1].d = 0;
	int position = 0;
	int *visited = (int *)calloc(sizeof(int), graph->numNodes);
	int mindistance = 0, nextnode = poz_actor1, i = 0;

	for (i = 0; i < graph->numNodes; i++)
		visited[i] = 0;

	visited[poz_actor1] = 1;
	while (position < graph->numNodes - 1) {
		mindistance = 1000;
		for (i = 0; i < graph->numNodes; i++)
			if (NDP[i].d < mindistance && visited[i] == 0) {
				mindistance = NDP[i].d;
				nextnode = i;
			}

		visited[nextnode] = 1;
		for (i = 0; i < graph->numNodes; i++)
			if (visited[i] == 0)
				if (NDP[nextnode].d + matrice_drum[nextnode][i] <= NDP[i].d) {
					NDP[i].d = NDP[nextnode].d + matrice_drum[nextnode][i];
					NDP[i].parent = nextnode;
				}
		position++;
	}

	/* for (int i = 0; i < numar_actori; i++)
		printf("%d \n", NDP[i].d); */

	return NDP[poz_actor2].d;
}

void computeDistBetweenActors(distributie *graph, char *nume_persoana, char *nume_persoana2, actor *persoana, FILE *f)
{
	int valoare = dijsktra2(graph, persoana, nume_persoana, nume_persoana2);
	fprintf(f, "%d\n", valoare);
}

void listDistBetweenActors(distributie *graph, actor *persoana, char *nume, char *nume2, FILE *f)
{
	int **matrice_drum = (int **)malloc(sizeof(int *) * numar_actori);
	for (int i = 0; i < numar_actori; i++)
		matrice_drum[i] = (int *)malloc(sizeof(int) * numar_actori);

	completare_matrice_drum(matrice_drum, persoana);

	int poz_actor1 = 0, poz_actor2 = 0;
	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume) == 0) {
			poz_actor1 = i;
			break;
		}

	for (int i = 0; i < numar_actori; i++)
		if (strcmp(persoana[i].nume, nume2) == 0) {
			poz_actor2 = i;
			break;
		}

	nodeDP *NDP = (nodeDP *)malloc(sizeof(nodeDP) * graph->numNodes);
	if (NDP == NULL)
		return;
	for (int i = 0; i < graph->numNodes; i++) {
		NDP[i].node = i;
		NDP[i].d = 1000;
		NDP[i].parent = -1;
	}
	NDP[poz_actor1].d = 0;
	int position = 0;
	int *visited = (int *)calloc(sizeof(int), graph->numNodes);
	int mindistance = 0, nextnode = poz_actor1, i = 0;

	for (i = 0; i < graph->numNodes; i++)
		visited[i] = 0;

	visited[poz_actor1] = 1;
	while (position < graph->numNodes - 1) {
		mindistance = 1000;
		for (i = 0; i < graph->numNodes; i++)
			if (NDP[i].d < mindistance && visited[i] == 0) {
				mindistance = NDP[i].d;
				nextnode = i;
			}

		visited[nextnode] = 1;
		for (i = 0; i < graph->numNodes; i++)
			if (visited[i] == 0)
				if (NDP[nextnode].d + matrice_drum[nextnode][i] <= NDP[i].d) {
					NDP[i].d = NDP[nextnode].d + matrice_drum[nextnode][i];
					NDP[i].parent = nextnode;
				}
		position++;
	}

	fprintf(f, "%s ", persoana[poz_actor1].nume);
	int *vector = (int *)malloc(sizeof(int) * numar_actori);
	int poz_copie = poz_actor2;
	int index = 0;
	while (NDP[poz_copie].parent != poz_actor1) {
		vector[index] = NDP[poz_copie].parent;
		poz_copie = NDP[poz_copie].parent;
		index++;
	}

	for (int i = index - 1; i >= 0; i--) {
		//printf("%d\n", vector[i]);
		fprintf(f, "%s ", persoana[vector[i]].nume);
	}

	fprintf(f, "%s \n", persoana[poz_actor2].nume);
}

typedef struct partialSolution {
	int *originalVector;
	int *permutation;
	int position;
	int step;
} partialSolution;

int canReject(partialSolution PS, actor *persoana)
{
	for (int i = 0; i < PS.step; i++) {
		if (PS.permutation[i] == PS.permutation[PS.step])
			return 1;
		if (strcmp(persoana[PS.permutation[0]].nume, "Kevin_Bacon") != 0)
			return 1;
	}

	for (int i = 0; i < PS.step - 1; i++) {
		if (comparare(persoana[PS.permutation[i]], persoana[PS.permutation[i + 1]]) == 0)
			return 1;
	}
	return 0;
}

int persoane[50][50], numar_persoane = 0, pozitie[50];

void initializare_matrice()
{
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
			persoane[i][j] = -5;

	for (int i = 0; i < 50; i++)
		pozitie[i] = 0;

	numar_persoane = 0;
}

int isSolution(partialSolution PS, actor *persoana)
{
	if (PS.step > 2 && PS.step <= numar_actori / 2) {
		for (int i = 0; i < numar_persoane; i++) {
			int x = 1;
			if (PS.step == pozitie[i])
				for (int j = 0; j < pozitie[i]; j++) {
					if (PS.permutation[j] != persoane[i][j])
						x = 0;
				}
			else
				x = 0;

			if (x == 1)
				return 0;
		}
		for (int i = 0; i < PS.step; i++) {
			persoane[numar_persoane][i] = PS.permutation[i];
		}
		pozitie[numar_persoane] = PS.step;
		numar_persoane++;
	}
	return 1;
}

partialSolution increaseStep(partialSolution PS)
{
	PS.step++;
	PS.position = 0;
	return PS;
}

partialSolution getNextChoiceAtStep(partialSolution PS)
{
	PS.permutation[PS.step] = PS.originalVector[PS.position];
	PS.position++;
	return PS;
}

int hasChoiceAtStep(partialSolution PS, actor *persoana)
{
	if (PS.step < numar_actori && PS.position < numar_actori)
		return 1;
	return 0;
}

void backtracking(partialSolution PS, actor *persoana)
{
	if (canReject(PS, persoana))
		return;
	isSolution(PS, persoana);
	PS = increaseStep(PS);

	while (hasChoiceAtStep(PS, persoana)) {
		PS = getNextChoiceAtStep(PS);
		backtracking(PS, persoana);
	}
}

void computeBaconCrewsNum(actor *persoana, FILE *f)
{
	initializare_matrice();
	partialSolution PS;
	PS.originalVector = (int *)malloc(sizeof(int) * numar_actori);
	PS.permutation = (int *)malloc(sizeof(int) * numar_actori);
	for (int i = 0; i < numar_actori; i++) {
		PS.originalVector[i] = i;
		PS.permutation[i] = -1;
	}
	PS.position = -1;
	PS.step = -1;
	backtracking(PS, persoana);
	fprintf(f, "%d\n", numar_persoane);
}

void listBaconCrewsNum(actor *persoana, FILE *f)
{
	initializare_matrice();
	partialSolution PS;
	PS.originalVector = (int *)malloc(sizeof(int) * numar_actori);
	PS.permutation = (int *)malloc(sizeof(int) * numar_actori);
	for (int i = 0; i < numar_actori; i++) {
		PS.originalVector[i] = i;
		PS.permutation[i] = -1;
	}
	PS.position = -1;
	PS.step = -1;
	backtracking(PS, persoana);

	for (int i = 0; i < numar_persoane; i++) {
		for (int j = 0; j < numar_persoane; j++)
			if (strcmp(persoana[persoane[i][j]].nume, "\0") != 0)
				fprintf(f, "%s ", persoana[persoane[i][j]].nume);
		fprintf(f, "\n");
	}
}

void read_function(char *f, actor *persoana, char *f1, distributie *graph)
{
	FILE *g = fopen(f, "r");
	FILE *g1 = fopen(f1, "w");
	int numar_persoane;
	char c[20];
	fgets(c, 20, g);
	numar_persoane = atoi(c);
	char buffer[256];
	int ok = 0;
	while (fgets(buffer, 256, g)) {
		if (ok >= numar_persoane + 1) {
			char *p = strtok(buffer, " \n\r(),");
			while (p != NULL) {
				char *nume_functie = (char *)malloc(sizeof(char) * (strlen(p) + 1));
				strcpy(nume_functie, p);
				//nume_functie = strtok(nume_functie, "!");

				if (strcmp(nume_functie, "listCast") == 0) {
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana, p);
					listCast(nume_persoana, persoana, numar_persoane, g1);
				} else if (strcmp(nume_functie, "listBaconNum") == 0) {
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana, p);
					listBaconNum(graph, nume_persoana, persoana, g1);
				} else if (strcmp(nume_functie, "listDistBetweenActors") == 0) {
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana, p);
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana2 = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana2, p);
					listDistBetweenActors(graph, persoana, nume_persoana, nume_persoana2, g1);
				} else if (strcmp(nume_functie, "listBaconCrewsNum") == 0)
					listBaconCrewsNum(persoana, g1);
				else if (strcmp(nume_functie, "computeBaconNum") == 0) {
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana, p);
					computeBaconNum(graph, nume_persoana, persoana, g1);
				} else if (strcmp(nume_functie, "computeDistBetweenActors") == 0) {
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana, p);
					p = strtok(NULL, " \n\r(),");
					char *nume_persoana2 = (char *)malloc(sizeof(char) * (strlen(p) + 1));
					strcpy(nume_persoana2, p);
					computeDistBetweenActors(graph, nume_persoana, nume_persoana2, persoana, g1);
				} else if (strcmp(nume_functie, "computeBaconCrewsNum") == 0)
					computeBaconCrewsNum(persoana, g1);

				p = strtok(NULL, " \n\r(),");
			}
		}
		ok++;
	}
}

int main(int argc, char *argv[])
{
	actor *persoana = NULL;
	distributie *persoane = (distributie *)malloc(sizeof(distributie));
	persoana = read_actor(argv[1], persoana);
	completare_matrice(persoana, &persoane);
	read_function(argv[1], persoana, argv[2], persoane);
	return 0;
}