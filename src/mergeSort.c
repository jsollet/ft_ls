#include "../libft/includes/libft.h"
#include "../includes/sort.h"
#include "../includes/types.h"
// nouvelle version
time_t get_mtime(t_fileData *f) {
	return f->st_mtimes;
}

time_t get_atime(t_fileData *f) {
	return f->st_atimes;
}

int 	cmp_mtime(t_fileData *a, t_fileData *b) {
	return compare_by_time_generic(a, b, get_mtime);
}

int		cmp_atime(t_fileData *a, t_fileData *b) {
	return compare_by_time_generic(a, b, get_atime);
}

int		cmp_mtime_rev(t_fileData *a, t_fileData *b) {
	return compare_by_time_generic_reverse(a, b, get_mtime);
}

int		cmp_atime_rev(t_fileData *a, t_fileData *b) {
	return compare_by_time_generic_reverse(a, b, get_atime);
}
// foireux
int compare_by_time_generic1(t_fileData *a, t_fileData *b, get_time_func get_time) {
	time_t ta = get_time(a);
	time_t tb = get_time(b);

	if (ta == tb)
		#ifdef __APPLE__
			return ft_strcmp(a->fileName, b->fileName);
		#else
			return (a->st_ino - b->st_ino);
		#endif
	if (ta < tb)
		return 1;
	else
		return -1;
}

int compare_by_time_generic(t_fileData *a, t_fileData *b, get_time_func get_time) {
    time_t ta = get_time(a);
    time_t tb = get_time(b);

    if (ta < tb)
        return -1;  // a vient avant b (plus ancien)
    else if (ta > tb)
        return 1;   // a vient après b (plus récent)
    else
        //#ifdef __APPLE__
          //  return ft_strcmp(a->fileName, b->fileName);  // Si les dates sont égales, trie lexicographique
        //#else
            return (a->st_ino - b->st_ino);  // Si les dates sont égales, trie par inode
        //#endif
}

int compare_by_time_generic_reverse(t_fileData *a, t_fileData *b, get_time_func get_time) {
	time_t ta = get_time(a);
	time_t tb = get_time(b);

	if (ta == tb)
		#ifdef __APPLE__
			return ft_strcmp(a->fileName, b->fileName);
		#else
			return (a->st_ino - b->st_ino);
		#endif
	if (ta < tb)
		return -1;
	else
		return 1;
}




// ancienne version
int	compare_by_absolutePath(t_fileData *a, t_fileData *b) {
	return ft_strcmp(a->absolutePath, b->absolutePath);
}

int	compare_by_fileName(t_fileData *a, t_fileData *b) {
	return ft_strcmp(a->fileName, b->fileName);
}

int	compare_by_time(t_fileData *a, t_fileData *b) {
	// Comparer directement les time_t, version -t (sinon -1, 1,0)
	if (a->st_mtimes == b->st_mtimes){
		return (a->st_ino - b->st_ino);
	}
	if (a->st_mtimes < b->st_mtimes) return 1;
	else if (a->st_mtimes > b->st_mtimes) return -1;
	else return 0;
}

int	compare_by_time_u(t_fileData *a, t_fileData *b) {
	// Comparer directement les time_t, version -t (sinon -1, 1,0)
	if (a->st_atimes == b->st_atimes){
		return (a->st_ino - b->st_ino);
	}
	if (a->st_atimes < b->st_atimes) return 1;
	else if (a->st_atimes > b->st_atimes) return -1;
	else return 0;
}

int	compare_by_absolutePath_reverse(t_fileData *a, t_fileData *b) {
	return ft_strcmp(b->absolutePath, a->absolutePath);
}

int compare_by_fileName_reverse(t_fileData *a, t_fileData *b) {
	return ft_strcmp(b->fileName, a->fileName);
}

int compare_by_time_reverse(t_fileData *a, t_fileData *b) {
	// Comparer directement les time_t, version -t 
	if (a->st_mtimes == b->st_mtimes){
		return (a->st_ino - b->st_ino);
	}
	if (a->st_mtimes < b->st_mtimes) return -1;
	else if (a->st_mtimes > b->st_mtimes) return 1;
	else return 0;
}

int compare_by_time_reverse_u(t_fileData *a, t_fileData *b) {
	// Comparer directement les time_t, version -t 
	if (a->st_atimes == b->st_atimes){
		return (a->st_ino - b->st_ino);
	}
	if (a->st_atimes < b->st_atimes) return -1;
	else if (a->st_atimes > b->st_atimes) return 1;
	else return 0;
}

void	merge(t_fileData *arr[], t_fileData *temp[], int left, int mid, int right, int (*cmp)(t_fileData *, t_fileData *)) {
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

void	mergeSort_iterative(t_fileData *arr[], int n, int (*cmp)(t_fileData *, t_fileData *)) {

	t_fileData **temp = malloc(n * sizeof(t_fileData *));
	if (!temp) {
		ft_printf_fd(2,"Erreur d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}
	for (int size = 1; size < n; size *= 2) {
		for (int left = 0; left < n; left += 2 * size) {
			int mid = left + size - 1;
			if (mid >= n) break;
			int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);
			merge(arr, temp, left, mid, right, cmp);
		}
	}
	free(temp);
}
