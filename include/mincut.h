#pragma once

#define WIN32_LEAN_AND_MEAN // Escludere gli elementi usati raramente dalle intestazioni di Windows

/**
 *   Author Andrea De Lorenzi
 *   for the course of Operation Research 2, AY 2022/23
 *
 *   Source code extracted from the 03.12.19 release of the Concorde TSP solver
 *   developed by William Cook and available for academic research use at
 *   math.uwaterloo.ca/tsp/concorde/index.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CC_MINCUT_BIGDOUBLE (1e30)
#define CC_MINCUT_ONE_EPSILON (0.000001)
#define SRK_ZERO_EPSILON (1e-10)
#define CONNECT_ZERO_EPSILON (1e-10)
#define CC_INFINITY (1 << 30)
#define PRF_EPSILON 0.000000001
#define GOING_IN 0
#define GOING_OUT 1
#define GLOBAL_RELABEL_FREQUENCY 1.0

#define UNDIRECTED_GRAPH

#define HIGHEST_LABEL_PRF
#undef QUEUE_PRF

#undef PR_USE3 /* PR_USE3 and PR_USE4 cannot be defined in current code */
#undef PR_USE4

#define USE_GAP

#ifdef HIGHEST_LABEL_PRF
#define ADD_TO_ACTIVE(n)                          \
    {                                             \
        if (!(n)->active)                         \
        {                                         \
            (n)->highnext = high[(n)->flowlabel]; \
            high[(n)->flowlabel] = (n);           \
            if (G->highest < (n)->flowlabel)      \
                G->highest = (n)->flowlabel;      \
            (n)->active = 1;                      \
        }                                         \
    }
#endif

#ifdef UNDIRECTED_GRAPH
#define RELABEL_BODY(n)                               \
    for (rele = (n)->out; rele; rele = rele->outnext) \
    {                                                 \
        if (rele->cap - rele->flow > PRF_EPSILON &&   \
            (relt = rele->ends[1]->flowlabel) < relm) \
            relm = relt;                              \
    }                                                 \
    for (rele = (n)->in; rele; rele = rele->innext)   \
    {                                                 \
        if (rele->cap + rele->flow > PRF_EPSILON &&   \
            (relt = rele->ends[0]->flowlabel) < relm) \
            relm = relt;                              \
    }                                                 \
    (n)->flowlabel = ++relm;
#else
#define RELABEL_BODY(n)                               \
    for (rele = (n)->out; rele; rele = rele->outnext) \
    {                                                 \
        if (rele->cap - rele->flow > PRF_EPSILON &&   \
            (relt = rele->ends[1]->flowlabel) < relm) \
            relm = relt;                              \
    }                                                 \
    for (rele = (n)->in; rele; rele = rele->innext)   \
    {                                                 \
        if (rele->flow > PRF_EPSILON &&               \
            (relt = rele->ends[0]->flowlabel) < relm) \
            relm = relt;                              \
    }                                                 \
    (n)->flowlabel = ++relm;
#endif

#ifdef USE_GAP
#define RELABEL(n)                                      \
    {                                                   \
        int relm = CC_INFINITY;                            \
        edge *rele;                                     \
        int relt, relold = (n)->flowlabel;              \
                                                        \
        RELABEL_BODY(n)                                 \
                                                        \
        if ((n)->levelprev)                             \
        {                                               \
            (n)->levelprev->levelnext = (n)->levelnext; \
        }                                               \
        else                                            \
        {                                               \
            level[relold] = (n)->levelnext;             \
        }                                               \
        if ((n)->levelnext)                             \
            (n)->levelnext->levelprev = (n)->levelprev; \
                                                        \
        if (relm < ncount)                              \
        {                                               \
            if (level[relm])                            \
            {                                           \
                level[relm]->levelprev = (n);           \
                (n)->levelnext = level[relm];           \
                (n)->levelprev = (node *)NULL;          \
                level[relm] = (n);                      \
            }                                           \
            else                                        \
            {                                           \
                (n)->levelprev = (node *)NULL;          \
                (n)->levelnext = (node *)NULL;          \
                level[relm] = (n);                      \
            }                                           \
            if (!level[relold])                         \
            {                                           \
                relold++;                               \
                while (level[relold])                   \
                {                                       \
                    node *relno;                        \
                    for (relno = level[relold]; relno;  \
                         relno = relno->levelnext)      \
                    {                                   \
                        relno->flowlabel = ncount;      \
                    }                                   \
                    level[relold] = (node *)NULL;       \
                    relold++;                           \
                }                                       \
            }                                           \
        }                                               \
    }
#else
#define RELABEL(n)           \
    {                        \
        int relm = CC_INFINITY; \
        edge *rele;          \
        int relt;            \
                             \
        RELABEL_BODY(n)      \
    }
#endif /* USE_GAP */

#define CC_SAFE_MALLOC(nnum, type) \
    (type *)CCutil_allocrus(((size_t)(nnum)) * sizeof(type))

#define CC_FREE(object, type)             \
    {                                     \
        CCutil_freerus((void *)(object)); \
        object = (type *)NULL;            \
    }

#define CC_IFFREE(object, type)      \
    {                                \
        if ((object))                \
            CC_FREE((object), type); \
    }

#define ADD_TO_PR_QUEUE(n)                   \
    {                                        \
        if (!(n)->onqueue)                   \
        {                                    \
            (n)->qnext = (CC_SRKnode *)NULL; \
            if (qtail)                       \
                qtail->qnext = (n);          \
            else                             \
                qhead = (n);                 \
            qtail = (n);                     \
            (n)->onqueue = 1;                \
        }                                    \
    }

