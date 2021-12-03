#include <stdio.h>


struct Complex {
  double real, imaginary;
};

struct Complex complex_product(struct Complex c1, struct Complex c2){
  struct Complex output;

  output.real = c1.real * c2.real - c1.imaginary * c2.imaginary;
  output.imaginary = c1.real * c2.imaginary + c1.imaginary * c2.real;
  return output;
}

void describe_complex_number(struct Complex c1){
  printf("%2.2f + %2.2f i\n", c1.real, c1.imaginary);
}

int main(){
  struct Complex c1, c2;
  double c1_real, c1_imaginary, c2_real, c2_imaginary;
  printf("Enter real part of c1: ");
  scanf("%lf", &c1.real);
  printf("Enter imaginary part of c1: ");
  scanf("%lf", &c1.imaginary);
  printf("Enter real part of c2: ");
  scanf("%lf", &c2.real);
  printf("Enter imaginary part of c2: ");
  scanf("%lf", &c2.imaginary);
  printf("Complex number: ");
  describe_complex_number(complex_product(c1, c2));
  printf("\n");
}
