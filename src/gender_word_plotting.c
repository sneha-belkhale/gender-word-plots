#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>  // Getopt for arguments parsing as well as stdin IO
#include <ctype.h>   // For isprint declaration

const long long max_size = 2000;         // max number of vector componenets
const long long max_w = 50;              // max length of vocabulary entries
const long long max_to_project = 100;    // max length of words to project
long long words,size;       //global variables for dimensions of vector-bin


int print_help() {
  printf("\n");
  printf("DESCRIPTION\n");
  printf("\n");
  printf("    This program numericaly maps given words to a pair of predifined words.\n");
  printf("    This is useful to identify words relatinship and semantic biases.\n");
  printf("    This program accept space separated set of word and output scored\n\
         result to a file or stdout\n");
  
  printf("\n");
  
  printf("OPTIONAL ARGUMENTS\n");
  printf("\n");
  printf("    -v                    Print extra information\n");
  printf("    -k                    Word pair to map samples words. he-she is default\n");
  printf("    -g                    Generate all scores for unique words.\n");
  printf("    -i <FILE_PATH>        Input vectorbin file\n");
  printf("    -o <FILE_PATH>        Output score file\n");
  printf("    -w <FILE_PATH>        Sample words file\n");
  printf("\n");
  
  return 0;
}

int
generate_scores_for_unique_words(
  FILE *score_file,
  bool output_to_file,
  char* word_string_master,
  float* word_vector_master,
  float* he_she_vec) {
  //declare helper variables
  long long a;
  float dot;
  float* dots;
  dots = (float *)malloc(words * sizeof(float));
  //check memory available
  if (dots == NULL) {
    printf("Cannot allocate memory to store the scores: %lld\n", (long long)words * sizeof(float) / 1048576);
    return -1;
  }

  float minDot = 1.0;
  float maxDot = 0.0;
  printf("%lld %lld", words, size);
  
  // Iterate through all words in the bin
  for (int word = 0; word < words; word++ ) {
    //Project onto the she<->he axis
    for (a = 0; a < size; a++) {
      dot += (he_she_vec[a] * word_vector_master[a + word * size]);  //a * b
    }
    //update min an max values
    if (dot < minDot){
      minDot = dot;
    }
    if (dot > maxDot){
      maxDot = dot;
    }
    dots[word]=dot;

    // Reset dot product
    dot = 0;
  }
  
  float mapped_dot;
  for (int word = 0; word < words; word++ ){
    //mapping to range 0-1
    mapped_dot = (dots[word] - minDot) / (maxDot - minDot);
    if (output_to_file) {
      //output word and score to file
      fprintf(score_file, "%s %f\n", &word_string_master[word * max_w], mapped_dot);
    } else {
      // Output word and score to stdout
      printf("%s %f\n", &word_string_master[word * max_w], mapped_dot);
    }
  }
  return 0 ;
}

int
generate_scores_for_sample_words(
  FILE *score_file,
  bool output_to_file,
  float* word_vector_master,
  char* gen_words,
  long long* gen_words_pos,
  float *he_she_vec,
  int sample_words_count) {
  
  //declare helper variables
  long long a;
  float dot;

  // Iterate through all sample words
  for (int word = 0; word < sample_words_count; word++ ) {
    //Project onto the she<->he axis
    if (gen_words_pos[word]!=0) {
      for (a = 0; a < size; a++) {
        dot += (he_she_vec[a] * word_vector_master[a + gen_words_pos[word] * size]);  //a * b
      }
      
      if (output_to_file) {
        //output word and score to file
        fprintf(score_file, "%s %f\n", &gen_words[word * max_size], dot);
      } else {
        //Output word and score to stdout
        printf("%s %f\n", &gen_words[word * max_size], dot);
      }
      
      // Reset dot product
      dot = 0;
    }
  }

  return 0;

}

int
find_sample_words_in_vector_bin(
  char* word_string_master,
  char* gen_words,
  long long* gen_words_pos,
  int sample_words_count) {
  
  //declare helper variables
  long long a,b;
  
  for (a=0; a < sample_words_count; a++) {
    // Find the word in the set
    for (b = 0; b < words; b++) {
      if (!strcmp(&word_string_master[b * max_w], &gen_words[a*max_size])) {
        break;
      }
    }
    //If we have reached the end, the word was not found :(
    if (b == words) {
      b = 0;
    }
    // Save the words position in the sample word position container
    gen_words_pos[a] = b;
  }
  return 0;
}

