LATEX   = $${TEX:-latex} -file-line-error-style -halt-on-error


all: doc

doc: linux-sync.pdf

linux-sync.pdf: linux-sync.tex code/*.c
	-@exec cp -- linux-sync.toc linux-sync.toc.old >/dev/null 2>&1
	exec pdflatex -file-line-error-style -halt-on-error -interaction scrollmode $<
	for count in 1 2 3 4 5; do \
		if ! grep 'Rerun (LaTeX|to get cross-references right)' <linux-sync.log >/dev/null 2>&1 && \
		   cmp -- linux-sync.toc linux-sync.toc.old >/dev/null 2>&1; then break; fi ;\
		echo "Rerunning latex...." ;\
		cp -- linux-sync.toc linux-sync.toc.old >/dev/null 2>&1 ;\
		pdflatex -file-line-error-style -halt-on-error -interaction batchmode $< ;\
	done
	-@rm -- linux-sync.toc.old >/dev/null 2>&1

clean:
	exec rm -f -- linux-sync.aux linux-sync.dvi linux-sync.nav linux-sync.out linux-sync.snm linux-sync.toc linux-sync.toc.old linux-sync.vrb linux-sync.log

distclean: clean
	exec rm -f -- linux-sync.pdf linux-sync.ps

.PHONY: clean distclean
