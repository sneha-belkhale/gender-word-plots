
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


const long long max_size = 2000;         // max length of strings
const long long max_w = 50;              // max length of vocabulary entries
const long long max_to_project = 100;     // max length of words to project


int main(int argc, char **argv) {
  FILE *f;
  char file_name[max_size], st[100][max_size];
  float dist, he_she_vec[max_size];
  long long words, size, a, b, c, d, cn, bi[100], gen_words_pos[100];
  float dot, len, proj;  char ch;
  float *M;
  char *vocab;
    
  //reading the vector component per word into the array M
  if (argc < 2) {
    printf("Usage: ./word-analogy <FILE>\nwhere FILE contains word projections in the BINARY FORMAT\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input binary file not found\n");
    return -1;
  }
  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));
  M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    fscanf(f, "%s%c", &vocab[b * max_w], &ch);
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    //normalize the vector
    for (a = 0; a < size; a++) M[a + b * size] /= len;
  }
  fclose(f);
    
  //reading in the words to project from sample_words.txt
  int count = 0;
  char gen_words[max_to_project][max_size];
  char str[max_w];
  f = fopen("sample_words.txt", "rb");
  if (f==NULL) {
    printf("Cannot find sample_words.txt in the src folder D:\n");
  } else {
    while (fscanf(f, "%s", str)!=EOF && count < max_to_project){
      for (int i=0; i < sizeof str-1; i++) gen_words[count][i]=str[i];
      count = count+1;
  }
  fclose(f);
  }
  
  for (a=0; a < count; a++){
    //find the word in the set
    for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], gen_words[a])) break;
    if (b == words) b = 0;
      gen_words_pos[a] = b; //save the words position in the vocabulary
      
  }
  
  //find the she and he positions in the vector bin
  long long she, he;
  char she_char[max_size] = "she";
  char he_char[max_size] = "he";
  
  
  for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], she_char)) break;
  if (b == words) b = 0;
  she = b; //she position
  
  for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], he_char)) break;
  if (b == words) b = 0;
  he = b;  //he position
    

  //find the vector direction he->she in vec
  for (a = 0; a < size; a++) he_she_vec[a] = M[a + she * size] - M[a + he * size];


  FILE *score_file = fopen("scores.txt","wb");
  //project all words onto the she<->he axis
  for (int word = 0; word<129; word ++ ){
  //go through genwords and find their projection on she -. he
    if (gen_words_pos[word]!=0){
      for (a = 0; a < size; a++) dot += (he_she_vec[a] * M[a + gen_words_pos[word] * size]); //a * b
      for (a = 0; a < size; a++) len += (he_she_vec[a] * he_she_vec[a]); //||b||
      proj = dot/sqrt(len);
      printf("projections length of the %s onto she -> he direction: %f\n", gen_words[word] , dot);
      fprintf(score_file, "%s %f\n", gen_words[word], dot);
      //reset all variables
      dot = 0;
      len = 0;
      proj = 0;
    }
  }
    
  fclose(score_file);
  return 0;
}
