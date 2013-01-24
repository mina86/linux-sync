static ssize_t sys_read(unsigned fd, char __user * buf, size_t count)
{
	ssize_t ret = -EBADF;
	struct file *file;
	int fput_needed;

	file = fget_light(fd, &fput_needed);
	if (file) {
		loff_t pos = file->f_off;
		ret = vfs_read(file, buf, count, &pos);
		file->f_off = pos;
		fput_light(file, fput_needed);
	}

	return ret;
}
