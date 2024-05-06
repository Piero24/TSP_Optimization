#include "mincut.h"


int CCcut_connect_components(int ncount, int ecount, int* elist, double* x, int* ncomp, int** compscount, int** comps)
{
    graph G;
    int i, k, rval;
    int* nmarks = (int*)NULL;
    int* dstack = (int*)NULL;

    G.nodelist = (node*)NULL;
    G.adjspace = (int*)NULL;

    *ncomp = 0;
    *comps = CC_SAFE_MALLOC(ncount, int);
    if (!(*comps)) {
        fprintf(stderr, "out of memory in CCcut_connect_components\n");
        rval = 1; goto CLEANUP;
    }

    rval = build_graph(&G, ncount, ecount, elist, x);
    if (rval) {
        fprintf(stderr, "build_graph failed\n");
        goto CLEANUP;
    }

    dstack = CC_SAFE_MALLOC(ncount, int);
    if (!dstack) {
        fprintf(stderr, "out of memory in CCcut_connect_components\n");
        CC_FREE(*comps, int);
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < ncount; i++) {
        if (!G.nodelist[i].mark) {
            (*ncomp)++;
            connect_search(&G, i, *ncomp, dstack);
        }
    }

    *compscount = CC_SAFE_MALLOC(*ncomp, int);
    nmarks = CC_SAFE_MALLOC(*ncomp, int);
    if (!(*compscount) || !nmarks) {
        fprintf(stderr, "out of memory in CCcut_connect_components\n");
        CC_FREE(*comps, int);
        CC_IFFREE(*compscount, int);
        CC_IFFREE(nmarks, int);
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < *ncomp; i++) {
        nmarks[i] = 0;
    }
    for (i = 0; i < ncount; i++) {
        nmarks[G.nodelist[i].mark - 1]++;
    }
    for (i = 0, k = 0; i < *ncomp; i++) {
        (*compscount)[i] = nmarks[i];
        nmarks[i] = k;
        k += (*compscount)[i];
    }
    for (i = 0; i < ncount; i++) {
        (*comps)[(nmarks[G.nodelist[i].mark - 1])++] = i;
    }

CLEANUP:

    free_graph(&G);
    CC_IFFREE(nmarks, int);
    CC_IFFREE(dstack, int);
    return rval;
}

int CCcut_violated_cuts(int ncount, int ecount, int* elist, double* dlen,
    double cutoff, int (*doit_fn) (double, int, int*, void*),
    void* pass_param)
{
    int rval = 0;

    rval = mincut_work(ncount, ecount, elist, dlen, (double*)NULL,
        (int**)NULL, (int*)NULL, cutoff, doit_fn, pass_param);
    if (rval) {
        fprintf(stderr, "mincut_work failed\n"); goto CLEANUP;
    }

CLEANUP:

    return rval;
}

void* CCutil_allocrus(size_t size)
{
    void* mem = (void*)NULL;

    if (size == 0) {
        fprintf(stderr, "Warning: 0 bytes allocated\n");
    }

    mem = (void*)malloc(size);
    if (mem == (void*)NULL) {
        fprintf(stderr, "Out of memory. Asked for %d bytes\n", (int)size);
    }
    return mem;
}

void CCutil_freerus(void* p)
{
    if (!p) {
        fprintf(stderr, "Warning: null pointer freed\n");
        return;
    }

    free(p);
}

static int build_graph(graph* G, int ncount, int ecount, int* elist,
    double* x)
{
    int rval = 0;
    int i;
    int* p;
    node* nodelist;

    G->nodelist = (node*)NULL;
    G->adjspace = (int*)NULL;
    G->ncount = ncount;
    if (x) {
        G->ecount = 0;
        for (i = 0; i < ecount; i++) {
            if (x[i] > CONNECT_ZERO_EPSILON)
                G->ecount++;
        }
    }
    else {
        G->ecount = ecount;
    }

    G->nodelist = CC_SAFE_MALLOC(G->ncount, node);
    if (!G->nodelist) {
        fprintf(stderr, "out of memory in build_graph\n");
        rval = 1; goto CLEANUP;
    }
    nodelist = G->nodelist;

    if (G->ecount) {
        G->adjspace = CC_SAFE_MALLOC(2 * G->ecount, int);
        if (!G->adjspace) {
            fprintf(stderr, "out of memory in build_graph\n");
            rval = 1; goto CLEANUP;
        }
    }

    for (i = 0; i < ncount; i++) {
        nodelist[i].degree = 0;
        nodelist[i].mark = 0;
    }

    if (x) {
        for (i = 0; i < ecount; i++) {
            if (x[i] > CONNECT_ZERO_EPSILON) {
                nodelist[elist[2 * i]].degree++;
                nodelist[elist[2 * i + 1]].degree++;
            }
        }
    }
    else {
        for (i = 0; i < ecount; i++) {
            nodelist[elist[2 * i]].degree++;
            nodelist[elist[2 * i + 1]].degree++;
        }
    }

    p = G->adjspace;
    for (i = 0; i < ncount; i++) {
        nodelist[i].adj = p;
        p += nodelist[i].degree;
        nodelist[i].degree = 0;
    }

    if (x) {
        for (i = 0; i < ecount; i++) {
            if (x[i] > CONNECT_ZERO_EPSILON) {
                nodelist[elist[2 * i]].adj[nodelist[elist[2 * i]].degree++]
                    = elist[2 * i + 1];
                nodelist[elist[2 * i + 1]].adj[nodelist[elist[2 * i + 1]].degree++]
                    = elist[2 * i];
            }
        }
    }
    else {
        for (i = 0; i < ecount; i++) {
            nodelist[elist[2 * i]].adj[nodelist[elist[2 * i]].degree++]
                = elist[2 * i + 1];
            nodelist[elist[2 * i + 1]].adj[nodelist[elist[2 * i + 1]].degree++]
                = elist[2 * i];
        }
    }

CLEANUP:

    if (rval) {
        CC_IFFREE(G->nodelist, node);
        CC_IFFREE(G->adjspace, int);
    }
    return rval;
}

static void free_graph(graph* G)
{
    CC_IFFREE(G->nodelist, node);
    CC_IFFREE(G->adjspace, int);
}

static void connect_search(graph* G, int n, int marker, int* dstack)
{
    int i, k, head = 0;
    node* nodelist = G->nodelist;

    nodelist[n].mark = marker;
    dstack[head++] = n;

    while (head > 0) {
        n = dstack[--head];
        for (i = 0; i < nodelist[n].degree; i++) {
            k = nodelist[n].adj[i];
            if (!nodelist[k].mark) {
                nodelist[k].mark = marker;
                dstack[head++] = k;
            }
        }
    }
}

