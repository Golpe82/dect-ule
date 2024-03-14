# SPDX-License-Identifier: MIT
all: dist/fwtool

dist/fwtool:
	pyinstaller --onefile fwtool.py

doc: README.pdf

README.pdf: README.adoc Makefile
	bundle exec asciidoctor-pdf \
		-a pdf-stylesdir=../doc/_resources/themes \
		-a pdf-style=dspg \
		-a pdf-fontsdir=../doc/_resources/fonts \
		README.adoc

.phony: dist/fwtool