int
get_he_she_axis(
  char* from_word,
  char* to_word,
  float* he_she_vec,
  char* word_string_master,
  float* word_vector_master) {

  //declare helper variables
  long long a, b, she, he;
  float dot;
  char ch;

  //Find from_word position in word_string_master
  for (b = 0; b < words; b++) {
    if (!strcmp(&word_string_master[b * max_w], from_word)) {
      break;
     }
  }
  //if we have reached the end, from_word was not found :(
  if (b == words) {
    b = 0;
  }
  //save position
  she = b;

  //Find to_word position in word_string_master
  for (b = 0; b < words; b++) {
    if (!strcmp(&word_string_master[b * max_w], to_word)) {
      break;
    }
  }
  //if we have reached the end, from_word was not found :(
  if (b == words) {
    b = 0;
  }
  //save position
  he = b;
  
  // Find the vector direction from_word->to_word
  for (a = 0; a < size; a++) {
    he_she_vec[a] = word_vector_master[a + she * size] - word_vector_master[a + he * size];
  }
  
  return 0;
} //he_she vector is now populated



int
get_size( FILE *f) {

  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);

  return 0;
  
}

int
load_vector_bin(
  FILE *f,
  char* word_string_master,
  float* word_vector_master) {
  //declare helper variables
  long long a, b;
  float len;
  char ch;
  
  //scan through each line
  for (b = 0; b < words; b++) {
    //save current word in word_string_master
    fscanf(f, "%s%c", &word_string_master[b * max_w], &ch);
    //save vector components in word_vector_master
    for (a = 0; a < size; a++) {
      fread(&word_vector_master[a + b * size], sizeof(float), 1, f);
    }
    len = 0;
    // Normalize the vector
    for (a = 0; a < size; a++) {
      len += word_vector_master[a + b * size] *word_vector_master[a + b * size];
    }
    len = sqrt(len);
    for (a = 0; a < size; a++) {
      word_vector_master[a + b * size] /= len;
    }
  }
  return 0;
} //word_string_master and word_vector_master are now populated


//***** MAIN PROCESS *****//

