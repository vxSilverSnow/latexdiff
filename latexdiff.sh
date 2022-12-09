#!/bin/zsh

file=${1##*/}
dir=${1%/*}
name=${file%.*}

source "${0%/*}/config/config"
cd "${dir}"
if [ "${CFONT}" = "true" ]
then
    latexdiff-vc --git --flatten -t CFONT -r ${DIFFHASH} ${file}
else
    latexdiff-vc --git --flatten -r ${DIFFHASH} ${file}
fi

"${0%/*}/bin/rewrite_diff.out" ${name} ${DIFFHASH} ${REMOVETEXROOT} ${BIB} ${CFONT} ${STYLE} ${FADD} ${FDEL} ${HEAD}

rm ${name}-diff${DIFFHASH}.tex

${TEX} temp.tex
${TEX} temp.tex
${DVIPDF} temp.dvi

[ -e diff.tex ] && rm diff.tex
[ -e diff.pdf ] && rm diff.pdf

mv temp.tex diff.tex
mv temp.pdf diff.pdf

rm temp*
read -s -k 1
