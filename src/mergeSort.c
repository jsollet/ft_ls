#include "../libft/includes/libft.h"
#include "../includes/sort.h"
#include "../includes/types.h"

int	(*get_cmp_func(t_flags *flags))(t_fileData *, t_fileData *) {
	if (flags->t && !flags->r){
		return compare_by_time;}
	else if (flags->t && flags->r)
		return compare_by_time_reverse;
	else if (!flags->t && !flags->r)
		return compare_by_fileName;
	else
		return compare_by_fileName_reverse;
}


int	compare_by_fileName(t_fileData *a, t_fileData *b) {
	return ft_strcmp(a->fileName, b->fileName);
}


int compare_by_time(t_fileData *a, t_fileData *b) {
	if (a->st_mtimes > b->st_mtimes)
		return -1;
	else if (a->st_mtimes < b->st_mtimes)
		return 1;
	if (a->st_mtime_nsec > b->st_mtime_nsec)
		return -1;
	else if (a->st_mtime_nsec < b->st_mtime_nsec)
		return 1;
	return ft_strcmp(a->fileName, b->fileName);
}


int compare_by_fileName_reverse(t_fileData *a, t_fileData *b) {
	return ft_strcmp(b->fileName, a->fileName);
}


int compare_by_time_reverse(t_fileData *a, t_fileData *b) {
	if (a->st_mtimes < b->st_mtimes)
		return -1;
	else if (a->st_mtimes > b->st_mtimes)
		return 1;
	if (a->st_mtime_nsec < b->st_mtime_nsec)
		return -1;
	else if (a->st_mtime_nsec > b->st_mtime_nsec)
		return 1;
	return ft_strcmp(a->fileName, b->fileName);
}

static void insertion_sort(t_fileData *arr[], int left, int right, int (*cmp)(t_fileData *, t_fileData *)) {
	for (int i = left + 1; i <= right; i++) {
		t_fileData *key = arr[i];
		int j = i - 1;
		while (j >= left && cmp(arr[j], key) > 0) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

void	merge(t_fileData *arr[], t_fileData *temp[], int left, int mid, int right, int (*cmp)(t_fileData *, t_fileData *)) {
	int i = left, j = mid + 1, k = left;
	
	while (i <= mid && j <= right) {
		if (cmp(arr[i], arr[j])<= 0)
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
	const int INSERTION_THRESHOLD = 24;

	t_fileData **temp = malloc(n * sizeof(t_fileData *));
	if (!temp) {
		ft_printf_fd(2,"Erreur d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i += INSERTION_THRESHOLD) {
		int right = (i + INSERTION_THRESHOLD - 1 < n - 1) ? (i + INSERTION_THRESHOLD - 1) : (n - 1);
		insertion_sort(arr, i, right, cmp);
	}
	
	for (int size = INSERTION_THRESHOLD; size < n; size *= 2) {
		for (int left = 0; left < n; left += 2 * size) {
			int mid = left + size - 1;
			if (mid >= n) continue;
			int right = (left + 2 * size - 1 < n - 1) ? (left + 2 * size - 1) : (n - 1);
			merge(arr, temp, left, mid, right, cmp);
		}
	}
	free(temp);
}