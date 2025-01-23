
#ifndef MFSET_H
#define MFSET_H


/*
 * Disjoint Set (Union-Find) data structure.
 * @attr `n`: number of elements
 * @attr `parent`: array of size `n` mapping each element to its parent
 * @attr `rank`: array of size `n` mapping each element to its rank
*/
typedef struct MFSet {
  int n;
  int *parent;
  int *rank;
} MFSet;

/*
 * Initializes a Disjoint Set with `n` elements.
 * @param `n`: number of elements
 * @return `MFSet*`: pointer to the Disjoint Set
*/
MFSet* init_mfset(const int);

/*
 * Frees the memory allocated for the Disjoint Set.
 * @param `mfset`: pointer to the Disjoint Set
*/
void free_mfset(MFSet*);

/*
 * Finds the representative of the set containing `x`.
 * Performs path compression.
 * @param `mfset`: pointer to the Disjoint Set
 * @param `x`: element
 * @return `int`: representative of the set containing `x`
*/
int find(MFSet*, const int);

/*
 * Unites the sets with parents `x` and `y`.
 * @param `mfset`: pointer to the Disjoint Set
 * @param `x`: element
 * @param `y`: element
*/
void unite(MFSet*, const int, const int);

#endif // !MFSET_H