static int flip_the_cut(int ncount, int** cut, int* cutcount)
{
    int rval = 0;
    int i;
    char* marks = (char*)NULL;
    int* newcut = (int*)NULL;
    int newcutcount = 0;

    if (*cutcount == ncount) {
        fprintf(stderr, "cut is the entire graph\n");
        rval = 1; goto CLEANUP;
    }

    marks = CC_SAFE_MALLOC(ncount, char);
    if (!marks) {
        fprintf(stderr, "out of memory in flip_the_cut\n");
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < ncount; i++) {
        marks[i] = 0;
    }
    for (i = 0; i < *cutcount; i++) {
        marks[(*cut)[i]] = 1;
    }

    newcut = CC_SAFE_MALLOC(ncount - *cutcount, int);
    if (!newcut) {
        fprintf(stderr, "out of memory in flip_the_cut\n");
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < ncount; i++) {
        if (!marks[i]) {
            newcut[newcutcount++] = i;
        }
    }

    CC_IFFREE(*cut, int);
    *cut = newcut;
    *cutcount = newcutcount;

CLEANUP:

    if (rval) {
        CC_IFFREE(newcut, int);
    }
    CC_IFFREE(marks, char);

    return rval;
}

static int mincut_work(int ncount, int ecount, int* elist, double* dlen,
    double* cutval, int** cut, int* cutcount, double cutoff,
    int (*doit_fn) (double, int, int*, void*), void* pass_param)
{
    int rval = 0;
    CC_SRKgraph G;
    CC_SRKexpinfo E;
    int i, sncount, secount;
    int* slist = (int*)NULL;
    double* slen = (double*)NULL;
    double minval = CC_MINCUT_BIGDOUBLE;
    double val;
    CC_SRKnode* squeue = (CC_SRKnode*)NULL;
    CC_SRKedge* f;
    int* tcut = (int*)NULL;
    int** mytcut = (int**)NULL;
    int tcount = 0;
    CC_SRKcallback* cb = (CC_SRKcallback*)NULL;

    CCcut_SRK_init_graph(&G);
    CCcut_SRK_init_expinfo(&E);
    if (cut) {
        *cut = (int*)NULL;
        if (cutcount) {
            *cutcount = 0;
        }
        else {
            fprintf(stderr, "cut specified, but not cutcount\n");
            rval = 1; goto CLEANUP;
        }
    }
    if (cut || doit_fn) {
        mytcut = &tcut;
    }
    else {
        mytcut = (int**)NULL;
    }
    if (cutval) {
        *cutval = CC_MINCUT_BIGDOUBLE;
    }
    if (doit_fn) {
        cb = CC_SAFE_MALLOC(1, CC_SRKcallback);
        if (!cb) {
            fprintf(stderr, "out of memory in mincut_work\n");
            rval = 1; goto CLEANUP;
        }
        cb->cutoff = cutoff;
        cb->pass_param = pass_param;
        cb->doit_fn = doit_fn;
    }

    rval = CCcut_SRK_buildgraph(&G, ncount, ecount, elist, dlen);
    if (rval) {
        fprintf(stderr, "buildgraph failed in shrink_ones\n"); goto CLEANUP;
    }
    rval = CCcut_SRK_subtour_shrink(&G, &minval, CC_MINCUT_ONE_EPSILON,
        cb, cut, cutcount);
    if (rval) {
        fprintf(stderr, "CCcut_SRK_subtour_shrink failed\n"); goto CLEANUP;
    }

    if (CCcut_SRK_grab_edges(&G, &sncount, &secount, &slist, &slen,
        (CC_SRKexpinfo*)NULL)) {
        fprintf(stderr, "grab edges failed in shrink_ones\n");
        rval = 1; goto CLEANUP;
    }

    while (sncount > 1) {
        if (G.head->adj == (CC_SRKedge*)NULL ||
            G.head->next->adj == (CC_SRKedge*)NULL) {
            fprintf(stderr, "Disconnected graph\n");
            rval = 1; goto CLEANUP;
        }
        rval = CCcut_mincut_st(sncount, secount, slist, slen, 0, 1,
            &val, mytcut, &tcount);
        if (rval) {
            fprintf(stderr, "CCcut_mincut_st failed\n");
            goto CLEANUP;
        }
        if (val < minval) {
            minval = val;
            if (cut) {
                CC_IFFREE(*cut, int);
                rval = CCcut_SRK_grab_nodes(&G, &E);
                if (rval) {
                    fprintf(stderr, "CCcut_SRK_grab_nodes failed\n");
                    goto CLEANUP;
                }
                CCcut_SRK_expand(&E, tcut, tcount, cut, cutcount);
                CCcut_SRK_free_expinfo(&E);
            }
        }
        if (val < cutoff && doit_fn) {
            int* fcut = (int*)NULL;
            int fcutcount = 0;

            rval = CCcut_SRK_grab_nodes(&G, &E);
            if (rval) {
                fprintf(stderr, "CCcut_SRK_grab_nodes failed\n");
                goto CLEANUP;
            }
            CCcut_SRK_expand(&E, tcut, tcount, &fcut, &fcutcount);
            CCcut_SRK_free_expinfo(&E);
            rval = doit_fn(val, fcutcount, fcut, pass_param);
            if (rval) {
                fprintf(stderr, "doit_fn failed\n");
                CC_IFFREE(fcut, int);
                goto CLEANUP;
            }
            CC_IFFREE(fcut, int);
        }

        if (cut || doit_fn) {
            CC_IFFREE(tcut, int);
        }

        CCcut_SRK_identify_nodes(&G, G.head, G.head->next);

        squeue = (CC_SRKnode*)NULL;
        for (f = G.head->adj; f; f = f->next) {
            f->end->qnext = squeue;
            squeue = f->end;
        }
        G.head->qnext = squeue;
        squeue = G.head;

        CCcut_SRK_identify_pr_edges(&G, &minval, &i, squeue,
            CC_MINCUT_ONE_EPSILON, cb, cut, cutcount);

        /* if (i) { printf ("[%d]", i); fflush (stdout); } */

        CC_IFFREE(slist, int);
        CC_IFFREE(slen, double);
        rval = CCcut_SRK_grab_edges(&G, &sncount, &secount, &slist, &slen,
            (CC_SRKexpinfo*)NULL);
        if (rval) {
            fprintf(stderr, "grab edges failed in shrink_ones\n");
            goto CLEANUP;
        }
    }

    if (cutval) {
        *cutval = minval;
    }

    if (cut) {
        if (*cutcount > ncount / 2) {
            rval = flip_the_cut(ncount, cut, cutcount);
            if (rval) {
                fprintf(stderr, "flip_the_cut failed\n"); goto CLEANUP;
            }
        }
    }

CLEANUP:

    if (rval) {
        if (cut) {
            CC_IFFREE(*cut, int);
        }
    }
    CC_IFFREE(slist, int);
    CC_IFFREE(slen, double);
    CC_IFFREE(tcut, int);
    CCcut_SRK_free_graph(&G);
    CCcut_SRK_free_expinfo(&E);
    CC_IFFREE(cb, CC_SRKcallback);

    return rval;
}

