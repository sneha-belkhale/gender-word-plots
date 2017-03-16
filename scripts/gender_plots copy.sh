#!/bin/bash

DATA_DIR=../vectorbins
WORD2VEC_BIN_DIR=../bin
SRC_DIR=../src


##train or use existing data. if train:


TEXT_DATA=$DATA_DIR/text8
VECTOR_DATA=$DATA_DIR/text8-vector.bin

  echo -----------------------------------------------------------------------------------------------------
  echo -- Training vectors...
time $BIN_DIR/word2vec -train $TEXT_DATA -output $VECTOR_DATA -cbow 0 -size 200 -window 5 -negative 0 -hs 1 -sample 1e-3 -threads 12 -binary 1
  
##if already trained.. just use this

gcc SRC_DIR/gendered_word_plotting.c

./SRC_DIR/a.out $VECTOR_DATA

#plot
python word_plots.py
