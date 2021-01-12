#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_EXIST -1

typedef struct{
    char**  seq;        // 待編碼輸入的一堆字符串
    int*    code;       // 編碼序號
    int     size;       // 字典已經有幾個編碼符
    int     max_size;   // 字典最多容納多少編碼
} Dictionary;

char* inputText ="TOBEORNOTTOBEORTOBEORNOT#";
int arr[16]={20,15,2,5,15,18,14,15,20,27,29,31,36,30,32,34}; //解碼數組 一編解碼一邊構建字典

//插入一段字符串到字典
void insert_seq(Dictionary* dict, char* seq) {
    int i = dict->size; //目前放的位置
    dict->seq[i]    =   malloc(sizeof(char)* strlen(seq)+1);// 新建一個字符串 +1 結束字符串
    dict->code[i]   =  i;
    dict->size++;
    strcpy(dict->seq[i], seq);//填上第i個字符串 插入
}

// 把26個字母a~Z 添加到字典裡面
void init_dictionary(Dictionary* dict, int max_size) {
    dict->max_size = max_size;
    dict->size  =   0;
    dict->seq   =   malloc(sizeof(char*) * max_size);
    dict->code  =   malloc(sizeof(int)* max_size);

    insert_seq(dict, "#"); //略掉最初 防a變0 b變1 
    char seq[2] =   "A";
    for (int i=0; i<26; i++){
        insert_seq(dict,seq);
        seq[0]++;
    }
}

// encode  seq -> code
int get_seq_code(Dictionary* dict, char* seq){
    for (int i=0; i<dict->size; i++){
        if(strcmp(dict->seq[i],seq)== 0){
            return dict->code[i];
        }
    }
    return NOT_EXIST;
}
 
// decode  code -> seq
char* get_code_seq(Dictionary* dict, int code){
    if(code <0 || code >= dict->size){
        return NULL;
    }
    else{
        int i=code;
        return dict->seq[i];
    }
}


void print_dictionary(Dictionary* dict){
    printf("=================\n");
    printf("Code       Sequence\n");
    printf("=================\n");
    for(int i=0; i<dict->size; i++){
        printf("%5d%7c",dict->code[i], ' ');
        printf("%s\n", dict->seq[i]);
    }
  //  printf("=================\n");

}

void lzw_encode(char* text, Dictionary* dict) {
    char current[1000];
    char next;
    int code;
    int i=0;
    unsigned int j=0;
    double ans;
    printf("=================================\n");
    printf("Current  Next   Code   Dictionary\n");
    printf("=================================\n");
    while (i < strlen(text)){
        sprintf(current, "%c", text[i]);  //打入第i的字母
        next    = text[i+1];   //下一個
        while (get_seq_code(dict,current)  != NOT_EXIST){ // 若字母在字典裡面找不到current的話 必須往後探測一位
            sprintf(current,  "%s%c", current,next); //  新組合放入 current
            i++;               //next 下一個
            next = text[i+1];  // 探測 next ＝下一個next 何時停？
            if (next == '\0'){ //  next 下個等於空字符時停掉

             }
        }

        current[strlen(current)-1] = '\0'; // 往前推一個格子 TOB 把B去掉 current 最後一個變成 No byte
        next = text[i]; // next 要縮一格回去 因為 上面有next = text[i+1];
        code = get_seq_code(dict,current); // 把current 編碼下來
       if(code != NOT_EXIST)
         j++;

        printf("%s%7c", current ,' ');
        printf("%c%7c" ,next,' ' );
        printf("%d%7c" ,code,' ' );
        sprintf(current,"%s%c", current,next);//新組合
        printf("%s:%u\n" ,current ,j+26);
        insert_seq(dict,current);  // 新組合current放入字典裡面

       
        
//輸出
        //printf("%d %s %s:%d\n", code ,current , current,j+26);
        //25 bytes 壓縮到-> 17 bytes
    }
    printf("=================================\n");
    ans = ((float)(strlen(inputText)-j)/strlen(inputText))*100;
    printf("Input Text is %s: total %lu bytes -> coded %u bytes   Data reduction rate : %1.f%%  \n" ,inputText ,strlen(inputText),j ,ans);
}

void lwz_decode(int codes[], int n ,Dictionary* dict){
    int code;
    char prev[1000];
    char* output;
int loop =0 ;
    code = codes[0];
    output = get_code_seq(dict,code);

     printf( "Input coded is ");
     for(loop = 0; loop < 16; loop++)
      printf( "%d ", arr[loop]);

    printf("\n=================================\n");
    printf("Coded    Output    Dictionary\n");
    printf("=================================\n");
   // printf("%s\n" ,output);
    printf("%d%7c", code ,' ');//第一個 碼
    printf("%s%7c\n", output,' '); //第一個 解碼字 Ｔ
    printf("%s", prev);//第一個 是空

    int i;
    for (i=1; i<n;i++){
        code = codes[i];
        strcpy(prev,output);
        output = get_code_seq(dict,code);
        sprintf(prev,"%s%c",prev,output[0]);
        insert_seq(dict,prev);
        printf("%d%7c", code ,' ');
        //printf("%s\n" ,output);
        printf("%s%7c", output,' ');
        printf("%s%7c\n", prev ,' ');
    }
  

}

int main(){
    Dictionary dict;
    init_dictionary(&dict,1000); //開1000空間字典

    int size = sizeof arr / sizeof *arr;
    //lwz_decode(arr,sizeof(size),&dict);
    //print_dictionary(dict);
    lwz_decode(arr,16,&dict);    
   // printf("%d\n,", get_seq_code(&dict,"B")); //test  get seq code index 
    //lzw_encode(inputText,&dict);
    
   
  return 0;
}
