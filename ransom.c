#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);
void merge(char **array, int low, int mid, int high);
void merge_sort(char** array, int low, int high);
void free_mallocs(char **arr, int len);

/*----------------------------CONSTANTS------------------------------*/
enum CONSTS {WORD_MAX_SIZE = 6}; //Remeber '/0' at the end



/**  
    * @brief: this function will check if it is possible to make the 
    *         ransom note from Words in array magazine.
    * @param magazine_count: Nuber of words in the magzine. Int IN[1,3000].
    * @param magazine: Array of the words in the magazine. Word len in [1,5].
    * @param note_count: Nuber of words in the note. Integer between [1,3000].
    * @param note: Array of the words in the note. Word length between [1,5].
*/


void checkMagazine(int magazine_count, char** magazine, int note_count, char** note) {
    /*Method of work:
    1. Check lengths are valid.
    2. Create working array.
    3. Sort both working arrays.
    4. Go over note array, look for matching words in magazine array.

    We used sorted arrays instead of linked lists as proposed in the clue
    because we believe this implementation  is easier and has less complexity
    (O(nlogn) - worst case scenario).
    */

    //Check lengths
    if (note_count > magazine_count){
        printf("No");
        return;
    }

    //Work arrays creation
    char **mag_arr = malloc(sizeof(char *) * magazine_count);
    if (mag_arr == NULL){
        fprintf(stderr,"Malloc failed\n");
        return;
    }

    char **note_arr = malloc(sizeof(char *) * note_count);
    if (mag_arr == NULL){
        fprintf(stderr,"Malloc failed\n");
        free(mag_arr);
        return;
    }

    for(int i=0; i<magazine_count;i++){
        //Malloc the maximum size of the a word in bytes.
        mag_arr[i] = malloc(sizeof(char) * WORD_MAX_SIZE);   
        if (mag_arr[i] == NULL){
            fprintf(stderr,"Malloc failed\n");
            free_mallocs(mag_arr, i);
            free(mag_arr);
            return;
        }
        strcpy(mag_arr[i],magazine[i]);
    }

    for(int j=0; j<note_count;j++){
        //Malloc the size of the string in bytes.
        note_arr[j] = malloc(sizeof(char) * WORD_MAX_SIZE);   
        if (note_arr[j] == NULL){
            fprintf(stderr,"Malloc failed\n");
            free_mallocs(mag_arr, magazine_count);
            free_mallocs(note_arr, j);
            free(mag_arr);
            free(note_arr);
            return;
        }
        strcpy(note_arr[j],note[j]);
    }

    //Sort arrays
    merge_sort(mag_arr, 0, magazine_count - 1);
    merge_sort(note_arr, 0, note_count - 1);

    //Go over arrays
    int mag_idx = 0;
    int match_count = 0;
    int diff = 0;
    //If match_count = note_count, we have found all words.
    for(int i=0; (i < note_count) && (match_count < note_count) && 
                                 (mag_idx < magazine_count); i++){
        diff = strcmp(note_arr[i],mag_arr[mag_idx]);
        while(diff > 0 && mag_idx < magazine_count){
            mag_idx++;
            diff = strcmp(note_arr[i],mag_arr[mag_idx]);
        }
        if (diff == 0){
            match_count++;
            mag_idx++;
        } else{     
            printf("No");
            free_mallocs(mag_arr, magazine_count);
            free_mallocs(note_arr, note_count);
            free(mag_arr);
            free(note_arr);
            return;
        }   /*If we exited the while without a match there will be
              no more matches for specific word
              and we can stop searching. */
    }

    if(match_count == note_count){
        printf("Yes");
    } else {
        printf("No");
    }
    free_mallocs(mag_arr, magazine_count);
    free_mallocs(note_arr, note_count);
    free(mag_arr);
    free(note_arr);
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

/**  
    * @brief: This function will merge the arrays while sorting.
    * @param array: The array that needs to be sorted.
    * @param low: Lowest index of array.
    * @param mid: Middle of array.
    * @param high: Highest index in array.
*/
void merge(char **array, int low, int mid, int high){
    //Init
    int left_len = mid - low + 1; //Length of left array
    int right_len = high - mid;

    //Mallocs of big arrays
    char **left_arr = malloc(sizeof(char *) * left_len);
    if (left_arr == NULL){
        fprintf(stderr,"Malloc failed\n");
        return;
    }
    char **right_arr = malloc(sizeof(char *) * right_len);
    if (right_arr == NULL){
        fprintf(stderr,"Malloc failed\n");
        free(left_arr);
        return;
    }

    //Array creating
    for(int i=0; i<left_len;i++){
        //Malloc max size of string
        left_arr[i] = malloc(sizeof(char*) * WORD_MAX_SIZE);
        if (left_arr[i] == NULL){
            fprintf(stderr,"Malloc failed\n");
            free_mallocs(left_arr, i);
            free(left_arr);
            free(right_arr);
            return;
        }
        strcpy(left_arr[i],array[low + i]);
    }

    for(int j=0; j<right_len;j++){
        //Malloc the size of the string in bytes.
        right_arr[j] = malloc(sizeof(char*) * WORD_MAX_SIZE);   
        if (right_arr[j] == NULL){
            fprintf(stderr,"Malloc failed\n");
            free_mallocs(left_arr, left_len);
            free_mallocs(right_arr, j);
            free(left_arr);
            free(right_arr);
            return;
        }
        strcpy(right_arr[j],array[mid + j +1]);
    }

    //Merging
    int l_idx = 0;  //Left index.
    int r_idx = 0;  //Right index.
    int a_idx = low;    //Big array index.

    while(l_idx < left_len && r_idx < right_len){
        if(strcmp(left_arr[l_idx], right_arr[r_idx]) <= 0){
            strcpy(array[a_idx], left_arr[l_idx]);
            l_idx++;
        } else{
            strcpy(array[a_idx], right_arr[r_idx]);
            r_idx++;
        }
        a_idx++;
    }

    //Finish coping strings to big array
    while(l_idx < left_len){
        strcpy(array[a_idx], left_arr[l_idx]);
        l_idx++;
        a_idx++;
    }

    while(r_idx < right_len){
        strcpy(array[a_idx], right_arr[r_idx]);
        r_idx++;
        a_idx++;
    }
    free_mallocs(left_arr, left_len);
    free_mallocs(right_arr, right_len);
    free(left_arr);
    free(right_arr);


}

/**  
    * @brief: This function will manage the sorting process.
    * @param array: The array that needs to be sorted.
    * @param low: Lowest index of array.
    * @param high: Highest index in array.
*/
void merge_sort(char **array, int low, int high){
    if(low < high){
        int mid = (low + high) / sizeof(short); //Size of short = 2
        merge_sort(array, low, mid);
        merge_sort(array, mid + 1, high);
        merge(array, low, mid, high);
    }
}

/**  
    * @brief: This function will free array of mallocs.
    * @param array: The array that needs it's values to be freed.
*/
void free_mallocs(char **array, int len){
    for(int i=0; i < len; i++){
        free(array[i]);
    }
}
