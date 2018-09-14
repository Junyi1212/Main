#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Run: ./ArgOpt -i -lr 'hi there' -f fread.c -q
//Run: ./ArgOpt -f

//全局变量
// extern char *optarg;
// extern int optind;
// extern int opterr;
// extern int optopt;

int main(int argc, char *argv[])
{
    int opt;

	//f后面有冒号(：)，则表明f选项后有关联参数
	//返回值opt是argv数组中的下一个选项字符，循环调用getopt依次获得每个选项
    while((opt = getopt(argc, argv, ":if:lr")) != -1) {
        switch(opt) {
        case 'i':
        case 'l':
        case 'r':
            printf("option: %c\n", opt);
            break;
        case 'f':
			//optarg指向-f后面的关联参数
            printf("filename: %s\n", optarg);
            break;
        case ':':
			//如果-f后面的关联参数没有，通常返回问号(?);如果字符串中的第一个字符设置为冒号(:),则返回冒号(:)
            printf("option needs a value\n");
            break;
        case '?':
			//无法识别的选项返回问号(?)，并保存在optopt中
            printf("unknown option: %c\n", optopt);
            break;
        }
    }
	
	//getopt实际上重写了argv数组，把所有的非选项参数集中在后面，从argv[optind]开始
    for(; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
    exit(0);
}
