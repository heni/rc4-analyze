#!/usr/bin/env bash

for ((i=25;i<=32;++i)); do OUT=$(./maxpos-decryptor tests/02/encrypted.unigrams-$i.bin | ./colorize-answer.py --hide-scores --output-html); echo "$i:	<span style=\"color:red\">$OUT</span>"; done > maxpos-log.html 
for ((i=25;i<=32;++i)); do OUT=$(./unigrams-decryptor tests/02/encrypted.unigrams-$i.bin | ./colorize-answer.py --hide-scores --output-html); echo "$i:	<span style=\"color:red\">$OUT</span>"; done > unigrs-log.html
for ((i=25;i<=32;++i)); do OUT=$(./bigrams-decryptor tests/02/encrypted.bigrams-$i.bin | ./colorize-answer.py --hide-scores --output-html); echo "$i:	<span style=\"color:red\">$OUT</span>"; done > bigrms-log.html
