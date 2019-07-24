#include "RG.h"
#include <iostream>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;

size_t  heap_malloc_total, heap_free_total,mmap_total, mmap_count;

void print_info()
{
    struct mallinfo mi = mallinfo();
    printf("count by itself:\n");
    printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
           heap_malloc_total*1024, heap_free_total*1024, heap_malloc_total*1024-heap_free_total*1024,
           mmap_total*1024, mmap_count);
    printf("count by mallinfo:\n");
    printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
           mi.arena, mi.fordblks, mi.uordblks,
           mi.hblkhd, mi.hblks);
    printf("from malloc_stats:\n");
    malloc_stats();
}

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

int main() {
    double starttime, endtime;
    starttime = get_time();
    Petri ptnet;
    char filename[]="model.pnml";
    ptnet.getSize(filename);
    if(ptnet.NUPN)
    {
        ptnet.readNUPN(filename);
    }
    else
        ptnet.readPNML(filename);

    NUPN_RG *graph = new NUPN_RG(&ptnet);
    NUPN_RGNode *initnode=graph->RGinitialnode();
    graph->Generate(initnode);
    cout<<"STATESPACE:"<<graph->nodecount<<endl;
    endtime = get_time();
    cout<<"RUNTIME:"<<endtime-starttime<<endl;
    cout<<endl;
    print_info();
//    malloc_stats();
    delete graph;

//    ptnet.printPlace();
//    ptnet.printTransition();
//    ptnet.printGraph();
//    ptnet.printUnit();

//    RG *graph = new RG(&ptnet);
//    RGNode *initnode = graph->RGinitialnode();
//    graph->Generate(initnode);
//    cout<<"STATESPACE: "<<graph->nodecount<<endl;
//    endtime = get_time();
//    cout<<"RUNTIME:"<<endtime-starttime<<endl;
//    cout<<endl;
//    print_info();
//    cout<<endl;
//    delete graph;
//    print_info();

//    RG *graph1 = new RG(&ptnet);
//    RGNode *initnode1 = graph1->RGinitialnode();
//    graph1->Generate(initnode1);
//    cout<<"STATESPACE: "<<graph1->nodecount<<endl;
//    endtime = get_time();
//    cout<<"RUNTIME:"<<endtime-starttime<<endl;
//    delete graph1;
    return 0;
}