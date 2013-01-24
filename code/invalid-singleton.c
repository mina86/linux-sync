struct foo *foo_singleton() {
	static struct mutex mutex;
	static struct foo *foo;
	if (!foo) {
		mutex_lock(&mutex);
		if (!foo) {
			struct foo *f = malloc(sizeof *f);
			init_foo(f);
			foo = f
		}
		mutex_unlock(&mutex);
	}
	return foo;
}
