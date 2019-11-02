#include<stdio.h>
#include<string.h>

char bflag = 0;

char *ifile(char *name, const char *target, const char *hash)
{
    char *ref = "-diff";
    char *src = ".tex";

    strcat(name, target);
    strcat(name, ref);
    strcat(name, hash);
    strcat(name, src);

    return name;
}

int rewrite(FILE *fp, const int *line, const char REMOVETEXROOT, const char BIB, const char CFONT, const char STYLE, const char *fadd, const char *fdel, const char HEAD)
{
    int i, j = 0;
    char len[2048] = {0};

    for(i = 0; line[i] != 0; i++){
        if(line[i] != ' '){
            len[j] = (char)line[i];
            j++;
        }
    }

    if(strncmp(len, "%!TEXroot", 9) == 0 && REMOVETEXROOT){
        ;
    }else if(strncmp(len, "\\DIFdelbegin%DIFDELCMD<\\bibliography{bibliography}", 50) == 0 && BIB){
        bflag = 1;
        fprintf(fp, "\\begin{thebibliography}{99}");
    }else if(strncmp(len, "\\DIFaddend", 10) == 0 && bflag && BIB){
        bflag = 0;
        fprintf(fp, "\\end{thebibliography}");
    }else if(strncmp(len, "\\bibitem", 8) != 0 && bflag && BIB){
        for(i = 0; len[i] != 0; i++){
            if(len[i] == 'D' && len[i+1] == 'I' && len[i+2] == 'F' && len[i+3] == 'a' && len[i+4] == 'd' && len[i+5] == 'd' && len[i+6] == '{'){
                fprintf(fp, len+i+7);
                break;
            }
        }
    }else if(strncmp(len, "\\providecommand{\\DIFadd}", 24) == 0 && CFONT && STYLE){
        fprintf(fp, fadd);
    }else if(strncmp(len, "\\providecommand{\\DIFdel}", 24) == 0 && CFONT && STYLE){
        fprintf(fp, fdel);
    }else if(strncmp(len, "\\begin{document}", 16) == 0 && HEAD){
        fprintf(fp, "%s\n", len);
        fprintf(fp, "\\vskip-50mm { \\large \\rightline{ {\\color{red}赤字は削除個所}，{\\color{blue}青字は追記個所}を示しております} }");
    }else{
        for(i = 0; line[i] != 0; i++){
            fputc(line[i], fp);
        }
    }

    fputc('\n', fp);

    return 0;
}

int main(int argc, char *argv[])
{
    const char *TARGET = argv[1];
    const char *DIFFHASH = argv[2];
    char REMOVETEXROOT = 0;
    char BIB = 0;
    char CFONT = 1;
    char STYLE = 0;
    const char *FADD = argv[7];
    const char *FDEL = argv[8];
    char HEAD = 0;

    int ch, i;
    char name[256] = {0};
    int len[2048] = {0};
    FILE *fpi, *fpo;

    if(strcmp(argv[3], "True") == 0 || strcmp(argv[3], "true") == 0 || strcmp(argv[3], "TRUE") == 0){
        REMOVETEXROOT = 1;
    }
    if(strcmp(argv[4], "True") == 0 || strcmp(argv[4], "true") == 0 || strcmp(argv[4], "TRUE") == 0){
        BIB = 1;
    }
    if(strcmp(argv[5], "True") == 0 || strcmp(argv[5], "true") == 0 || strcmp(argv[5], "TRUE") == 0){
        CFONT = 0;
    }
    if(strcmp(argv[6], "True") == 0 || strcmp(argv[6], "true") == 0 || strcmp(argv[6], "TRUE") == 0){
        STYLE = 1;
    }
    if(strcmp(argv[9], "True") == 0 || strcmp(argv[9], "true") == 0 || strcmp(argv[9], "TRUE") == 0){
        HEAD = 1;
    }

    fpi = fopen(ifile(name, TARGET, DIFFHASH), "r");
    fpo = fopen("temp.tex", "w");

    if(fpi == NULL || fpo == NULL){
        printf("ファイルのOpenに失敗しました\n");

        return -1;
    }

    i = 0;
    while( (ch = fgetc(fpi)) != EOF ){
        if(ch != '\n'){
            len[i] = ch;
            i++;
        }else{
            len[i] = 0;
            rewrite(fpo, len, REMOVETEXROOT, BIB, CFONT, STYLE, FADD, FDEL, HEAD);
            i = 0;
        }
    }

    fclose(fpi);
    fclose(fpo);

    return 0;
}
