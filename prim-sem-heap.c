#include <stdio.h>
#include <stdlib.h>

typedef struct grafo grafo;
typedef struct adj_lista adj_lista;
typedef struct fila fila;

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

struct fila{
	adj_lista* cabeca;
};

//Passa a quantidade de vertices do grafo
grafo* criar_grafo(int tamanho);
//Adiciona uma aresta junto de seu custo
void adicionar_vertice(grafo* g, int vertice1, int vertice2, int custo);

//Origem é o vertice de 'partida'
adj_lista* adicionar_adj_lista(int vertice, int origem, int custo);

//Cira uma fila de prioridade vazia
fila* criar_fila();

//Enfileira uma aresta na fila de prioridade
void enqueue_priority(fila* fp, adj_lista* aresta_enfilerar);
//Checa se a fila está vazia, retorna 1 se está vazia e 0 se não está vazia
int fila_vazia(fila* fp);

//Desenfileira uma aresta
adj_lista* dequeue(fila* fp);
//Calcula a  Árvore de custo mínimo de um grafo
grafo* algoritmo_prim(grafo* g, int partida);
//Imprime na tela uim grafo
void print_g(grafo* g);
//Imprime na tela uma fila
void print_fila(fila* fp);

int main()
{
	//algum código aqui
	return 0;

}

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

fila* criar_fila()
{
	fila* novo = (fila*)malloc(sizeof(fila));
	novo->cabeca = NULL;
	return novo;
}

void enqueue_priority(fila* fp, adj_lista* aresta_enfilerar)
{
	adj_lista* aresta = adicionar_adj_lista(aresta_enfilerar->vertice, aresta_enfilerar->origem, aresta_enfilerar->custo);

	if(fila_vazia(fp))
	{
		fp->cabeca = aresta;
	}
	else
	{
		adj_lista* atual = fp->cabeca;
		adj_lista* anterior = NULL;
		while((atual!= NULL) && (atual->custo < aresta->custo))
		{
			anterior = atual;
			atual = atual->prox;
		}
		if(anterior == NULL)
		{
			aresta->prox = fp->cabeca;
			fp->cabeca = aresta;
		}
		else
		{
			aresta->prox = atual;
			anterior->prox = aresta;
		}
	}
}

int fila_vazia(fila* fp)
{
	return (fp->cabeca == NULL);
}

adj_lista* dequeue(fila* fp)
{
	if(!fila_vazia(fp))
	{
		adj_lista* alvo = fp->cabeca;

		fp->cabeca = fp->cabeca->prox;
		alvo->prox = NULL;
		return alvo;
	}
	else
	{
		printf("FILA UNDERFLOW\n");
		exit(0);
	}
}

grafo* algoritmo_prim(grafo* g, int partida)
{
	/*ACHO que não funciona para alguns grafos,  sei lá*/
	grafo* ACM = criar_grafo(g->tamanho);
	fila* fp;
	adj_lista* arestas;
	adj_lista* min_aresta;
	int adicionados = 1;//O primeiro vertice já está na árvore mínima

	fp = criar_fila();
	arestas = g->vertices[partida];//Pegue toda a franja da árvore

	while(adicionados < g->tamanho)//Enquanto ainda não estão todos os vertices na ACM
	{
		while(arestas != NULL)
		{
			enqueue_priority(fp, arestas);
			arestas = arestas->prox;
		}

		//print_fila(fp);
		min_aresta = dequeue(fp);

		while(ACM->vertices[min_aresta->vertice]!= NULL)//Se a aresta que liga ao vertice que se deseja adicionar já está na arvore
		{
			min_aresta = dequeue(fp);//Pegue a próxima aresta minima
		}

		adicionar_vertice(ACM, min_aresta->origem, min_aresta->vertice, min_aresta->custo);//Adicione a aresta minima na ACM

		printf("aresta adicionada: %d -- %d \n", min_aresta->origem, min_aresta->vertice);

		adicionados+=1;
		arestas = g->vertices[min_aresta->vertice];//Pegue todas as arestas do novo vertice da ACM
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

void print_fila(fila* fp)
{
	adj_lista* cabeca = fp->cabeca;
	printf("Estado da fila: ");
	while(cabeca!= NULL)
	{
		printf("%d -- %d | ", cabeca->origem, cabeca->vertice);
		cabeca = cabeca->prox;
	}
	printf("\n");
}