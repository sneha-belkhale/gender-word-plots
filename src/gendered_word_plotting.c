//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> // mac os x

//try using reditt data


const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

int main(int argc, char **argv) {
  FILE *f;
  char st1[max_size];
  char bestw[N][max_size];
  char file_name[max_size], st[100][max_size];
  float dist, bestd[N], vec[max_size];
  long long words, size, a, b, c, d, cn, bi[100], gen_words_pos[100];
  float dot, len, proj;  char ch;
  float *M;
  char *vocab;
  if (argc < 2) {
    printf("Usage: ./word-analogy <FILE>\nwhere FILE contains word projections in the BINARY FORMAT\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
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
    
    
  // initialize a list of words to project onto the she - he axis
    
   
    
     char gen_words[129][max_size] = {"tote", "treats", "subject", "heavy", "commit", "game", "browsing", "sites", "seconds", "slow", "arrival", "tactical",
         "crafts",
         "trimester", "tanning", "ultrasound",
         "modeling", "beautiful", "cake", "victims", "looks", "sewing", "dress", "dance", "letters", "nuclear", "hay", "quit",
         "pageant", "earrings", "divorce", "firms",
         "yard",
         "seeking", "ties", "guru", "buddy",
         "salon", "sassy", "breasts",
         "dancers", "thighs", "lust", "lobby",
         "user", "identity", "drop", "reel", "firepower",
         "busy", "parts", "hoped", "command", "housing", "caused", "ill", "scrimmage",
         "voters",
         "vases", "frost", "governor", "sharply",
         "rule",
         "builder", "drafted", "brilliant", "genius",
         "cocky", "journeyman",
         "buddies", "burly", "war", "fight", "guns", "firearm", "confusion", "rape", "sex", "sexy", "murder", "fat", "slut", "chubby", "curvy", "slap", "leader", "programming", "power", "funny", "giggle", "chuckle", "smart", "weak", "bitch", "gossip", "bossy", "shrill", "dramatic", "catty", "cold", "ditzy", "prude", "quiet", "teeth", "mind", "thought", "police", "deity", "hands", "butt", "sociopath", "iphone", "texting", "texted", "surrender", "villain", "armpit", "heart", "love", "bdsm", "eat", "diet", "makeup", "melee", "videogames", "sports", "religion", "emotional","depression"
         
        } ;
    
    
    for (a=0; a<129; a++){
        //find the word in the set
        for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], gen_words[a])) break;
            if (b == words) b = 0;
            //find the words position in vocabulary -- bi[0] is the first words position
            gen_words_pos[a] = b;
    }
    
    long long she, he;
    char she_char[max_size] = "she";
    char he_char[max_size] = "he";
    
    
    for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], she_char)) break;
    if (b == words) b = 0;
    //find the words position in vocabulary -- bi[0] is the first words position
    she = b;
    
    for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], he_char)) break;
    if (b == words) b = 0;
    //find the words position in vocabulary -- bi[0] is the first words position
    he = b;
    

   //find the vector direction he->she

    for (a = 0; a < size; a++) vec[a] = M[a + she * size] - M[a + he * size];
    
    
    
    FILE *score_file = fopen("scores.txt","wb");
    


    
    
    for (int word = 0; word<129; word ++ ){
  //go through genwords and find their projection on she -. he
    
    for (a = 0; a < size; a++) dot += (vec[a] * M[a + gen_words_pos[word] * size]);
    // ||b||
    for (a = 0; a < size; a++) len += (vec[a] * vec[a]);
    
    proj = dot/sqrt(len);
    
    printf("projections length of the %s onto she -> he direction: %f\n", gen_words[word] , dot);
    fprintf(score_file, "%s %f\n", gen_words[word], dot);
    //reset all variables
    //save and graph later.
    dot = 0;
    len = 0;
    proj = 0;
    
    }
    
    fclose(score_file);
     return 0;
}
