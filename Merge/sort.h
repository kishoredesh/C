#ifndef SORT_H
#define SORT_H

typedef int (*Compare_fn)(const void *, const void *);
void MergeSort(void *arr, int num_elem, int elem_size, Compare_fn cmp);
void mt_mergesort(void *arr, int num_elem, int elem_size, Compare_fn cmp,int num_threads);
void InsertionSort(void *arr, int num_elem,int elem_size, Compare_fn cmp);
void mt_InsertionSort(void *arr, int num_elem, int elem_size,Compare_fn cmp,int num_threads);
void QuickSort(void *arr,int p, int r, Compare_fn cmp);
//void QuickSort(void *arr, int num_elem, int elem_size,int p, int r, Compare_fn cmp);
void mt_QuickSort(void *arr,int num_elem,int elem_size,int p, int r,Compare_fn cmp,int num_threads);

#endif
