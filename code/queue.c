








static DEFINE_MUTEX(mutex), LIST_HEAD(first);

static int push(struct list_head *n) {
	if (mutex_lock_interruptible(&mutex)) return -EINTR;
	list_add_tail(&first, n);
	mutex_unlock(&mutex);
	return 0;
}

static struct list_head *pop(void) {
	struct list_head *ret = NULL;
	if (mutex_lock_interruptible(&mutex)) return PTR_ERR(-EINTR);
	if (!list_empty(&first)) {
		ret = first.next;
		list_del(ret);
	}
	mutex_unlock(&mutex);
	return ret;
}

static DEFINE_SPINLOCK(lock), LIST_HEAD(first);

static void push(struct list_head *n) {
	spin_lock(&lock);
	list_add_tail(&first, n);
	spin_unlock(&lock);
}

static struct list_head *pop(void) {
	struct list_head *ret = NULL;
	spin_lock(lock);
	if (!list_empty(&first)) {
		ret = first.next;
		list_del(ret);
	}
	spin_unlock(&lock);
	return ret;
}


static void push(struct list_head *n) {
	unsigned long flags;
	spin_lock_irqsave(&lock, flags);
	list_add_tail(&first, n);
	spin_unlock_irqrestore(&lock, flags);
}

static struct list_head *pop(void) {
	struct list_head *ret = NULL;
	unsigned long flags;
	spin_lock_irqsave(&lock, flags);
	if (!list_empty(&first)) {
		ret = first.next;
		list_del(ret);
	}
	spin_unlock_irqrestore(&lock, flags);
	return ret;
}