typedef struct CC_SRKnode
{
    struct CC_SRKedge *adj;
    struct CC_SRKnode *next;
    struct CC_SRKnode *prev;
    struct CC_SRKnode *members;
    struct CC_SRKnode *parent;
    struct CC_SRKnode *qnext;
    double prweight;
    double weight;
    int num;
    int newnum;
    int onecnt;
    int onqueue;
    int mark;
} CC_SRKnode;

typedef struct CC_SRKedge
{
    struct CC_SRKnode *end;
    struct CC_SRKedge *other;
    struct CC_SRKedge *next;
    struct CC_SRKedge *prev;
    double weight;
} CC_SRKedge;

typedef struct CC_SRKgraph
{
    struct CC_SRKnode *nodespace;
    struct CC_SRKedge *edgespace;
    struct CC_SRKnode *head;
    struct CC_SRKedge **hit;
    int original_ncount;
    int original_ecount;
    int marker;
} CC_SRKgraph;

typedef struct CC_SRKexpinfo
{
    int ncount;
    int *members;
    int *memindex;
} CC_SRKexpinfo;

typedef struct CC_SRKcallback
{
    double cutoff;
    void *pass_param;
    int (*doit_fn)(double, int, int *, void *);
} CC_SRKcallback;

typedef struct node
{
    int *adj;
    int degree;
    int mark;

    struct node *qnext;
    struct node *tnext;
#ifdef USE_GAP
    struct node *levelnext;
    struct node *levelprev;
#endif
#ifdef HIGHEST_LABEL_PRF
    struct node *highnext;
#endif
    struct edge *in;
    struct edge *out;
    struct edge *incurrent;
    struct edge *outcurrent;
    double excess;
    int magiclabel;
    int flowlabel;
    char inout;
    char active;
} node;

typedef struct graph
{
    node *nodelist;
    int *adjspace;
    int ncount;
    int ecount;

    struct edge *edgelist;
#ifdef USE_GAP
    struct node **level;
#endif
#ifdef HIGHEST_LABEL_PRF
    struct node **high;
    int highest;
#endif
    int nnodes;
    int nedges;
    int magicnum;
} graph;

typedef struct edge
{
    struct node *ends[2];
    struct edge *innext;
    struct edge *outnext;
    double cap;
    double flow;
} edge;

static int
buildgraph(graph *G, int ncount, int ecount, int *elist, double *gap),
    flip_the_cut(int ncount, int **cut, int *cutcount);
static double flow(graph *G, node *s, node *t);

static void merge_adj(CC_SRKgraph *G, CC_SRKnode *n, CC_SRKnode *m);
static int build_graph(graph *G, int ncount, int ecount, int *elist, double *x);

static int grab_the_cut(graph *G, node *n, int **cut, int *cutcount);
static int test_node(CC_SRKnode *n, double *minval, CC_SRKcallback *cb, int **cut, int *cutcount);
static int expand_the_node(CC_SRKnode *n, int *cutcount, int **cut);
static int expand_and_pass(CC_SRKnode *n, int (*doit_fn)(double, int, int *, void *), void *pass_param);
static int mincut_work(int ncount, int ecount, int *elist, double *dlen,
                       double *cutval, int **cut, int *cutcount, double cutoff,
                       int (*doit_fn)(double, int, int *, void *), void *pass_param);

static void
free_graph(graph *G),
    connect_search(graph *G, int n, int marker, int *dstack),
    init_graph(graph *G),
    free_graph_aux(graph *G, bool aux);
void *CCutil_allocrus(size_t size);
void CCutil_freerus(void *p);
#ifdef __cplusplus
extern "C"
{
#endif
    void
    CCcut_SRK_init_graph(CC_SRKgraph *G),
        CCcut_SRK_free_graph(CC_SRKgraph *G),
        CCcut_SRK_init_expinfo(CC_SRKexpinfo *expand),
        CCcut_SRK_free_expinfo(CC_SRKexpinfo *expand),
        CCcut_SRK_init_callback(CC_SRKcallback *cb),
        CCcut_SRK_identify_nodes(CC_SRKgraph *G, CC_SRKnode *n, CC_SRKnode *m);

    int
    CCcut_violated_cuts(int ncount, int ecount, int *elist, double *dlen,
                        double cutoff, int (*doit_fn)(double, int, int *, void *),
                        void *pass_param),
        CCcut_connect_components(int ncount, int ecount, int *elist, double *x,
                                 int *ncomp, int **compscount, int **comps),
        CCcut_SRK_buildgraph(CC_SRKgraph *G, int ncount, int ecount, int *elist,
                             double *dlen),
        CCcut_SRK_subtour_shrink(CC_SRKgraph *G, double *minval, double epsilon,
                                 CC_SRKcallback *cb, int **cut, int *cutcount),
        CCcut_SRK_grab_edges(CC_SRKgraph *G, int *oncount, int *oecount,
                             int **olist, double **olen, CC_SRKexpinfo *expand),
        CCcut_mincut_st(int ncount, int ecount, int *elist, double *ecap,
                        int s, int t, double *value, int **cut, int *cutcount),
        CCcut_SRK_grab_nodes(CC_SRKgraph *G, CC_SRKexpinfo *expand),
        CCcut_SRK_expand(CC_SRKexpinfo *expand, int *arr, int size, int **pnewarr,
                         int *pnewsize),
        CCcut_SRK_identify_pr_edges(CC_SRKgraph *G, double *minval, int *count,
                                    CC_SRKnode *qstart, double epsilon, CC_SRKcallback *cb, int **cut,
                                    int *cutcount);

#ifdef __cplusplus
}

#endif
