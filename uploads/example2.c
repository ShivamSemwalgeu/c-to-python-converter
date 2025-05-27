int main(){
int* ptr, c;
c = 5;
ptr = &c;
c = 1;
printf("%d\n", c);    
printf("%d\n", *ptr);  
}
