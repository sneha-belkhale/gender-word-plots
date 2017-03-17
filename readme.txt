This code is meant to work with the word2vec model
(link to repository)

Steps:

1. train the model on a .txt file 

TEXT_DATA=$DATA_DIR/text8     <-path to data directory 
VECTOR_DATA=$DATA_DIR/text8-vector.bin  <-path to where you want your library to be stored

use the command:

time $BIN_DIR/word2vec -train $TEXT_DATA -output $VECTOR_DATA -cbow 0 -size 200 -window 5 -negative 0 -hs 1 -sample 1e-3 -threads 12 -binary 1



2. run the gendered word plotter

gcc gendered_word_plotting.c
./a.out $VECTOR_DATA

you should now be able to see the gendered words

3. Generate a word plot of the scores and hate the world forever!
python word_plots.py


4. here is my example script for running on a 