#ifndef __STR__
#define __STR__

class Str
{
    private :
        char *str; // content of Str
        int len; // length of Str
    public :
        Str(int leng); // Constructor: Create the Str which len = leng
        Str(const char *neyong); // Constructor: Create the Str which content is *neyong
        ~Str(); // deconstructor
        int length(void); // return the len of Str
        char *contents(void); // returnn the content(str) of Str
        int compare(class Str& a); // return the compare value with Str a (use strcmp)
        int compare(const char *a); // return the compare value with char *a (use strcno)
        void operator=(const char *a); // save the a in Str
        void operator=(class Str& a); // save the content, len of a in Str
};

#endif