int main(int argc, char **argv) {
  
  char *from_word = "she";
  char *to_word = "he";
  float* he_she_vec;
  he_she_vec = (float *)malloc(max_size * sizeof(float));

  //container to hold the strings of each sample word to project
  char* gen_words;
  gen_words = (char *)malloc((long long)max_to_project * max_size * sizeof(char));
  //container to hold the positions in the vectorbin of each sample word to project
  long long gen_words_pos[max_to_project];

  /**
   stores the vector components for each word. 
   for example, to access the vector component <a> from word at position <b>:
        word_vector_master[a + b * size]
   **/
  float *word_vector_master;
  
  /**
   stores the word string.
   for example, to access the string at position <b>:
          &word_string_master[b * max_w]
   **/
  char *word_string_master;
  
  //task variables
  bool generate_all_scores = false;
  bool use_sample_words_file = false;
  bool output_words_to_file = false;
  bool verbose = false;
  
  
  //default inputs
  char *vectorbin_file = "vectorbins/text8-vector.bin";
  char *sample_words_file = "sample_words.txt";
  //default outputs
  char *word_scores_file = NULL;
  char *all_scores_file = "scores.txt";
  
  
  //parser variables
  const char vector_delim[2] = "-";
  char ch;
  FILE *f;
  int arg_char;
  int sample_words_count = 0;

  
  // Parse command line arguments
  // If no arguments were specified the program will run with default options
  while ((arg_char = getopt(argc, argv, "vho:i:w:k:g")) != -1) {
    switch (arg_char) {
      case 'g':  // generate scores for unique words, to file
        generate_all_scores = true;
        break;
      case 'v':  // Verbose output
        verbose = true;
        break;
      case 'h':  // Print help and exit
        print_help();
        return 0;
      case 'i':  // Input vectorbin file
        vectorbin_file = optarg;
        break;
      case 'o':  // Output score file
        output_words_to_file = true;
        word_scores_file = optarg;
        break;
      case 'w':  // Sample words file
        use_sample_words_file = true;
        sample_words_file = optarg;
        break;
      case 'k':  // Key pair for mapping
        from_word = strtok(optarg, vector_delim);
        // To search for the next token just pass NULL
        to_word = strtok(NULL, vector_delim);
        break;
      default:
        print_help();
        return 0;
    }
  }
  
  if (verbose) {
    printf("mapping to %s-%s\n", from_word, to_word);
  }
  

  // Open word2vec trained vectorbin file
  f = fopen(vectorbin_file, "rb");
  if (f == NULL) {
    printf("Input vectorbin file not found\n");
    return -1;
  } else {
    get_size(f);
    //allocate memory to vector bin containers based on the size and width of the vector bin
    word_string_master = (char *)malloc((long long)words * max_w * sizeof(char));
    word_vector_master = (float *)malloc((long long)words * (long long)size * sizeof(float));
    if (word_vector_master == NULL) {
      printf("Cannot allocate memory: %lld word_vector_master    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
      return -1;
    }
    //load the vector bin data into containers
    load_vector_bin(f, word_string_master, word_vector_master);
    fclose(f);
  }
  
  //get the axis to project , from_word <-> to_word
  get_he_she_axis(from_word, to_word, he_she_vec, word_string_master, word_vector_master);
  
  //if -g flag, generate score for every word in the vector bin, and write values to provided text file.
  if (generate_all_scores){
      f = fopen(word_scores_file, "wb");
      if (f == NULL) {
        printf("Could not find the file to ouptut all scores.. printing all scores to console \n");
        //print all scores to the console.
        generate_scores_for_unique_words(f, output_words_to_file , word_string_master, word_vector_master, he_she_vec);
      } else {
        //save every word score to the provided text file
        printf("Saving scores for all unique words in: %s \n", word_scores_file);
        generate_scores_for_unique_words(f, output_words_to_file ,word_string_master, word_vector_master, he_she_vec);
        fclose(f);
      }
    
  //if not generating scores for all unique words, use sample words or stdin
  } else {

  int char_count = 0;
  
  if (use_sample_words_file) {
    f = fopen(sample_words_file, "rb");
  }
  // Reading in the words to project from stdin
  // or file if -w flag was specified
  // Words should come separated by space
  while (1) {
    // Fetch file stream or stdin char by char
    if (use_sample_words_file) {
      ch = getc(f);
    } else {
      ch = getc(stdin);
    }
    
    if (ch == ' ' || ch == EOF) {
      // Got the full word
      char_count = 0;
      sample_words_count += 1;
    } else if (isalpha(ch)) {
      // For every alphabetic character
      // Add it to our gen_words array
      gen_words[sample_words_count*max_size + char_count] = ch;
      char_count += 1;
    }
    
    if (ch == EOF) {
      // This is the end of the word list
      if (use_sample_words_file) {
        fclose(f);
      }
      break;
    }
  }
  
  //find sample words in the vector bin and populate gen_words_pos
  find_sample_words_in_vector_bin(word_string_master, gen_words, gen_words_pos, sample_words_count);

  
  //Output the scores of each sample word, to the console or to specified file
  if (output_words_to_file) {
    // Seems like user provided a score output file. Use it.
    f = fopen(word_scores_file, "wb");
    if (f == NULL) {
      printf("Can not create %s\n", word_scores_file);
      printf("Make sure that the path exists and you have sufficient permissions\n");
    } else {
      printf("Printing sample word scores to file: %s\n", word_scores_file);
      generate_scores_for_sample_words(f, true, word_vector_master, gen_words, gen_words_pos, he_she_vec, sample_words_count);
      fclose(f);
    }
  } else {
    printf("Printing sample word scores to console instead.\n");
    generate_scores_for_sample_words(f, false, word_vector_master, gen_words, gen_words_pos, he_she_vec, sample_words_count);
  }
  
  
  
  }
  
  
  return 0;
  
}



