#/bin/bash!

THIS=`basename $0`
HERE=$(pwd)
NOW=`date +%Y%m%D-%H%M%S`
SRC_DIR=../src
WORD2VEC_DIR=../../word2vec
BIN_DIR=$WORD2VEC_DIR/bin




red="\e[31m"
nor="\e[39m"

function usage() {
    printf "\nUsage: $THIS [-train PATH_TO_TXT_FILE] [-genPlot PATH_TO_BIN]\n\n"
    printf "${red}\nARGUMENTS\n${nor}"
    printf "\n    -train        Train the model on a specific data set\n"
    printf "\n    -genPlot        generate a plot from a vector reading bin\n"

    return 0
}

if [[ $# = 0 ]]; then
    usage
    exit 0
fi

# Parse Opts
#while the number of arguments is greater than

while [[ $# -gt 0 ]]; do
    #check signal against the following options
    case $1 in
        -train)
            if [ -z "$2" ]; then
                echo "Must specify a text file to train your model"
                shift 1
            else
                echo "Training on data set $2"
                TEXT_DATA=$2
                VECTOR_DATA="${TEXT_DATA%%.txt}"-vector.bin
                TASK=1
                shift 2
            fi
            ;;
        -genPlot)
            GENMAP=1
            if [ -z "$2" ]; then
                echo "Must specify the vector bin that you are referencing for your plot"
                echo "Plotting using default"
                VECTOR_DATA=../vectorbins/text8-vector.bin
                shift 1
            else
                VECTOR_DATA=$2
                shift 2
            fi
            ;;
        *)
            usage
            shift 1
            ;;
    esac
done



if [ -z "$TASK" ]; then
    echo
else
    #check if word2vec model already exists in the folder above. clone it if it does not exist
    if [ -d $WORD2VEC_DIR ]; then
        echo "Word2vec already cloned. cool"
    else
        git clone https://github.com/dav/word2vec.git $WORD2VEC_DIR
    fi
    pushd $WORD2VEC_DIR/src && make; popd
    time $BIN_DIR/word2vec -train $TEXT_DATA -output $VECTOR_DATA -cbow 0 -size 200 -window 5 -negative 0 -hs 1 -sample 1e-3 -threads 12 -binary 1
fi


if [ -z "$GENMAP"  ]; then
    echo
else
    gcc $SRC_DIR/gendered_word_plotting.c

    ./$SRC_DIR/a.out $VECTOR_DATA

    python $SRC_DIR/word_plots.py

fi