void CCcut_SRK_init_graph(CC_SRKgraph* G)
{
    if (G) {
        G->nodespace = (CC_SRKnode*)NULL;
        G->edgespace = (CC_SRKedge*)NULL;
        G->head = (CC_SRKnode*)NULL;
        G->hit = (CC_SRKedge**)NULL;
        G->original_ncount = 0;
        G->original_ecount = 0;
        G->marker = 0;
    }
}

void CCcut_SRK_free_graph(CC_SRKgraph* G)
{
    if (G) {
        CC_IFFREE(G->nodespace, CC_SRKnode);
        CC_IFFREE(G->edgespace, CC_SRKedge);
        CC_IFFREE(G->hit, CC_SRKedge*);
    }
}

void CCcut_SRK_init_expinfo(CC_SRKexpinfo* expand)
{
    expand->ncount = 0;
    expand->memindex = (int*)NULL;
    expand->members = (int*)NULL;
}

void CCcut_SRK_free_expinfo(CC_SRKexpinfo* expand)
{
    expand->ncount = 0;
    CC_IFFREE(expand->memindex, int);
    CC_IFFREE(expand->members, int);
}

void CCcut_SRK_init_callback(CC_SRKcallback* cb)
{
    if (cb) {
        cb->cutoff = 0.0;
        cb->pass_param = (void*)NULL;
        cb->doit_fn = (int (*) (double, int, int*, void*)) NULL;
    }
}

void CCcut_SRK_identify_nodes(CC_SRKgraph* G, CC_SRKnode* n, CC_SRKnode* m)
{
    CC_SRKedge* e;

    m->parent = n;
    n->weight += m->weight;

    if (!n->members) {
        n->members = m;
    }
    else if (!m->members) {
        m->members = n->members;
        n->members = m;
    }
    else {
        CC_SRKnode* t;
        for (t = n->members; t->members; t = t->members);
        t->members = m;
    }

    for (e = m->adj; e; e = e->next) {
        e->other->end = n;
    }

    merge_adj(G, n, m);

    if (m->prev)
        m->prev->next = m->next;
    else
        G->head = m->next;
    if (m->next)
        m->next->prev = m->prev;
}

static void merge_adj(CC_SRKgraph* G, CC_SRKnode* n, CC_SRKnode* m)
{
    CC_SRKedge* e, * f, * last, * work;
    CC_SRKedge** hit = G->hit;

    /* String together the two lists */

    if (n->adj) {
        for (last = n->adj; last->next; last = last->next);
        last->next = m->adj;
        if (m->adj)
            m->adj->prev = last;
        work = n->adj;
    }
    else {
        work = m->adj;
    }

    /* Remove any edges that become loops */

    while (work && work->end == n) {
        n->weight -= work->weight;
        work = work->next;
    }

    if (work) {
        work->prev = (CC_SRKedge*)NULL;
        for (e = work->next; e; e = e->next) {
            if (e->end == n) {
                n->weight -= e->weight;
                e->prev->next = e->next;
                if (e->next)
                    e->next->prev = e->prev;
            }
        }
    }
    else {
        n->adj = (CC_SRKedge*)NULL;
        return;
    }

    /* Remove the duplicate edges in the work list */

    hit[work->end->num] = work;
    for (e = work->next; e; e = e->next) {
        if (hit[e->end->num]) {
            /* A duplicate edge */

            hit[e->end->num]->weight += e->weight;
            hit[e->end->num]->other->weight = hit[e->end->num]->weight;
            e->prev->next = e->next;
            if (e->next)
                e->next->prev = e->prev;

            /* Fix the adj of the other end of the duplicate */

            f = e->other;
            if (f->prev) {
                f->prev->next = f->next;
            }
            else {
                e->end->adj = f->next;
            }
            if (f->next) {
                f->next->prev = f->prev;
            }
        }
        else {
            hit[e->end->num] = e;
        }
    }

    for (e = work; e; e = e->next)
        hit[e->end->num] = (CC_SRKedge*)NULL;
    n->adj = work;
}

