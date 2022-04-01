#!/bin/bash

pdfFont="Hack:pixelsize=15:antialias=true:autohint=true:style=Regular"

input="cursed.md"
pdfHeaderInput="header.pdf"
indexedMdOut="$(mktemp).md"
imaginedMdOut="../docs/index.md"
htmlOut="../docs/index.html"
pdfOut="$(mktemp).pdf"
pdfHeaderedOut="../docs/cursed.pdf"
umlDir="uml"
imgDirOut="../docs/img"

function indexGen() {
    out=$(mktemp)
    index=$(mktemp)

    cp "$1" "$out"

    tab='\t'
    i=(-1 1)
    prevLen=0
    while read -r line; do
        hash="$(md5sum <<< "$line" | cut -d ' ' -f 1)"
        printf "<a id=\"%s\"/>\n\n%s\n\n" "$hash" "$line" > tmp
        sed "/$line/ {
            x
            r tmp
        }" "$out" > "${out}.tmp"
        mv "${out}.tmp" "$out"
        hdrLen=$(awk -F'#' '{print NF-1}' <<< "$line")
        hdrTxt="${line//#/}"
        (( hdrLen > 1 )) && for (( j=1; j<hdrLen; j++ )); do echo -en "$tab"; done
        (( prevLen < hdrLen )) && i[$hdrLen]=1
        printf "%d. [%s ](#%s)\n" ${i[$hdrLen]} "$hdrTxt" "$hash"
        prevLen=$hdrLen
        let i[$hdrLen]++
    done <<< "$(grep --color=no -E "^#+ " "$1")" > $index

    cat "$out" > "$out".tmp
    printf "# Содержание\n" > "$out"
    cat $index >> "$out"
    cat "$out".tmp >> "$out"

    rm $index
    rm tmp
    rm "$out".tmp

    mv "$out" "$2"
}

# function imaginizeGen() {
#     out="$(basename $imgDirOut)"
#     for img in "$imgDirOut"/*; do
#         search=${img/.svg/}
#         sed "/\*[*\]($search)/\![]"
#     done
# }

function umlGen() {
    plantuml -tsvg "$1"/*
    mv "$1"/*.svg "$2"/
}

function htmlGen() {
    pandoc -f markdown -s "$1" -o "$2"
}

function pdfGen() {
    pandoc --pdf-engine xelatex -V mainfont="$1" -f markdown "$2" -o "$3"
}

function pdfHeaderedGen() {
    pdftk "$1" "$2" cat output "$3"
}

function umlf() { [[ -d "$imgDirOut" ]] || mkdir -p "$imgDirOut"; umlGen "$umlDir" "$imgDirOut";}
# function mdf() { indexGen "$input" "$indexedMdOut"; imaginizeGen "$indexedMdOut" "$imaginedMdOut" ;}
function mdf() { indexGen "$input" "$imaginedMdOut" ;}
# function htmlf() { htmlGen "$indexedMdOut" "$htmlOut" ;}
function pdff() { pdfGen "$pdfFont" "$imaginedMdOut" "$pdfOut"; pdfHeaderedGen "$pdfHeaderInput" "$pdfOut" "$pdfHeaderedOut" ;}

case $1 in
    uml)  umlf ;;
    md)   mdf ;;
    # html) htmlf ;;
    pdf)  pdff ;;
    *)
        umlf
        mdf
        # htmlf
        pdff
        ;;
esac

rm "$indexedMdOut"
rm "$pdfOut"
