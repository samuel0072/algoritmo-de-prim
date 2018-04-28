#include <stdio.h>
#include <stdlib.h>


typedef struct grafo grafo;
typedef struct adj_lista adj_lista;
typedef struct heap heap;

struct adj_lista{
	int custo;
	int vertice;
	int origem;
	adj_lista* prox;
};

struct grafo{
	adj_lista** vertices;
	int tamanho;
	int custo;
};

struct heap{
	int tam_max;
	int tam_atual;
	adj_lista** arestas;
};
/*FUNCOES PARA O GRAFO*/

//Passa a quantidade de vertices do grafo
grafo* criar_grafo(int tamanho);

//Adiciona uma aresta junto de seu custo
void adicionar_vertice(grafo* g, int vertice1, int vertice2, int custo);

//Imprime na tela um grafo
void print_g(grafo* g);

//Origem é o vertice de 'partida'
adj_lista* adicionar_adj_lista(int vertice, int origem, int custo);

//g é o grafo a ser feita a MST, partida é o vertice inicial, m é o tamanho da heap, retorna a MST de G
grafo* algoritmo_prim(grafo* g, int partida, int m);

/*FUNCOES PARA A HEAP MÍNIMA*/

heap* criar_heap(int tamanho);

void enqueue(heap* hp, adj_lista* aresta);

adj_lista* dequeue(heap* hp);

int indice_pai(int i);

int indice_esquerda(int i);

int indice_direita(int i);

void min_heapify(heap* hp ,int i);

int heap_vazia(heap* hp);

void print_heap(heap* hp);

int main()
{
	//algum código aqui
	return 0;
}
/*funcoes pra heap*/
heap* criar_heap(int tam_max)
{
	heap* nova = (heap*)malloc(sizeof(heap));
	int i;

	nova->tam_max = tam_max;
	nova->tam_atual = 0;
	nova->arestas = (adj_lista**)malloc(tam_max*sizeof(adj_lista*));

	for(i = 0;i < tam_max; ++i)
	{
		nova->arestas[i] = NULL;
	}
	return nova;
}
void enqueue(heap* hp, adj_lista* aresta)
{
	if(hp->tam_atual >= hp->tam_max)
	{
		printf("heap overflow!\n");
	}
	else
	{
		adj_lista* aresta_adc = adicionar_adj_lista(aresta->vertice, aresta->origem, aresta->custo);
		hp->arestas[hp->tam_atual] = aresta_adc;
		int indice = hp->tam_atual;
		int indicePai = indice_pai(hp->tam_atual);//Atribui ao indice e depois incrementa o tamanho atual da hp
		hp->tam_atual++;
		while((indicePai >= 0) && (hp->arestas[indice]->custo < hp->arestas[indicePai]->custo))
		{
			adj_lista* aux = hp->arestas[indice];
			hp->arestas[indice] = hp->arestas[indicePai];
			hp->arestas[indicePai] = aux;
			indice = indicePai;
			indicePai = indice_pai(indice);
		}
	}
}
adj_lista* dequeue(heap* hp)
{
	if(hp->tam_atual == 0)
	{
		printf("heap underrflow!");
		exit(0);
	}
	else
	{
		adj_lista* dequeued = hp->arestas[0];
		hp->arestas[0] = hp->arestas[--hp->tam_atual];
		min_heapify(hp, 0);
		//printf("dequeued = %d, hp[0] = %d\n", dequeued->custo, hp->arestas[0]->custo);
		return dequeued;
	}
}
int indice_pai(int i)
{
	return i/2;
}
int indice_esquerda(int i)
{
	return i*2;
}
int indice_direita(int i)
{
	return (2*i)+1;
}
void min_heapify(heap* hp ,int i)
{
	int menor;
	int i_esquerda = indice_esquerda(i);
	int i_direita = indice_direita(i);

	if((i_esquerda <= (hp->tam_atual-1)) && (hp->arestas[i]->custo > hp->arestas[i_esquerda]->custo))
	{
		menor = i_esquerda;
	}
	else
	{
		menor = i;
	}

	if((i_direita <= (hp->tam_atual-1)) && (hp->arestas[menor]->custo > hp->arestas[i_direita]->custo))
	{
		menor = i_direita;
	}

	if(hp->arestas[i]->custo != hp->arestas[menor]->custo)
	{
		adj_lista* aux = hp->arestas[i];
		hp->arestas[i] = hp->arestas[menor];
		hp->arestas[menor] = aux;
		min_heapify(hp, menor);
	}
}

int heap_vazia(heap* hp)
{
	return (hp->tam_atual == 0);
}
void print_heap(heap* hp)
{
	int i;
	printf("Heap:\n");
	for(i = 0; i < hp->tam_atual; i++)
	{
		printf("%d || ", hp->arestas[i]->custo);
	}
	printf("\n");
}
/*funcoes para grafo*/

grafo* criar_grafo(int tamanho)
{
	grafo* novo = (grafo*)malloc(sizeof(grafo));
	int i;

	novo->vertices = (adj_lista**)malloc(tamanho* sizeof(adj_lista*));
	novo->tamanho = tamanho;
	novo->custo = 0;

	for(i = 0; i < tamanho; ++i)
	{
		novo->vertices[i] = NULL;
	}
	return novo;
}

void adicionar_vertice(grafo* g, int vertice1, int vertice2, int custo)
{
	adj_lista* temp = adicionar_adj_lista(vertice2, vertice1, custo);

	temp->prox = g->vertices[vertice1];
	g->vertices[vertice1] = temp;

	temp = adicionar_adj_lista(vertice1, vertice2, custo);

	temp->prox = g->vertices[vertice2];
	g->vertices[vertice2] = temp;

	g->custo += custo;
}

adj_lista* adicionar_adj_lista(int vertice, int origem, int custo)
{
	adj_lista* novo = (adj_lista*)malloc(sizeof(adj_lista));
	novo->vertice = vertice;
	novo->custo = custo;
	novo->origem = origem;
	novo->prox = NULL;
	return novo;
}

grafo* algoritmo_prim(grafo* g, int partida, int m)
{
	grafo* ACM = criar_grafo(g->tamanho);
	heap* hp;
	adj_lista* arestas;
	adj_lista* min_aresta;
	int i;

	arestas = g->vertices[partida];
	hp = criar_heap(m+1);
	/*Adiciona as arestas do vertice de partida na heap*/
	while(arestas != NULL)
	{
		enqueue(hp, arestas);
		arestas = arestas->prox;
	}

	while(!heap_vazia(hp))
	{
		min_aresta = dequeue(hp);
		/*Se o vertice a ser adicionado ainda não está na árvore, evita formar ciclos*/
		if(ACM->vertices[min_aresta->vertice] == NULL)
		{
			adicionar_vertice(ACM, min_aresta->origem, min_aresta->vertice, min_aresta->custo);
			arestas = g->vertices[min_aresta->vertice];
			while(arestas != NULL)//Adiciona as arestas deste novo vertice
			{
				if(ACM->vertices[arestas->vertice] == NULL)//Não adiciona arestas que possam causar ciclos
					enqueue(hp, arestas);
				arestas = arestas->prox;
			}
		}

	}
	return ACM;
}

void print_g(grafo* g)
{
	int i;
	adj_lista* temp;

	for(i = 0; i < g->tamanho; i++)
	{
		temp = g->vertices[i];
		printf("vertice: %d |--> ", i);
		while(temp!= NULL)
		{
			printf("vertice : %d custo:%d\\ ", temp->vertice, temp->custo);
			temp= temp->prox;
		}
		printf("\n");
	}
}
