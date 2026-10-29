/*Enache Miruna - 311 CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod
{
	int frecv;
	char *nume;
	struct nod *st, *dr, *parinte;
} nod;

typedef int (*CompF)(nod *, nod *);

typedef struct heap
{
	int memorie, nrElem;
	nod **v;
	CompF comp;
} heap;

int comparare_nod(nod *a, nod *b)
{
	if (a->frecv != b->frecv)
		return a->frecv < b->frecv;
	// comparam satelitii dupa frecventa

	if (strcmp(a->nume, b->nume) < 0)
		return 1;
	else
		return 0;
	// comparam satelitii dupa nume daca frecventele sunt egale
}

heap *initializare_heap(int memorie, CompF comp)
{
	heap *h = calloc(1, sizeof(heap));
	// alocam memorie pentru heap
	h->memorie = memorie;
	h->comp = comp;
	// setam campurile memorie si comp cu valorile date
	h->v = calloc(memorie, sizeof(nod *));
	h->nrElem = 0;
	// alocam memorie pentru vectorul de noduri
	return h;
}

void insereaza_element(heap *h, nod *elem)
{
	if (h->nrElem >= h->memorie)
	{
		h->v = realloc(h->v, (h->nrElem + h->memorie) * sizeof(nod *));
		h->memorie = h->nrElem + h->memorie;
		// realoca memorie in heap daca este necesar
	}
	h->v[h->nrElem] = elem;
	h->nrElem++;
	// adauga elementul in heap
	int i, min_poz;
	i = h->nrElem - 1;
	min_poz = i;
	while (i > 0 && h->comp(h->v[min_poz], h->v[(i - 1) / 2]))
	{
		nod *aux = h->v[(i - 1) / 2];
		h->v[(i - 1) / 2] = h->v[min_poz];
		h->v[min_poz] = aux;
		min_poz = (i - 1) / 2;
		i = (i - 1) / 2;
	}
	// muta elementul inserat pe pozitia lui corecta in heap
}

nod *extract_heap(heap *h)
{
	if (h->nrElem == 0)
		return NULL;
	nod *elem = h->v[0];
	// salveaza radacina
	h->v[0] = h->v[h->nrElem - 1];
	// pune ultimul element pe pozitia radacinii
	h->nrElem--;

	int i, min_poz;
	i = 0;
	while (1)
	{
		min_poz = i;
		if (i * 2 + 1 < h->nrElem && h->comp(h->v[i * 2 + 1], h->v[min_poz]))
			min_poz = i * 2 + 1;
		if (i * 2 + 2 < h->nrElem && h->comp(h->v[i * 2 + 2], h->v[min_poz]))
			min_poz = i * 2 + 2;
		// comparam elementul mutat cu cei 2 fii ai sai
		// daca unul are prioritate mai mare se interschimba cele 2 elemente

		if (min_poz == i)
			break;
		// elementrul este pe pozitia corecta din heap

		nod *aux = h->v[i];
		h->v[i] = h->v[min_poz];
		h->v[min_poz] = aux;
		i = min_poz;
	}
	// mutam elementul pus in locul radacinii pe pozitia corecta in heap
	return elem;
}

nod *construieste_arbore(heap *h)
{
	while (h->nrElem > 1)
	{
		nod *st = extract_heap(h);
		nod *dr = extract_heap(h);
		// extrage primele 2 noduri din heap
		if (st->frecv == dr->frecv && strcmp(st->nume, dr->nume) > 0)
		{
			nod *aux = st;
			st = dr;
			dr = aux;
		}
		// le ordoneaza dupa nume daca au frecvente egale

		char *nume_parinte = calloc(strlen(st->nume) + strlen(dr->nume) + 1, sizeof(char));
		strcpy(nume_parinte, st->nume);
		strcat(nume_parinte, dr->nume);
		nod *parinte = calloc(1, sizeof(nod));
		parinte->frecv = st->frecv + dr->frecv;
		parinte->nume = nume_parinte;
		parinte->st = st;
		parinte->dr = dr;
		// creeaza parintele comun al celor 2 noduri
		st->parinte = parinte;
		dr->parinte = parinte;
		// seteaza nodul drept parinte pentru cele 2 noduri
		insereaza_element(h, parinte);
		// se insereaza parintele nou creat in heap
	}
	nod *rad = extract_heap(h);
	rad->parinte = NULL;
	// radacina arborelui ramane ultimul element din heap
	// parintele radacinii se seteaza pe NULL
	return rad;
}

int adancime(nod *n)
{
	int d = 0;
	while (n != NULL)
	{
		n = n->parinte;
		d++;
	}
	return d;
	//returneaza nivelul la care se afla nodul in arbore
}

void afisare_arbore(nod *radacina, FILE *g)
{
	if (radacina == NULL)
		return;

	nod **sateliti = calloc(1000, sizeof(nod *));
	int *adancimi = calloc(1000, sizeof(int));
	int nr_elem = 0, i = 0;
	sateliti[nr_elem] = radacina;
	//adauga radacina in vectorul de sateliti
	adancimi[nr_elem] = adancime(radacina);
	//adauga adancimea radacinii in vectorul de adancimi
	nr_elem++;
	int ad_max = adancimi[0];
	//ad_max reprezinta adancimea maxima din arbore
	while (i < nr_elem)
	{
		nod *curent = sateliti[i];
		//verifica daca exista nod in stanga
		if (curent->st != NULL)
		{
			sateliti[nr_elem] = curent->st;
			adancimi[nr_elem] = adancime(curent->st);
			//adauga fiul stang si adancimea sa in cei 2 vectori
			if (adancimi[nr_elem] > ad_max)
				ad_max = adancimi[nr_elem];
			//modifica adancimea maxima
			nr_elem++;
		}
		if (curent->dr != NULL)
		{
			sateliti[nr_elem] = curent->dr;
			adancimi[nr_elem] = adancime(curent->dr);
			//adauga fiul drept si adancimea sa in cei 2 vectori
			if (adancimi[nr_elem] > ad_max)
				ad_max = adancimi[nr_elem];
			//modifica adancimea maxima
			nr_elem++;
		}
		i++;
	}

	for (int nivel = 1; nivel <= ad_max; nivel++)
	{
		//se afiseaza nodurile pe nivele
		for (int j = 0; j < nr_elem; j++)
		{
			//se parcurge vectorul de sateliti si se veifica daca
			//adancima sa e egala cu nivelul pe care il afisam in prezent
			if (adancimi[j] == nivel)
				fprintf(g, "%d-%s ", sateliti[j]->frecv, sateliti[j]->nume);
		}
		fprintf(g, "\n");
	}

	free(adancimi);
	free(sateliti);
}

void decodificare(nod *radacina, char *cod, FILE *g)
{
	int n = 0, i;
	nod *q = radacina;
	while (n < strlen(cod))
	//parcurgem dupa cod
	{
		if (cod[n] == '0')
		{
			//daca cod e 0 mergem in stanga
			if (q->st != NULL)
				q = q->st;
			else
			{
				//daca nu mai putem sa mergem in stanga
				//afisam nodul curent
				//pornim iar cu parcurgerea din radacina
				fprintf(g, "%s ", q->nume);
				n--;
				q = radacina;
			}
		}
		else
		{
			//daca cod e 1 mergem in dreapta
			if (q->dr != NULL)
				q = q->dr;
			else
			{
				//daca nu mai putem sa mergem in stanga
				//afisam nodul curent
				//pornim iar cu parcurgerea din radacina
				fprintf(g, "%s ", q->nume);
				n--;
				q = radacina;
			}
		}
		n++;
	}
	if (q != radacina && q->st == NULL && q->dr == NULL)
		fprintf(g, "%s ", q->nume);
	//se verifica daca s-a terminat codul si nu s-a afisat ultimul satelit
}

nod *gaseste_satelit(nod *radacina, char *cautat)
{
	//parcurge arborele si intoarce adresa nodului cu numele dat
	if (radacina == NULL)
		return NULL;

	if (strcmp(radacina->nume, cautat) == 0)
		return radacina;
	nod *gasit = gaseste_satelit(radacina->st, cautat);
	if (gasit != NULL)
		return gasit;
	return gaseste_satelit(radacina->dr, cautat);
}

char *strrev(char *s)
{
	//inverseaza un sir de caractere
	char *a = calloc(strlen(s) + 1, sizeof(char));
	int i;
	for (i = 0; s[i]; i++)
		a[strlen(s) - 1 - i] = s[i];
	a[strlen(s)] = '\0';
	strcpy(s, a);
	free(a);
	return s;
}

char *codificare(nod *radacina, char *nume)
{
	nod *gasit = gaseste_satelit(radacina, nume);
	//gasim nodul cu numele dat in arbore
	char *cod = calloc(1001, sizeof(char));
	cod[0] = '\0';
	while (gasit != NULL && gasit->parinte != NULL)
	{
		//facem o parcurgere inversa
		nod *q = gasit->parinte;
		//ne ducem in parintele nodului curent
		if (q->st == gasit)
			strcat(cod, "0");
		else
			strcat(cod, "1");
		//verificam daca nodul curent e fiu stang sau fiu drept
		gasit = gasit->parinte;
	}
	strrev(cod);
	//se inverseaza codul
	return cod;
}

nod *parinte_comun(nod *satelit1, nod *satelit2)
{
	int ad_1, ad_2;
	ad_1 = adancime(satelit1);
	ad_2 = adancime(satelit2);
	//se calculeaza adancimea celor 2 noduri
	while (ad_2 > ad_1)
	{
		satelit2 = satelit2->parinte;
		ad_2--;
	}
	while (ad_1 > ad_2)
	{
		satelit1 = satelit1->parinte;
		ad_1--;
	}
	//se aduc cele 2 noduri "la acelasi nivel"
	while (satelit1 != satelit2)
	{
		satelit1 = satelit1->parinte;
		satelit2 = satelit2->parinte;
	}
	//se continua urcarea in arbore pana se ajunge la parintele comun
	return satelit1;
}

void free_arbore(nod *radacina)
{
	//elibereaza memoria pentru arbore
	if (radacina == NULL)
		return;
	free_arbore(radacina->st);
	free_arbore(radacina->dr);
	free(radacina->nume);
	free(radacina);
}

int main(int argc, char *argv[])
{
	char *fisier_intrare = argv[2];
	char *fisier_iesire = argv[3];
	FILE *f = fopen(fisier_intrare, "r");
	FILE *g = fopen(fisier_iesire, "w");
	int n, i;
	fscanf(f, "%d", &n);
	heap *h = initializare_heap(n, comparare_nod);
	//se initializeaza heapul

	for (i = 0; i < n; i++)
	{
		int frecv;
		char nume[20];
		fscanf(f, "%d %s", &frecv, nume);
		nod *satelit = calloc(1, sizeof(nod));
		satelit->frecv = frecv;
		satelit->nume = calloc(strlen(nume) + 1, sizeof(char));
		strcpy(satelit->nume, nume);
		satelit->st = NULL;
		satelit->dr = NULL;
		insereaza_element(h, satelit);
		//se citesc satelitii si se adauga in heap
	}

	nod *radacina = construieste_arbore(h);
	//se transforma heapul in arbore
	if (strcmp(argv[1], "-c1") == 0)
	{
		afisare_arbore(radacina, g);
		//pentru cerinta 1 se afiseaza arborele
	}
	else if (strcmp(argv[1], "-c2") == 0)
	{
		fscanf(f, "%d", &n);
		//citim nr de codificari
		char *cod = calloc(1001, sizeof(char));
		for (i = 0; i < n; i++)
		{
			fscanf(f, "%s", cod);
			decodificare(radacina, cod, g);
			//se parcurge arborele dupa cod
			fprintf(g, "\n");
		}
		free(cod);
	}
	else if (strcmp(argv[1], "-c3") == 0)
	{
		fscanf(f, "%d", &n);
		//citim nr de siruri de codificat
		char *nume = calloc(1001, sizeof(char));
		for (i = 0; i < n; i++)
		{
			fscanf(f, "%s", nume);
			char *cod = codificare(radacina, nume);
			fprintf(g, "%s", cod);
			//se calculeaza codul
			free(cod);
		}
		free(nume);
	}
	else if (strcmp(argv[1], "-c4") == 0)
	{
		fscanf(f, "%d", &n);
		//citim nr de siruri
		char *nume = calloc(1001, sizeof(char));
		fscanf(f, "%s", nume);
		nod *satelit1 = gaseste_satelit(radacina, nume);
		for (i = 0; i < n; i++)
		{
			fscanf(f, "%s", nume);
			nod *satelit2 = gaseste_satelit(radacina, nume);
			satelit1 = parinte_comun(satelit1, satelit2);
		}
		fprintf(g, "%s ", satelit1->nume);
		//se calculeaza satelitul parinte
		free(nume);
	}

	//se elibereaza memoria
	free_arbore(radacina);
	free(h->v);
	free(h);
	fclose(f);
	fclose(g);
	return 0;
}