## Word2vec addon

This repo is an add-on to to the word2vec module. By training a word2vec model, we create a text embedding that is an N-dimensional vector space containing relative positions of words in the text. By projecting words onto a she<->he axis in this N-dimensional vector space, we can see whether the word is more closely associated with she, or he. Projecting supposedly gender neutral words such as genius, intelligent, or bossy, should hopefully be right in the middle of the she<->he axis. However, we see that this is seldom the case, revealing the gender biases propagated through the original text and into the text embedding.

In summary, this experiment uses the word2vec module to shine light on the issue that the media commonly associates toxic words with women. We consume this media every day, and are therefore subliminally consuming these biases every day. Much of our community believes that feminism isn’t relevant anymore as women and men have “equal rights”. Hopefully this scientific evidence will be concrete proof of the disparities that exist in the way we perceive gender, and that we still have a long way to go.   

The repo already contains a default vector-bin in the vector-bin folder (trained from text8 wikipedia data set). All demos will be run with this vector-bin unless specified.

However, the purpose of this repository is to be able to take any desired text file, train a word2vec model, and determine gender biases present in the text. I have tested the model on data sets from reddit, google news, the Enron emails, (links to sets below), and observed some extremely shocking word plots and wanted to bring awareness to this issue.

Steps:

1. train the model on a .txt file 

```
./gender_plots.sh —train PATH_TO_TXT_FILE
```

2. Generate a word plot of the scores and hate the world forever! Just kidding. Be inspired to make change.

./gender_plots.sh —genplot PATH_TO_BIN

(The bin that you just created should be stored in the vector bin file)



Note. If you would like to modify the sample set of words that are getting projected onto the he-she axis, you can edit the sample_words.txt file in the src folder.



Here is where you can find links to more interesting data sets (including Donald Trump comments, Religious texts (lol), Reddit posts, etc. 

https://www.kaggle.com/datasets?sortBy=relevance&group=featured&search=text

https://www.springboard.com/blog/free-public-data-sets-data-science-project/


..

#femaleisfuture