int CCcut_SRK_buildgraph(CC_SRKgraph* G, int ncount, int ecount, int* elist,
    double* dlen)
{
    int i;
    int* degree = (int*)NULL;
    int newecount = 0;
    CC_SRKnode* nodespace, * n, * n1, * n2;
    CC_SRKedge* e, * adj1, * adj2;
    CC_SRKedge** hit;

    G->nodespace = CC_SAFE_MALLOC(ncount, CC_SRKnode);
    G->hit = CC_SAFE_MALLOC(ncount, CC_SRKedge*);
    if (!G->nodespace || !G->hit) {
        fprintf(stderr, "out of memory in CCcut_SRK_buildgraph\n");
        CC_IFFREE(G->nodespace, CC_SRKnode);
        CC_IFFREE(G->hit, CC_SRKedge*);
        return 1;
    }
    nodespace = G->nodespace;
    hit = G->hit;
    G->head = nodespace;
    G->original_ncount = ncount;
    G->original_ecount = ecount;
    G->marker = 0;

    degree = CC_SAFE_MALLOC(ncount, int);
    if (!degree) {
        fprintf(stderr, "out of memory in CCcut_SRK_buildgraph\n");
        CC_IFFREE(G->nodespace, CC_SRKnode);
        CC_IFFREE(G->hit, CC_SRKedge*);
        return 1;
    }

    for (i = 0, n = nodespace; i < ncount; i++, n++) {
        n->prev = n - 1;
        n->next = n + 1;
        n->num = i;
        n->members = (CC_SRKnode*)NULL;
        n->parent = n;
        n->prweight = -CC_MINCUT_BIGDOUBLE;
        n->weight = 0.0;
        hit[i] = (CC_SRKedge*)NULL;
        degree[i] = 0;
        n->onecnt = 0;
        n->mark = 0;
    }
    nodespace[0].prev = (CC_SRKnode*)NULL;
    nodespace[ncount - 1].next = (CC_SRKnode*)NULL;

    for (i = 0; i < ecount; i++) {
        if (dlen[i] > SRK_ZERO_EPSILON) {
            newecount++;
            degree[elist[2 * i]]++;
            degree[elist[2 * i + 1]]++;
        }
    }
    G->edgespace = CC_SAFE_MALLOC(2 * newecount, CC_SRKedge);
    if (!G->edgespace) {
        fprintf(stderr, "out of memory in CCcut_SRK_buildgraph\n");
        CC_IFFREE(G->nodespace, CC_SRKnode);
        CC_IFFREE(G->hit, CC_SRKedge*);
        return 1;
    }

    for (e = G->edgespace, i = 0; i < ncount; i++) {
        nodespace[i].adj = e;
        e += degree[i];
    }
    for (i = 0; i < ecount; i++) {
        if (dlen[i] > SRK_ZERO_EPSILON) {
            n1 = nodespace + elist[2 * i];
            n2 = nodespace + elist[2 * i + 1];
            adj1 = n1->adj;
            adj2 = n2->adj;
            adj1->end = n2;
            adj1->weight = dlen[i];
            adj1->next = adj1 + 1;
            adj1->prev = adj1 - 1;
            adj1->other = adj2;
            adj2->end = n1;
            adj2->weight = dlen[i];
            adj2->next = adj2 + 1;
            adj2->prev = adj2 - 1;
            adj2->other = adj1;
            n1->adj++;
            n2->adj++;
            if (dlen[i] == 1.0) {
                n1->onecnt++;
                n2->onecnt++;
            }
        }
    }
    for (e = G->edgespace, i = 0; i < ncount; i++) {
        if (degree[i]) {
            (nodespace[i].adj - 1)->next = (CC_SRKedge*)NULL;
            nodespace[i].adj = e;
            nodespace[i].adj->prev = (CC_SRKedge*)NULL;
            e += degree[i];
        }
        else {
            nodespace[i].adj = (CC_SRKedge*)NULL;
        }
    }

    for (i = 0, n = nodespace; i < ncount; i++, n++) {
        for (e = n->adj; e; e = e->next) {
            n->weight += e->weight;
        }
    }

    CC_IFFREE(degree, int);
    return 0;
}

int CCcut_SRK_subtour_shrink(CC_SRKgraph* G, double* minval, double epsilon,
    CC_SRKcallback* cb, int** cut, int* cutcount)
{
    int rval = 0;
    int k;
    int cnt = 0;
    CC_SRKnode* n;

    for (n = G->head; n; n = n->next) {
        cnt++;
    }

    /* In the tsp, paths of 1's can be shrunk via a call to the function  */
    /* CCcut_SRK_identify_paths.                                          */

    /* Could call a version of CCcut_SRK_identify_ones */

    /* printf ("Identify PR edges ....\n"); fflush (stdout); */
    rval = CCcut_SRK_identify_pr_edges(G, minval, &k, (CC_SRKnode*)NULL,
        epsilon, cb, cut, cutcount);
    if (rval) {
        fprintf(stderr, "CCcut_SRK_identify_pr_edges failed\n"); goto CLEANUP;
    }

    cnt -= k;
    /* printf ("Graph shrunk to %d nodes\n", cnt); fflush (stdout); */

CLEANUP:

    return rval;
}

int CCcut_SRK_grab_edges(CC_SRKgraph* G, int* oncount, int* oecount,
    int** olist, double** olen, CC_SRKexpinfo* expand)
{
    int rval = 0;
    int k, num;
    int ncount = 0, ecount = 0;
    CC_SRKnode* n;
    CC_SRKedge* e;

    *oncount = 0;
    *oecount = 0;
    *olist = (int*)NULL;
    *olen = (double*)NULL;
    if (expand) {
        CCcut_SRK_init_expinfo(expand);
    }

    for (n = G->head; n; n = n->next) {
        n->newnum = ncount;
        for (e = n->adj; e; e = e->next)
            ecount++;
        ncount++;
    }

    if (ecount % 2) {
        fprintf(stderr, "Error in grab_edges\n");
        rval = 1; goto CLEANUP;
    }
    else {
        ecount /= 2;
    }

    if (ecount == 0) {
        rval = 0; goto CLEANUP;
    }

    *olist = CC_SAFE_MALLOC(ecount * 2, int);
    *olen = CC_SAFE_MALLOC(ecount, double);
    if (!(*olist) || !(*olen)) {
        fprintf(stderr, "out of memory in grab_edges\n");
        rval = 1; goto CLEANUP;
    }

    k = 0;
    for (n = G->head; n; n = n->next) {
        num = n->newnum;
        for (e = n->adj; e; e = e->next) {
            if (num < e->end->newnum) {
                (*olist)[2 * k] = num;
                (*olist)[2 * k + 1] = e->end->newnum;
                (*olen)[k++] = e->weight;
            }
        }
    }
    if (k != ecount) {
        fprintf(stderr, "Error in grab_edges\n");
        rval = 1; goto CLEANUP;
    }

    *oncount = ncount;
    *oecount = ecount;

    if (expand) {
        rval = CCcut_SRK_grab_nodes(G, expand);
        if (rval) {
            fprintf(stderr, "CCcut_SRK_grab_nodes failed\n"); goto CLEANUP;
        }
    }

CLEANUP:

    if (rval) {
        CC_IFFREE(*olist, int);
        CC_IFFREE(*olen, double);
        if (expand) {
            CCcut_SRK_free_expinfo(expand);
        }
    }

    return rval;
}

