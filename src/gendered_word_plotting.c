
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>  // Getopt for arguments parsing as well as stdin IO
#include <ctype.h>   // For isprint declaration

const long long max_size = 2000;         // max length of strings
const long long max_w = 50;              // max length of vocabulary entries
const long long max_to_project = 100;    // max length of words to project

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
    printf("    -k                    Word pair to map samples words. he-she is defailt\n");
    printf("    -i <FILE_PATH>        Input vectorbin file\n");
    printf("    -o <FILE_PATH>        Output score file\n");
    printf("    -w <FILE_PATH>        Sample words file\n");
    printf("\n");

    return 0;
}

int main(int argc, char **argv) {

  FILE *f;
  FILE *score_file;

  char *sample_words_file = "sample_words.txt";
  char *output_scores_file = NULL;
  char *input_vectorbin_file = "vectorbins/text8-vector.bin";
  char *vocab;
  const char vector_delim[2] = "-";

  // This is default values for two word in between
  // which we going to map our sample words
  char *from_word = "she";
  char *to_word = "he";

  float dist, he_she_vec[max_size];
  float dot, len, proj;  char ch;
  float *M;

  int arg_char;
  int sample_words_count = 0;

  long long words, size, a, b, c, d, cn, bi[100], gen_words_pos[100];
  long long she, he;

  bool output_to_file = false;
  bool use_sample_words_file = false;
  bool verbose = false;

  // Parse command line arguments
  // If no arguments were specified the program will run with default options
  while ((arg_char = getopt(argc, argv, "vho:i:w:k:")) != -1) {
    switch (arg_char) {
      case 'v':  // Verbose output
        verbose = true;
        break;
      case 'h':  // Print help and exit
        print_help();
        return 0;
      case 'i':  // Input vectorbin file
        input_vectorbin_file = optarg;
        break;
      case 'o':  // Output score file
        output_scores_file = optarg;
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
    printf("Mapping to %s-%s\n", from_word, to_word);
  }

  // Open word2vec trained vectorbin file
  f = fopen(input_vectorbin_file, "rb");

  if (f == NULL) {
    printf("Input vectorbin file not found\n");
    return -1;
  }

  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);

  // Reading the vector component per word into the array M
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));

  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }

  for (b = 0; b < words; b++) {
    fscanf(f, "%s%c", &vocab[b * max_w], &ch);
    for (a = 0; a < size; a++) {
      fread(&M[a + b * size], sizeof(float), 1, f);
    }
    len = 0;
    for (a = 0; a < size; a++) {
      len += M[a + b * size] * M[a + b * size];
    }
    len = sqrt(len);
    // Normalize the vector
    for (a = 0; a < size; a++) {
      M[a + b * size] /= len;
    }
  }
  fclose(f);

  char gen_words[max_to_project][max_size];
  int char_count = 0;
  FILE *sample_words_fd;

  if (use_sample_words_file) {
    sample_words_fd = fopen(sample_words_file, "rb");
  }

  // Reading in the words to project from stdin
  // or file if -w flag was specified
  // Words should come separated by space
  while (1) {
    // Fetch stdin or file stream char by char
    if (use_sample_words_file) {
      ch = getc(sample_words_fd);
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
      gen_words[sample_words_count][char_count] = ch;
      char_count += 1;
    }

    if (ch == EOF) {
      // This is the end of the word list
      if (use_sample_words_file) {
        fclose(sample_words_fd);
      }
      break;
    }
  }

  for (a=0; a < sample_words_count; a++) {

    // Find the word in the set
    for (b = 0; b < words; b++) {
      if (!strcmp(&vocab[b * max_w], gen_words[a])) {
        break;
      }
    }

    if (b == words) {
      b = 0;
    }

    gen_words_pos[a] = b;  // Save the words position in the vocabulary
  }

  // Find the she and he positions in the vector bin
  for (b = 0; b < words; b++) {
    if (!strcmp(&vocab[b * max_w], from_word)) {
      break;
    }
  }

  if (b == words) {
    b = 0;
  }

  she = b;  // She position

  for (b = 0; b < words; b++) {
    if (!strcmp(&vocab[b * max_w], to_word)) {
      break;
    }
  }

  if (b == words) {
    b = 0;
  }

  he = b;  // He position

  // Find the vector direction he->she in vec
  for (a = 0; a < size; a++) {
    he_she_vec[a] = M[a + she * size] - M[a + he * size];
  }

  if (output_scores_file) {
    // Seams like user provided a score output file. Use it
    score_file = fopen(output_scores_file, "wb");
    if (score_file == NULL) {
      printf("Can not create %s\n", output_scores_file);
      printf("Make sure that the path exists and you have sufficient permissions\n");
      return -1;
    }
    output_to_file = true;
  }

  // Project all words onto the she<->he axis
  for (int word = 0; word < sample_words_count; word++ ) {

    // Go through genwords and find their projection on she<->he vector
    if (gen_words_pos[word]!=0) {
      for (a = 0; a < size; a++) {
        dot += (he_she_vec[a] * M[a + gen_words_pos[word] * size]);  //a * b
      }
      for (a = 0; a < size; a++) {
        len += (he_she_vec[a] * he_she_vec[a]); //||b||
      }
      proj = dot / sqrt(len);

      if (output_to_file) {
        // Output result to the file
        fprintf(score_file, "%s %f\n", gen_words[word], dot);
      } else {
        // Output result to stdout
        printf("%s %f\n", gen_words[word] , dot);
      }

      // Reset all variables
      dot = 0;
      len = 0;
      proj = 0;
    }
  }

  if (output_to_file) {
    fclose(score_file);
  }

  return 0;
}
