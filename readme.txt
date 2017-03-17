{\rtf1\ansi\ansicpg1252\cocoartf1504
{\fonttbl\f0\fswiss\fcharset0 Helvetica;\f1\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;\red28\green0\blue207;}
{\*\expandedcolortbl;\csgray\c100000;\csgenericrgb\c0\c0\c0;\csgenericrgb\c11000\c0\c81000;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 This code is meant to work with the word2vec model\
(link to repository)\
\
Steps:\
\
1. train the model on a .txt file \
\
\pard\tx543\pardeftab543\pardirnatural\partightenfactor0

\f1\fs22 \cf2 \CocoaLigature0 TEXT_DATA=$DATA_DIR/text8     <-path to data directory \
VECTOR_DATA=$DATA_DIR/text8-vector.bin  <-path to where you want your library to be stored\
\
use the command:\

\f0\fs24 \cf0 \CocoaLigature1 \

\f1\fs22 \cf2 \CocoaLigature0 time $BIN_DIR/word2vec -train $TEXT_DATA -output $VECTOR_DATA -cbow \cf3 0\cf2  -size \cf3 200\cf2  -window \cf3 5\cf2  -negative \cf3 0\cf2  -hs \cf3 1\cf2  -sample \cf3 1e-3\cf2  -threads \cf3 12\cf2  -binary \cf3 1\
\

\f0\fs24 \cf0 \CocoaLigature1 \
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0
\cf0 \
2. run the gendered word plotter\
\
gcc gendered_word_plotting.c\
./a.out $VECTOR_DATA\
\
you should now be able to see the gendered words\
\
3. Generate a word plot of the scores and hate the world forever!\
python word_plots.py\
\
\
4. here is my example script for running on a }