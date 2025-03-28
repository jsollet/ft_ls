#include "../includes/sort.h"
#include "../includes/types.h"
#include "../libft/includes/libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

// liste des comparateurs

int compare_by_absolutePath(t_fileData *a, t_fileData *b) {
    return ft_strcmp(a->absolutePath, b->absolutePath);
}

int compare_by_fileName(t_fileData *a, t_fileData *b) {
    return ft_strcmp(a->fileName, b->fileName);
}

int compare_by_time(t_fileData *a, t_fileData *b) {
    // Comparer directement les time_t, version -t (sinon -1, 1,0)
    if (a->st_mtimes == b->st_mtimes){
        return (a->st_ino - b->st_ino);// a tester sur linux
    }
    if (a->st_mtimes < b->st_mtimes) return 1;
    else if (a->st_mtimes > b->st_mtimes) return -1;
    else return 0;
}

// reverse
int compare_by_absolutePath_reverse(t_fileData *a, t_fileData *b) {
    return ft_strcmp(b->absolutePath, a->absolutePath);
}

int compare_by_fileName_reverse(t_fileData *a, t_fileData *b) {
    return ft_strcmp(b->fileName, a->fileName);
}

int compare_by_time_reverse(t_fileData *a, t_fileData *b) {
    // Comparer directement les time_t, version -t 
    if (a->st_mtimes == b->st_mtimes){
        return (a->st_ino - b->st_ino);// a tester sur linux
    }
    if (a->st_mtimes < b->st_mtimes) return -1;
    else if (a->st_mtimes > b->st_mtimes) return 1;
    else return 0;
}

void merge_old(char *arr[], char *temp[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    
    while (i <= mid && j <= right) {
        if (ft_strcmp(arr[i], arr[j]) <= 0)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];
    
    for (i = left; i <= right; i++)
        arr[i] = temp[i];
}

void mergeSort_iterative_old(char *arr[], int n) {
    char **temp = malloc(n * sizeof(char *));
    if (!temp) exit(1);

    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);
            merge_old(arr, temp, left, mid, right);
        }
    }
    free(temp);
}

void merge(t_fileData *arr[], t_fileData *temp[], int left, int mid, int right, int (*cmp)(t_fileData *, t_fileData *)) {
    int i = left, j = mid + 1, k = left;
    
    while (i <= mid && j <= right) {
        if (cmp(arr[i], arr[j])<= 0)//(ft_strcmp(arr[i]->fileName, arr[j]->fileName) <= 0)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];
    
    for (i = left; i <= right; i++)
        arr[i] = temp[i];
}

void mergeSort_iterative(t_fileData *arr[], int n, int (*cmp)(t_fileData *, t_fileData *)) {
    t_fileData **temp = malloc(n * sizeof(t_fileData *));
    if (!temp) {
        ft_printf_fd(2,"Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) { // left <n -1 avant
            int mid = left + size - 1;
            if (mid >= n) break; // Empêcher mid d'aller au-delà de la taille du tableau
            int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);
            merge(arr, temp, left, mid, right, cmp);
        }
    }
    free(temp);
}