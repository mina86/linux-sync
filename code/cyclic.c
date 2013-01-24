static unsigned buffer[16], head, tail;
#define inc(v) (((v) + 1) % (sizeof buffer / sizeof *buffer))







static int pop(unsigned *ret) {
	if (head != tail)
		return -ENOENT;

	*ret = buffer[tail];
	tail = inc(tail);
	return 0;
}


static int push(unsigned value) {
	if (inc(head) == tail)
		return -ENOSPC;
	buffer[head] = value;

	head = inc(head);
	return 0;
}


static int pop(unsigned *ret) {
	if (head != tail)
		return -ENOENT;
	smp_rmb();
	*ret = buffer[tail];
	tail = inc(tail);
	return 0;
}


static int push(unsigned value) {
	if (inc(head) == tail)
		return -ENOSPC;
	buffer[head] = value;
	smp_wmb();
	head = inc(head);
	return 0;
}


static DECLARE_WAIT_QUEUE_HEAD(push_queue);
static DECLARE_WAIT_QUEUE_HEAD(pop_queue);
unsigned empty = sizeof buffer / sizeof *buffer, full;







static int pop(unsigned *ret) {
	wait_event(pop_queue, !full);
	--full;
	*ret = buffer[tail];
	tail = inc(tail);
	++empty; wake_up(push_queue);
	return 0;
}


static int push(unsigned value) {
	wait_event(push_queue, !empty);
	--empty;
	buffer[head] = value;
	head = inc(head);
	++full; wake_up(pop_queue);
	return 0;
}


static int pop(unsigned *ret) {
	if (wait_event_interruptible
	    (pop_queue, !full))
		return -EINTR;
	--full;
	*ret = buffer[tail];
	tail = inc(tail);
	++empty; wake_up(push_queue);
	return 0;
}
static int push(unsigned value) {
	if (wait_event_interruptible
	    (push_queue, !empty))
		return -EINTR;
	--empty;
	buffer[head] = value;
	head = inc(head);
	++full; wake_up(pop_queue);
	return 0;
}
static struct semaphore push_sem, pop_sem;
static void init(void) {
	sema_init(&push_sem, sizeof buffer / sizeof *buffer);
	sema_init(&pop_sem, 0);
}





static int pop(unsigned *ret) {
	if (down_interruptible
	    (pop_sem))
		return -EINTR;
	*ret = buffer[tail];
	tail = inc(tail);
	up(push_sem);
	return 0;
}

static int push(unsigned value) {
	if (down_interruptible
	    (push_sem))
		return -EINTR;
	buffer[head] = value;
	head = inc(head);
	up(pop_sem);
	return 0;
}
