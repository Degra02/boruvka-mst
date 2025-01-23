
#include "../include/mfset.h"
#include <stdio.h>
#include <stdlib.h>

MFSet* init_mfset(const int n) {
  MFSet* mfset = (MFSet*)malloc(sizeof(MFSet));
  if (mfset == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for Disjoint Set\n");
    exit(1);
  }

  mfset->n = n;
  mfset->parent = (int*)malloc(n * sizeof(int));
  mfset->rank = (int*)malloc(n * sizeof(int));

  if (mfset->parent == NULL || mfset->rank == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for Disjoint Set\n");
    exit(1);
  }

  for (int i = 0; i < n; i++) {
    mfset->parent[i] = i;
    mfset->rank[i] = 0;
  }

  return mfset;
}

void free_mfset(MFSet* mfset) {
  free(mfset->parent);
  free(mfset->rank);
  free(mfset);
}

int find(MFSet *mfset, const int x) {
  if (mfset->parent[x] == x) {
    return x;
  } else {
    mfset->parent[x] = find(mfset, mfset->parent[x]);
    return mfset->parent[x];
  }
}

void unite(MFSet *mfset, const int x, const int y) {
  int x_root = find(mfset, x);
  int y_root = find(mfset, y);

  if (x_root == y_root) {
    return;
  }

  if (mfset->rank[x_root] < mfset->rank[y_root]) {
    mfset->parent[x_root] = y_root;
  } else if (mfset->rank[x_root] > mfset->rank[y_root]) {
    mfset->parent[y_root] = x_root;
  } else {
    mfset->parent[y_root] = x_root;
    mfset->rank[x_root]++;
  }
}
