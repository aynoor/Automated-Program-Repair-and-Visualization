#include <stdio.h>
#include <math.h>

int greater_root(int a,int  b, int c) {
    int larger_root;
    int determinant = (b*b) - (4*a*c);
    if (determinant >= 0) {
        int root1 = ((-b) + sqrt(determinant))/(2*a);
        int root2 = ((-b) - sqrt(determinant))/(2*a);
        if (root1>root2) {
            larger_root = root2;
        } else {
            larger_root = root2;
        }
     } 
     return larger_root;
}