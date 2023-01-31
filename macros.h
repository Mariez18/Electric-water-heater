#define toggle_bit(x,bit)  x^(1<<bit)

#define clear_bit(x,bit)  x&~(1<<bit)

#define Read_bit(x,bit)  (x&(1<<bit))>>bit

#define set_bit(x,bit)    x|(1<<bit)
