
#include "../include/mfset.h"
#include <stdio.h>
#include <stdlib.h>

MFSet *init_mfset(const uint32_t n) {
  MFSet *mfset = (MFSet *)malloc(sizeof(MFSet));
  if (mfset == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for Disjoint Set\n");
    exit(1);
  }

  mfset->n = n;
  mfset->parent = (uint32_t *)malloc(n * sizeof(uint32_t));
  mfset->rank = (uint32_t *)malloc(n * sizeof(uint32_t));

  if (mfset->parent == NULL || mfset->rank == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for Disjoint Set\n");
    exit(1);
  }

  for (uint32_t i = 0; i < n; i++) {
    mfset->parent[i] = i;
    mfset->rank[i] = 0;
  }

  return mfset;
}

void free_mfset(MFSet *mfset) {
  free(mfset->parent);
  free(mfset->rank);
  free(mfset);
}

uint32_t find(MFSet *mfset, const uint32_t x) {
  if (mfset->parent[x] == x) {
    return x;
  } else {
    mfset->parent[x] = find(mfset, mfset->parent[x]);
    return mfset->parent[x];
  }
}

void unite(MFSet *mfset, const uint32_t x, const uint32_t y) {
  uint32_t x_root = find(mfset, x);
  uint32_t y_root = find(mfset, y);

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
