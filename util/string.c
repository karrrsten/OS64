#include "string.h"

#include <stddef.h>

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
	char *restrict str1 = (char *restrict)s1;
	const char *restrict str2 = (const char *restrict)s2;
	for (size_t i = 0; i < n; ++i) {
		str1[i] = str2[i];
	}
	return s1;
}

void *memmove(void *s1, const void *s2, size_t n) {
	char *str1 = (char *)s1;
	const char *str2 = (const char *)s2;
	if (str1 < str2) {
		for (size_t i = 0; i < n; ++i) {
			str1[i] = str2[i];
		}
	} else {
		for (size_t i = n - 1; i >= 0; --i) {
			str1[i] = str2[i];
		}
	}
	return s1;
}

char *strcpy(char *restrict s1, const char *restrict s2) {
	for (size_t i = 0; s2[i] != 0; ++i) {
		s1[i] = s2[i];
	}
	return s1;
}

char *strncpy(char *restrict s1, const char *restrict s2, size_t n) {
	for (size_t i = 0; s2[i] != 0; ++i) {
		if (i >= n) {
			return s1;
		}
		s1[i] = s2[i];
	}
	return s1;
}

char *strcat(char *restrict s1, const char *restrict s2) {
	size_t i = 0;
	while (s1[i] != 0) {
		++i;
	}
	return strcpy(s1 + i, s2);
}

char *strncat(char *restrict s1, const char *restrict s2, size_t n) {
	size_t i = 0;
	while (s1[i] != 0) {
		++i;
	}
	return strncpy(s1 + i, s2, n);
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const char *str1 = (const char *)s1;
	const char *str2 = (const char *)s2;
	for (size_t i = 0; i < n; ++i) {
		if (str1[i] > str2[i]) {
			return 1;
		} else if (str1[i] < str2[i]) {
			return -1;
		}
	}
	return 0;
}

int strcmp(const char *s1, const char *s2) {
	size_t i = 0;
	for (; s1[i] != 0; ++i) {
		if (s1[i] != s2[i]) {
			break;
		}
	}

	if (s1[i] > s2[i]) {
		return 1;
	} else if (s1[i] < s2[i]) {
		return -1;
	} else {
		return 0;
	}
}

int strncmp(const char *s1, const char *s2, size_t n) {
	size_t i = 0;
	for (; s1[i] != 0; ++i) {
		if (s1[i] != s2[i]) {
			break;
		} else if (i >= n) {
			break;
		}
	}

	if (s1[i] > s2[i]) {
		return 1;
	} else if (s1[i] < s2[i]) {
		return -1;
	} else {
		return 0;
	}
}

void *memset(void *s, int c, size_t n) {
	char *str = (char *)s;
	for (size_t i = 0; i < n; ++i) {
		str[i] = (unsigned char)c;
	}
	return s;
}

volatile void *memset_volatile(volatile void *s, int c, size_t n) {
	volatile char *str = (volatile char *)s;
	for (size_t i = 0; i < n; ++i) {
		str[i] = (unsigned char)c;
	}
	return s;
}
