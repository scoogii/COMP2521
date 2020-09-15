// Lecture 01b
// Runtimes

// O(n^2)
n = 10;
for (int i = 0; i < n; i++) { <--- O(n)
	for (int j = 0; j <= i; j++) { <--- O(n^2) [1 + 2 + 3 + 4 + 5 + ... + (n-1)]
		printf(i, j); <--- O(n^2)
	}
}

// O(n)
n = 10;
for (int i = 0; i < n; i++) { <--- O(n)
	for (int j = 0; j <= 10; j++) { <--- O(n)
		printf(i, j); <--- O(n)
	}
}