int CCcut_SRK_grab_nodes(CC_SRKgraph* G, CC_SRKexpinfo* expand)
{
    int rval = 0;
    int i;
    CC_SRKnode* n, * m;
    int total = 0;
    int ncount = 0;

    if (!expand) {
        fprintf(stderr, "CCcut_SRK_grab_nodes called without an expand struct\n");
        rval = 1; goto CLEANUP;
    }

    for (n = G->head; n; n = n->next) {
        ncount++;
    }

    CCcut_SRK_init_expinfo(expand);
    expand->ncount = ncount;
    expand->members = CC_SAFE_MALLOC(G->original_ncount, int);
    expand->memindex = CC_SAFE_MALLOC(ncount + 1, int);
    if (!(expand->members) || !(expand->memindex)) {
        fprintf(stderr, "out of memory in grab_nodes\n");
        rval = 1; goto CLEANUP;
    }
    for (n = G->head, i = 0; n; n = n->next, i++) {
        expand->memindex[i] = total;
        expand->members[total++] = n->num;
        for (m = n->members; m; m = m->members)
            expand->members[total++] = m->num;
    }
    expand->memindex[i] = total;

CLEANUP:

    if (rval) CCcut_SRK_free_expinfo(expand);
    return rval;
}

int CCcut_mincut_st(int ncount, int ecount, int* elist, double* ecap,
    int s, int t, double* value, int** cut, int* cutcount)
{
    int rval = 0;
    graph G;

    init_graph(&G);

    if (cut) {
        *cut = (int*)NULL;
        if (cutcount) {
            *cutcount = 0;
        }
        else {
            fprintf(stderr, "cut is specified but not cutcount\n");
            rval = 1; goto CLEANUP;
        }
    }

    rval = buildgraph(&G, ncount, ecount, elist, ecap);
    if (rval) {
        fprintf(stderr, "Buildgraph failed\n"); goto CLEANUP;
    }
    *value = flow(&G, G.nodelist + s, G.nodelist + t);
    if (cut) {
        rval = grab_the_cut(&G, G.nodelist + t, cut, cutcount);
        if (rval) {
            fprintf(stderr, "grab_the_cut failed\n"); goto CLEANUP;
        }
    }


CLEANUP:

    free_graph_aux(&G, true);
    return rval;
}

int CCcut_SRK_expand(CC_SRKexpinfo* expand, int* arr, int size, int** pnewarr,
    int* pnewsize)
{
    int newsize = 0;
    int* newarr = (int*)NULL;
    int i, j, jend;

    *pnewsize = 0;
    *pnewarr = (int*)NULL;
    for (i = 0; i < size; i++) {
        newsize += expand->memindex[arr[i] + 1] - expand->memindex[arr[i]];
    }
    newarr = CC_SAFE_MALLOC(newsize, int);
    if (!newarr) {
        fprintf(stderr, "Out of memory in CCcut_SRK_expand\n");
        return -1;
    }
    newsize = 0;
    for (i = 0; i < size; i++) {
        for (j = expand->memindex[arr[i]], jend = expand->memindex[arr[i] + 1];
            j < jend; j++) {
            newarr[newsize++] = expand->members[j];
        }
    }
    *pnewarr = newarr;
    *pnewsize = newsize;
    return 0;
}

int CCcut_SRK_identify_pr_edges(CC_SRKgraph* G, double* minval, int* count,
    CC_SRKnode* qstart, double epsilon, CC_SRKcallback* cb, int** cut,
    int* cutcount)
{
    int rval = 0;
    CC_SRKnode* n;
    CC_SRKedge* e, * f, * h;
    double tol, tol1, tol2;
    CC_SRKnode* qhead, * qtail;

    /* Trivial PR Test: If w(uv) >= c(u)/2, then we can shrink edge uv.   */

    /* First PR Test: If we have a triangle uv, uw, vw with               */
    /* w(uv) + w(vw) >= c(v)/2 and w(uw) + w(vw) >= c(w)/2, then we can   */
    /* shrink edge vw.                                                    */

    /* Second PR Test: Compute a lower bound on any cut that separates    */
    /* the ends of an edge by summing the minimum values of the edges to  */
    /* common neighbors of the ends. If the bound is >= "2", then we can  */
    /* shrink the edge.                                                   */

    /* Third PR Test: Edge uv with common neighbors xy. If                */
    /* w(ux) + w(uy) + w(uv) >= "1" and w(vx) + w(vy) + w(uv) >= "1" and  */
    /* at least one of w(uy) + w(uv) and w(vx) + w(uv) is >= "1" and      */
    /* at least one of w(ux) + w(uv) and w(vy) + w(uv) is >= "1" then we  */
    /* can shrink the edge uv.                                            */

    * count = 0;

    if (cut && !cutcount) {
        fprintf(stderr, "cut defined, but not cutcount\n");
        rval = 1; goto CLEANUP;
    }

    if (qstart) {
        qhead = qstart;
        for (n = qstart; n->next; n = n->next)
            n->onqueue = 1;
        qtail = n;
        qtail->onqueue = 1;
    }
    else {
        for (n = G->head; n->next; n = n->next) {
            n->qnext = n->next;
            n->onqueue = 1;
        }
        qhead = G->head;
        qtail = n;
        qtail->onqueue = 1;
        qtail->qnext = (CC_SRKnode*)NULL;
    }

    while (qhead) {
        n = qhead;
        qhead = qhead->qnext;
        if (!qhead)
            qtail = (CC_SRKnode*)NULL;
        if (n->parent != n)
            continue;
        n->onqueue = 0;
        tol = n->weight / 2.0 - epsilon;

        for (e = n->adj; e && e->weight < tol; e = e->next);
        if (e) {
            rval = test_node(n, minval, cb, cut, cutcount);
            if (rval) { fprintf(stderr, "test_node failed\n"); goto CLEANUP; }
            rval = test_node(e->end, minval, cb, cut, cutcount);
            if (rval) { fprintf(stderr, "test_node failed\n"); goto CLEANUP; }
            CCcut_SRK_identify_nodes(G, n, e->end);
            (*count)++;
            ADD_TO_PR_QUEUE(n);
            for (h = n->adj; h; h = h->next) {
                ADD_TO_PR_QUEUE(h->end);
            }
        }
        else {
            for (e = n->adj; e; e = e->next)
                e->end->prweight = e->weight;
            for (e = n->adj; e; e = e->next) {
                tol1 = (n->weight / 2.0) - e->weight - epsilon;
                tol2 = (e->end->weight / 2.0) - e->weight - epsilon;
                for (f = e->end->adj; f; f = f->next) {
                    if (f->weight >= tol2 && f->end->prweight >= tol1) {
                        rval = test_node(n, minval, cb, cut, cutcount);
                        if (rval) {
                            fprintf(stderr, "test_node failed\n");
                            goto CLEANUP;
                        }
                        rval = test_node(e->end, minval, cb, cut, cutcount);
                        if (rval) {
                            fprintf(stderr, "test_node failed\n");
                            goto CLEANUP;
                        }
                        CCcut_SRK_identify_nodes(G, n, e->end);
                        (*count)++;
                        ADD_TO_PR_QUEUE(n);
                        for (h = n->adj; h; h = h->next) {
                            ADD_TO_PR_QUEUE(h->end);
                        }
                        goto GET_OUT;
                    }
                }
            }

#ifdef PR_USE3
            - Must modify to use node current min cut value.
                for (e = n->adj; e; e = e->next) {
                    tol = e->weight;
                    for (f = e->end->adj; f; f = f->next) {
                        if (f->end->prweight >= f->weight)
                            tol += f->weight;
                        else if (f->end->prweight > -CC_MINCUT_BIGDOUBLE)
                            tol += f->end->prweight;
                    }
                    if (tol >= 1.0 + onetol) {
                        printf("X"); fflush(stdout);
                        CCcut_SRK_identify_nodes(G, n, e->end);
                        (*count)++;
                        ADD_TO_PR_QUEUE(n);
                        for (h = n->adj; h; h = h->next) {
                            ADD_TO_PR_QUEUE(h->end);
                        }
                        goto GET_OUT;
                    }
                }
#endif

#ifdef PR_USE4
            - Must modify to use node weights.
                for (e = n->adj; e; e = e->next) {
                    tol = onetol - e->weight;
                    for (f = e->end->adj; f; f = f->next) {
                        if (f->end->prweight > -CC_MINCUT_BIGDOUBLE) {
                            for (h = f->next; h; h = h->next) {
                                if (h->end->prweight > -CC_MINCUT_BIGDOUBLE) {
                                    if (f->weight + h->weight >= tol
                                        && f->end->prweight + h->end->prweight >= tol
                                        && (f->weight >= tol || h->end->prweight >= tol)
                                        && (h->weight >= tol || f->end->prweight >= tol)) {
                                        CCcut_SRK_identify_nodes(G, n, e->end);
                                        (*count)++;
                                        ADD_TO_PR_QUEUE(n);
                                        for (h = n->adj; h; h = h->next) {
                                            ADD_TO_PR_QUEUE(h->end);
                                        }
                                        goto GET_OUT;
                                    }
                                }
                            }
                        }
                    }
                }
#endif

        GET_OUT:
            for (e = n->adj; e; e = e->next)
                e->end->prweight = -CC_MINCUT_BIGDOUBLE;
        }
    }

CLEANUP:

    return rval;
}

