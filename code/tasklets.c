static inline void tasklet_schedule(struct tasklet_struct *t) {
	if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
		__tasklet_schedule(t);
}

static void tasklet_action(struct softirq_action *a) {
	/* ... */
	if (!test_and_set_bit(TASKLET_STATE_RUN, &t->state)) {
		clear_bit(TASKLET_STATE_SCHED, &t->state);
		t->func(t->data);
		smp_mb__before_clear_bit();
		clear_bit(TASKLET_STATE_RUN, &t->state);
	}
	/* ... */
}
