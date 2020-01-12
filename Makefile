.PHONY: hw00.pdf

hw00.pdf:
	pandoc --from markdown --standalone --out hw00.pdf hw00.md
