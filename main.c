#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yyjson.h"
#include "algorithm.h"
#include <windows.h>


int test() {
    // 从文件中读取(仅用于测试)
    yyjson_doc *original_doc = yyjson_read_file("examples/origin1.json", 0, NULL, NULL);
    yyjson_mut_doc *original_mut_doc = yyjson_doc_mut_copy(original_doc, NULL);
    yyjson_doc_free(original_doc);
    yyjson_doc *template_doc = yyjson_read_file("examples/template1.json", 0, NULL, NULL);
    yyjson_mut_doc *template_mut_doc = yyjson_doc_mut_copy(template_doc, NULL);
    yyjson_doc_free(template_doc);
    char *original_str = yyjson_mut_write(original_mut_doc, 0,NULL);
    char *template_str = yyjson_mut_write(template_mut_doc, 0,NULL);

    printf("original_str: %s\n", original_str);
    printf("template_str: %s\n", template_str);

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    char *res = transform(original_str, template_str);

    QueryPerformanceCounter(&end);

    // 计算运行时间
    double duration = (double) (end.QuadPart - start.QuadPart) / (double) frequency.QuadPart;

    printf("result str: %s\n", res);
    printf("time cost: %f sec\n", duration);

    return 0;
}

int main() {
    test();
    return 0;
}
