static void
push(struct node **firstp,
     struct node *n) {
	struct node *f;
	do {
		f = *firstp;
		n->next = f;
	} while (cmpxchg(firstp, f, n) != f);
}

static struct node *
pop(struct node **firstp) {
	struct node *f;
	do {
		f = *first;
	} while (f && cmpxchg(firstp, f, f->next) != f);
	return f;
}
