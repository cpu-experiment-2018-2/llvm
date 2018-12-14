void f(int*p){
    char *l = (void*)p;
    p = (int*)(l+2);
    *p = 1;
}
