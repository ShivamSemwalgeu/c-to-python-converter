int main(){
int* ptr, c;
c = 5;
ptr = &c;
c = 1;
printf("%d\n", c);    // Output: 1
printf("%d\n", *ptr);  // Ouptut: 1
}
