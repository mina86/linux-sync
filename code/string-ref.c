basic_string assign(const basic_string &__str) {
	_CharT *__tmp = __str._M_rep()->_M_grab();
	_M_rep()->_M_dispose();
	_M_dataplus._M_p = __tmp;
	return *this;
}

_CharT *_M_grab() {
	if (_M_refcount >= 0) {
		__sync_fetch_and_add(&_M_refcount, 1);
		return _M_refdata();
	} else {
		_Rep *__r = _Rep::_S_create(_M_length, _M_capacity);
		_M_copy(__r->_M_refdata(), _M_refdata(), _M_length);
		_M_refcount = 0;
		return __r->_M_refdata();
	}
}

void _M_dispose() {
	if (__sync_fetch_and_add(&_M_refcount, -1) <= 0) _M_destroy();
}

void _M_leak() {
	if (_M_rep()->_M_refcount > 0) _M_mutate(0, 0, 0);
	_M_rep()->_M_refcount = -1;
}