static int test_node(CC_SRKnode* n, double* minval, CC_SRKcallback* cb,
    int** cut, int* cutcount)
{
    int rval = 0;

    if (n->weight < *minval) {
        *minval = n->weight;
        /* printf ("New minimum: %f\n", *minval); */
        if (cut) {
            CC_IFFREE(*cut, int);
            rval = expand_the_node(n, cutcount, cut);
            if (rval) {
                fprintf(stderr, "expand_the_node failed\n"); goto CLEANUP;
            }
        }
    }
    if (cb) {
        if (n->weight <= cb->cutoff) {
            rval = expand_and_pass(n, cb->doit_fn, cb->pass_param);
            if (rval) {
                fprintf(stderr, "expand_and_pass failed\n"); goto CLEANUP;
            }
        }
    }

CLEANUP:

    return rval;
}

static int expand_and_pass(CC_SRKnode* n, int (*doit_fn) (double, int, int*,
    void*), void* pass_param)
{
    int rval = 0;
    int cutcount;
    int* cut = (int*)NULL;

    if (!doit_fn) goto CLEANUP;

    rval = expand_the_node(n, &cutcount, &cut);
    if (rval) {
        fprintf(stderr, "expand_the_node failed\n"); fflush(stdout);
    }

    rval = doit_fn(n->weight, cutcount, cut, pass_param);
    if (rval) {
        fprintf(stderr, "doit_fn failed\n"); goto CLEANUP;
    }

CLEANUP:

    CC_IFFREE(cut, int);
    return rval;
}

static int expand_the_node(CC_SRKnode* n, int* cutcount, int** cut)
{
    int rval = 0;
    int cnt;
    int* tcut = (int*)NULL;
    CC_SRKnode* v;

    *cutcount = 0;
    *cut = (int*)NULL;

    cnt = 1;
    for (v = n->members; v; v = v->members) {
        cnt++;
    }
    tcut = CC_SAFE_MALLOC(cnt, int);
    if (!tcut) {
        fprintf(stderr, "out of memory in expand_the_node\n");
        rval = 1; goto CLEANUP;
    }

    tcut[0] = n->num;
    cnt = 1;
    for (v = n->members; v; v = v->members) {
        tcut[cnt++] = v->num;
    }

    *cutcount = cnt;
    *cut = tcut;

CLEANUP:

    return rval;
}

static void init_graph(graph* G)
{
    if (G) {
        G->nodelist = (node*)NULL;
        G->edgelist = (edge*)NULL;
#ifdef USE_GAP
        G->level = (node**)NULL;
#endif
#ifdef HIGHEST_LABEL_PRF
        G->high = (node**)NULL;
#endif
    }
}

static int buildgraph(graph* G, int ncount, int ecount, int* elist,
    double* ecap)
{
    int i;
    edge* edgelist;
    node* nodelist;

    G->nodelist = (node*)NULL;
    G->edgelist = (edge*)NULL;
#ifdef USE_GAP
    G->level = (node**)NULL;
#endif
#ifdef HIGHEST_LABEL_PRF
    G->high = (node**)NULL;
#endif

    G->magicnum = 0;
    G->nnodes = ncount;
    G->nedges = ecount;
    G->nodelist = CC_SAFE_MALLOC(ncount, node);
    G->edgelist = CC_SAFE_MALLOC(ecount, edge);
    if (!G->nodelist || !G->edgelist) {
        fprintf(stderr, "Out of memory in buildgraph\n");
        CC_IFFREE(G->nodelist, node);
        CC_IFFREE(G->edgelist, edge);
        return 1;
    }
#ifdef USE_GAP
    G->level = CC_SAFE_MALLOC(ncount + 1, node*);
    if (!G->level) {
        fprintf(stderr, "Out of memory in buildgraph\n");
        CC_IFFREE(G->nodelist, node);
        CC_IFFREE(G->edgelist, edge);
        return 1;
    }
    for (i = 0; i < ncount; i++)
        G->level[i] = (node*)NULL;
    G->level[ncount] = (node*)NULL;  /* A guard dog for a while loop */
#endif

#ifdef HIGHEST_LABEL_PRF
    G->high = CC_SAFE_MALLOC(ncount, node*);
    if (!G->high) {
        fprintf(stderr, "Out of memory in buildgraph\n");
        CC_IFFREE(G->nodelist, node);
        CC_IFFREE(G->edgelist, edge);
        return 1;
    }
#endif

    nodelist = G->nodelist;
    edgelist = G->edgelist;

    for (i = 0; i < ncount; i++) {
        nodelist[i].in = (edge*)NULL;
        nodelist[i].out = (edge*)NULL;
        nodelist[i].magiclabel = 0;
    }

    for (i = 0; i < ecount; i++) {
        int tail = elist[2 * i];
        int head = elist[(2 * i) + 1];
        if (tail < 0 || tail >= ncount ||
            head < 0 || head >= ncount) {
            fprintf(stderr, "Edge list in wrong format: Edge %d = [%d, %d]\n",
                i, tail, head);
            return 1;
        }
        edgelist[i].ends[0] = nodelist + tail;
        edgelist[i].ends[1] = nodelist + head;
        edgelist[i].cap = ecap[i];
        edgelist[i].outnext = nodelist[tail].out;
        nodelist[tail].out = &(edgelist[i]);
        edgelist[i].innext = nodelist[head].in;
        nodelist[head].in = &(edgelist[i]);
    }
    return 0;
}

