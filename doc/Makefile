
files=*.ronn.md

${files:ronn.md=} ${files:ronn.md=html}: ${files}
	ronn --manual="libhey manual" --organization="Per Johansson" ${files}
