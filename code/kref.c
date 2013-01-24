struct foo {
	struct kref ref;
	/* ... */
};

struct foo *new_foo(/* ... */) {
	struct foo *foo =
		kmalloc(sizeof *foo, GFP_KERNEL);
	kref_init(&foo->ref);
	/* ... */
	return foo;
}

void foo_get(struct foo *foo) {
	kref_get(&foo->ref);
}

void foo_release(struct kref *ref) {
	struct foo *foo =
		container_of(ref, struct foo, ref);
	/* ... */
	kfree(foo);
}

void foo_put(struct foo *foo) {
	kref_put(&foo->ref, foo_release);
}