static int grab_the_cut(graph* G, node* n, int** cut, int* cutcount)
{
    int rval = 0;
    edge* e;
    node* q, * top;
    int count = 0;
    int i, num;
    node* nodelist = G->nodelist;
    int* tcut = (int*)NULL;

    *cut = (int*)NULL;
    *cutcount = 0;

    tcut = CC_SAFE_MALLOC(G->nnodes, int);
    if (!tcut) {
        fprintf(stderr, "out of memory in grab_the_cut\n");
        rval = 1; goto CLEANUP;
    }

    G->magicnum++;
    num = G->magicnum;
    tcut[count++] = (int)(n - nodelist);
    q = n;
    q->magiclabel = num;
    q->tnext = (node*)NULL;

    while (q) {
        top = q;
        q = q->tnext;
        for (e = top->out; e; e = e->outnext) {
#ifndef UNDIRECTED_GRAPH
            if (e->cap + e->flow > 0.0 && e->ends[1]->magiclabel != num) {
#else
            if (e->flow > 0.0 && e->ends[1]->magiclabel != num) {
#endif
                tcut[count++] = (int)(e->ends[1] - nodelist);
                e->ends[1]->magiclabel = num;
                e->ends[1]->tnext = q;
                q = e->ends[1];
            }
            }
        for (e = top->in; e; e = e->innext) {
            if (e->cap - e->flow > 0.0 && e->ends[0]->magiclabel != num) {
                tcut[count++] = (int)(e->ends[0] - nodelist);
                e->ends[0]->magiclabel = num;
                q = e->ends[0];
            }
        }
        }

    *cut = CC_SAFE_MALLOC(count, int);
    if (!(*cut)) {
        fprintf(stderr, "out of memory in grab_the_cut\n");
        rval = 1; goto CLEANUP;
    }
    for (i = 0; i < count; i++) {
        (*cut)[i] = tcut[i];
    }
    *cutcount = count;

CLEANUP:

    if (rval) {
        CC_IFFREE(*cut, int);
    }
    CC_IFFREE(tcut, int);
    return rval;
    }

static void backwards_bfs(node * s, int K, graph * G)
{
    node* this1, * next, * tail;
    edge* e;
    int  dist;
#ifdef USE_GAP
    node dummy;
    node** level = G->level;
#endif
#ifdef HIGHEST_LABEL_PRF
    node** high = G->high;
#endif

    s->magiclabel = K;
    next = s;
    s->tnext = (node*)NULL;
    dist = s->flowlabel;

#ifdef USE_GAP
    {
        int i;
        for (i = 0; level[i]; i++)
            level[i] = (node*)NULL;
        level[dist] = s;
        s->levelnext = (node*)NULL;
    }
#endif
#ifdef HIGHEST_LABEL_PRF
    {
        int i;
        for (i = 0; i <= G->highest; i++)
            high[i] = (node*)NULL;
        G->highest = 0;
    }
#endif

    do {
#ifdef USE_GAP
        level[dist]->levelprev = (node*)NULL;
        level[dist + 1] = &dummy;
        dummy.levelprev = (node*)NULL;
#endif
        dist++;
        for (this1 = next, next = (node*)NULL; this1; this1 = this1->tnext) {
            for (e = this1->out; e; e = e->outnext) {
                tail = e->ends[1];
#ifdef UNDIRECTED_GRAPH
                if (tail->magiclabel != K && e->cap + e->flow > PRF_EPSILON) {
#else
                if (tail->magiclabel != K && e->flow > PRF_EPSILON) {
#endif
                    tail->flowlabel = dist;
                    tail->tnext = next;
                    next = tail;
                    tail->magiclabel = K;
#ifdef USE_GAP
                    tail->levelnext = level[dist];
                    level[dist]->levelprev = tail;
                    level[dist] = tail;
#endif
#ifdef HIGHEST_LABEL_PRF
                    if (tail->active) {
                        tail->highnext = high[dist];
                        high[dist] = tail;
                    }
#endif
                }
                }
            for (e = this1->in; e; e = e->innext) {
                tail = e->ends[0];
                if (tail->magiclabel != K && e->cap - e->flow > PRF_EPSILON) {
                    tail->flowlabel = dist;
                    tail->tnext = next;
                    next = tail;
                    tail->magiclabel = K;
#ifdef USE_GAP
                    tail->levelnext = level[dist];
                    level[dist]->levelprev = tail;
                    level[dist] = tail;
#endif
#ifdef HIGHEST_LABEL_PRF
                    if (tail->active) {
                        tail->highnext = high[dist];
                        high[dist] = tail;
                    }
#endif
                }
            }
            }
#ifdef USE_GAP
        if (dummy.levelprev) {
            dummy.levelprev->levelnext = (node*)NULL;
            level[dist]->levelprev = (node*)NULL;
        }
        else {
            level[dist] = (node*)NULL;
        }
#endif
#ifdef HIGHEST_LABEL_PRF
        if (high[dist])
            G->highest = dist;
#endif
        } while (next);
    }

static void setlabels(graph * G, node * s, node * t)
{
    node* n;
    int ncount = G->nnodes;
    int num = ++(G->magicnum);
    int i;
    /* static int duke = 0; */

    t->flowlabel = 0;
    backwards_bfs(t, num, G);
    if (s->magiclabel == num) {
        printf("Help - s should not get a label\n");
        s->flowlabel = ncount;
    }

    for (i = G->nnodes, n = G->nodelist; i; i--, n++) {
        n->outcurrent = n->out;
        n->incurrent = n->in;
        n->inout = GOING_OUT;
        if (n->magiclabel != num) {
            n->flowlabel = ncount;
        }
    }
}

static double flow(graph * G, node * s, node * t)
{
#ifdef QUEUE_PRF
    node* qhead = (node*)NULL;
    node* qtail = (node*)NULL;
#endif
    node* n;
    edge* e;
    int count, round;
    int i;
    int ncount = G->nnodes;
    edge* edgelist = G->edgelist;
    node* nodelist = G->nodelist;
#ifdef USE_GAP
    node** level = G->level;
#endif
#ifdef HIGHEST_LABEL_PRF
    node** high = G->high;
#endif

    /*
        printf ("Find cut separating %d and %d ...\n", s - nodelist, t - nodelist);
        fflush (stdout);
    */

    for (i = 0; i < ncount; i++) {
        nodelist[i].excess = 0.0;
        nodelist[i].active = 0;
#ifdef HIGHEST_LABEL_PRF
        high[i] = (node*)NULL;
#endif
    }
#ifdef HIGHEST_LABEL_PRF
    G->highest = 0;
#endif

    for (i = G->nedges - 1; i >= 0; i--)
        edgelist[i].flow = 0.0;

    t->active = 1;              /* a lie, which keeps s and t off the */
    s->active = 1;              /* active int                         */

    for (e = s->out; e; e = e->outnext) {
        if (e->cap > 0.0) {
            e->flow = e->cap;
            e->ends[1]->excess += e->cap;
#ifdef QUEUE_PRF
            ADD_TO_ACTIVE(e->ends[1]);
#endif
#ifdef HIGHEST_LABEL_PRF
            e->ends[1]->active = 1;
#endif
        }
    }
#ifdef UNDIRECTED_GRAPH
    for (e = s->in; e; e = e->innext) {
        if (e->cap > 0.0) {
            e->flow = -e->cap;
            e->ends[0]->excess += e->cap;
#ifdef QUEUE_PRF
            ADD_TO_ACTIVE(e->ends[0]);
#endif
#ifdef HIGHEST_LABEL_PRF
            e->ends[0]->active = 1;
#endif
        }
    }
#endif
    setlabels(G, s, t);
    count = 0;
    round = (int)(GLOBAL_RELABEL_FREQUENCY * ncount);

#ifdef QUEUE_PRF
    while (qhead) {
        n = qhead;
        qhead = qhead->qnext;
        if (!qhead)
            qtail = (node*)NULL;
        n->active = 0;
        if (n->flowlabel >= ncount)
            continue;
#endif

#ifdef HIGHEST_LABEL_PRF
        while (G->highest) {
            n = high[G->highest];
            n->active = 0;
            high[G->highest] = high[G->highest]->highnext;
            if (!high[G->highest]) {
                G->highest--;
                while (G->highest && (high[G->highest] == (node*)NULL))
                    G->highest--;
            }
#endif

            if (count == round) {
                setlabels(G, s, t);
                if (n->flowlabel >= ncount)
                    continue;
                count = 0;
            }
            else
                count++;

            if (n->inout == GOING_IN)
                goto DO_ME_IN;

            if (n->outcurrent) {
                while (n->excess > 0.0) {
                    e = n->outcurrent;
                    { /* PUSH OUT */
                        double rf = e->cap - e->flow;
                        node* n1 = e->ends[1];
                        if (n->flowlabel == n1->flowlabel + 1 && rf > 0.0) {
                            if (n->excess <= rf) {
                                e->flow += n->excess;
                                n1->excess += n->excess;
                                n->excess = 0.0;
                                ADD_TO_ACTIVE(n1);
                            }
                            else {
                                e->flow += rf;
                                n1->excess += rf;
                                n->excess -= rf;
                                ADD_TO_ACTIVE(n1);
                                n->outcurrent = e->outnext;
                                if (!n->outcurrent) {
                                    n->outcurrent = n->out;
                                    n->inout = GOING_IN;
                                    goto DO_ME_IN;
                                }
                            }
                        }
                        else {
                            n->outcurrent = e->outnext;
                            if (!n->outcurrent) {
                                n->outcurrent = n->out;
                                n->inout = GOING_IN;
                                goto DO_ME_IN;
                            }
                        }
                    }
                }
            }
        DO_ME_IN:
            if (n->incurrent) {
                while (n->excess > 0.0) {
                    e = n->incurrent;
                    { /* PUSH IN */
#ifdef UNDIRECTED_GRAPH
                        double rf = e->cap + e->flow;
#else
                        double rf = e->flow;
#endif
                        node* n1 = e->ends[0];
                        if (n->flowlabel == n1->flowlabel + 1 && rf > 0.0) {
                            if (n->excess <= rf) {
                                e->flow -= n->excess;
                                n1->excess += n->excess;
                                n->excess = 0.0;
                                ADD_TO_ACTIVE(n1);
                            }
                            else {
                                e->flow -= rf;
                                n1->excess += rf;
                                n->excess -= rf;
                                ADD_TO_ACTIVE(n1);
                                n->incurrent = e->innext;
                                if (!n->incurrent) {
                                    n->incurrent = n->in;
                                    n->inout = GOING_OUT;
                                    RELABEL(n);
                                    break;
                                }
                            }
                        }
                        else {
                            n->incurrent = e->innext;
                            if (!n->incurrent) {
                                n->incurrent = n->in;
                                n->inout = GOING_OUT;
                                RELABEL(n);
                                break;
                            }
                        }
                    }
                }
            }
            else {
                /* n->in is NULL */
                n->inout = GOING_OUT;
                RELABEL(n);
            }
            if (n->excess > 0.0 && n->flowlabel < ncount) {
                ADD_TO_ACTIVE(n);
            }
        }

        return t->excess;
    }


    static void free_graph_aux(graph * G, bool aux)
    {
        CC_IFFREE(G->nodelist, node);
        CC_IFFREE(G->edgelist, edge);
#ifdef USE_GAP
        CC_IFFREE(G->level, node*);
#endif
#ifdef HIGHEST_LABEL_PRF
        CC_IFFREE(G->high, node*);
#endif
    }
    