#include <stdio.h>
#include <stdlib.h>



typedef struct edge {
    int v1, v2, cost;
} edge;

typedef struct set_values {
    int djs;
    int rank;
} set_values;



void make_set(set_values *setv, int x) {
    setv[x].djs = x;
    setv[x].rank = 0;
}

void lin(set_values *setv, int x, int y) {

    if(setv[x].rank > setv[y].rank) {
        setv[y].djs = x;
    } else {
        setv[x].djs = y;

        if(setv[x].rank == setv[y].rank) {
            setv[y].rank++;
        }
    }
}

int find_set(set_values *setv, int x) {
    if(setv[x].djs != x) {
        setv[x].djs = find_set(setv, setv[x].djs);
    }

    return setv[x].djs;
}


int cmpfunc (const void *a, const void *b) {
    edge *ia = ( edge *)a;
    edge *ib = ( edge *)b;
    return (int)(100.f * ia->cost - 100.f * ib->cost);

}


void union_set(set_values *setv, int i, int j) {
    lin(setv, find_set(setv, i), find_set(setv, j));
}

int main(int argc, char *argv[]) {
    int k;
    int v;
    int i;
    int total_cost = 0;
    int src, des, weight;
    int total_vertices, total_edges;

    // total number of vertices
    scanf("%d %d", &total_vertices, &total_edges);

    set_values *setv = malloc(total_vertices * sizeof(set_values));
    edge *edg = malloc(total_vertices * sizeof(edge));
    edge *final_result = malloc(total_vertices * sizeof(edge));

    //accept cost of edges
    for(k = 0; k < total_edges; k++) {
        scanf("%d %d %d", &src, &des, &weight);
        edg[k].v1 = src;
        edg[k].v2 = des;
        edg[k].cost = weight;
    }

    //Kruskal's algorithm
    for(v = 0; v < total_vertices; v++) {
        make_set(setv, v);
    }

    // qsort(pointer to first edge, total number of edges, size in bytes of each element,compare function)
    qsort(edg, total_edges, sizeof(edge), cmpfunc);

    for(i = 0, k = 0; i < total_edges; i++) {

        if(find_set(setv, edg[i].v1) != find_set(setv, edg[i].v2)) {
            final_result[k].v1 = edg[i].v1;
            final_result[k].v2 = edg[i].v2;
            final_result[k].cost = edg[i].cost;
            union_set(setv, edg[i].v1, edg[i].v2);
            k++;
        }
    }

    //display result
    for(i = 0; i < k; i++) {
        printf("%d %d %d\n", final_result[i].v1, final_result[i].v2, final_result[i].cost);
        total_cost = total_cost + final_result[i].cost;
    }

    printf("Total cost: %d\n", total_cost);
}
