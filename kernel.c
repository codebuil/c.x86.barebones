 #include <stdint.h>
 int video;
 typedef int size_t;
 int NULL;
 char *keymap;
 volatile unsigned int *timerss;
 void uintToStr(unsigned int num, char *str) {
    int i = 0;
    char temp[11]; // Armazena a representação reversa do número (máximo de 10 dígitos + nulo)

    // Lidar com o caso especial de 0
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    // Converter o número para uma sequência reversa de caracteres
    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Inverter a sequência de caracteres para obter a representação correta
    int j;
    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    
    // Adicionar o caractere nulo de terminação
    str[j] = '\0';
}
 // Variável global para controlar o início da memória
static unsigned char *memoryStart;
 void memfill(void *dest, size_t length, unsigned char value) {
    // Cast o ponteiro para unsigned char* para permitir o preenchimento byte a byte
    unsigned char *d = (unsigned char *)dest;

    // Preencha os bytes da memória com o valor especificado
    for (size_t i = 0; i < length; i++) {
        d[i] = value;
    }
}

void memcopy(void *dest, const void *src, size_t length) {
    // Cast os ponteiros para unsigned char* para permitir a cópia byte a byte
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    // Copie os bytes de src para dest
    for (size_t i = 0; i < length; i++) {
        d[i] = s[i];
    }
}
 int *malloc(int length) {
    // Verifica se há espaço suficiente na memória
    unsigned char *memoryEnd = memoryStart + length + sizeof(int) * 2;
    if (memoryEnd > (unsigned char *)0x300000) {
        // Não há espaço suficiente
        return (int*)NULL;
    }

    // Preenche o bloco de memória com zeros
    memfill(memoryStart, length + sizeof(int) * 2, 0);

    // Preenche o inteiro do cabeçalho
    *(int *)memoryStart = length;

    // Preenche o inteiro do rodapé
    *(int *)(memoryStart + length + sizeof(int)) = length;

    // Retorna o endereço após os inteiros do cabeçalho
    return (int *)(memoryStart + sizeof(int));
}

 // Função para contar o tamanho de uma string
size_t lens(const char* str) {
    size_t length = 0;
    while (*str) {
        length++;
        str++;
    }
    return length;
}

// Função para copiar uma string e adicionar seu comprimento ao valor
void strcpys(char* dest, const char* src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0'; // Null-terminator
}
 void printc(char b)
        {
        		   int i=video;
			   char *fbp=(char* )i;
			   	*((char *)(fbp)) =(char)b;
			   	*((char *)(fbp+1)) =(char)0x17;
			   video++;
			   video++;  
        }

void prints(char *c)
{
	int counter=0;
	while(c[counter]!=0){
		printc(c[counter]);
		counter++;
	}
}

void cls(){

char *dest=(char* )0xb8000;
size_t length=4000;
unsigned int value=0x17;
    // Cast o ponteiro para unsigned char* para permitir o preenchimento byte a byte
    unsigned char *d = (unsigned char *)dest;

    // Preencha os bytes da memória com o valor especificado
    for (size_t i = 0; i < length; i=i+2) {
        d[i] = 32;
        d[i+1] = value;
    }
}
unsigned char getKeys() {
    unsigned char value;
    
    // Usando uma instrução asm inline para ler a porta 0x60
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(0x60));
    
    return value;
}
// Função para obter uma string até que a tecla "Enter" seja pressionada
char* getstring() {
    char* buffer = (char*)NULL;
    char* tempBuffer = (char*)NULL;
    int bufferSize = 0;
    int bufferLength = 0;
    char character ;

    while (1) {
        // Obtém a tecla atual
        unsigned char key = getKeys();

        // Verifica se a tecla é "Enter" (0x1C)
        

        // Converte a tecla para caractere e a coloca no buffer
        
        if (key<127){
        key=key;
        if (key == 0x1C) {
            break; // Tecla "Enter" pressionada, saia do loop
        }
        
            character = keymap[key];
            if (bufferSize == 0) {
                bufferSize = 1025;
                buffer = (char*)malloc(1025);
                if (buffer == (char*)NULL) {
                    return (char*)NULL; // Falha na alocação de memória
                }
            }
            /*
            if (bufferLength + 1 >= bufferSize) {
                // Redimensiona o buffer conforme necessário
                bufferSize *= 2;
                tempBuffer = (char*)realloc(buffer, bufferSize);
                if (tempBuffer == NULL) {
                    free(buffer);
                    return NULL; // Falha na realocação de memória
                }
                buffer = tempBuffer;
            }
            */
            buffer[bufferLength++] = character;
            printc(character);
            buffer[bufferLength] = '\0'; // Null-terminate a string
            while(!(getKeys()>127)){}
        }
        }
        
    

    return buffer;
}
void gets(){
while(1){
	if(getKeys()<127){
	printc(keymap[getKeys()]);
	
	}else{
	printc(32);	
	}
	video--;
	video--;
}}
// Função para pausar a execução por um número de segundos
void sleep(int seconds) {
    volatile unsigned int *timer = (volatile unsigned int *)0x46C;
    unsigned long long nows, after;

    // Converte segundos em ticks (1 tick = 1/18.2 segundos)
    unsigned int seconds18 = (unsigned int)seconds * 18;

    // Obtém o tempo atual em ticks
    nows = *timer;

    // Calcula o tempo após o qual queremos acordar
    after = nows + seconds18;

    if (after < nows) {
        // Se after for menor que nows, significa que ocorreu um overflow do timer
        // Portanto, esperamos até que nows seja maior ou igual a after
        while (*timer < after) {
            // Espera
        }
    } else {
        // Se after for maior que nows, simplesmente esperamos até que nows seja maior ou igual a after
        while (*timer < after) {
            // Espera
        }
    }
}
// Função para ler o timer da BIOS, remover o bit mais significativo e dividir pelo divisor (inteiro)
int readTimerAndDivide() {
    timerss=timerss+4;
    unsigned int timerValue;

    // Lê o valor do timer da BIOS
    timerValue = *timerss;

    
    

    // Divide o valor pelo divisor especificado
    int result = (int)(timerValue );

    return result;
}


// Função para obter um dígito de 0 a 9 a partir do resultado da divisão
int digit(int i) {
    int maxDivisor = i;
    int result = readTimerAndDivide(maxDivisor);
    result=(result & 255)/i;
    // Mapeia o resultado para um dígito de 0 a 9
    return result;
}
// Função para retornar o caractere ASCII correspondente a um dígito de 0 a 9
char digitToChar(int digit) {
    // Verifica se o dígito está dentro do intervalo válido (0 a 9)
    if (digit < 0 || digit > 9) {
        return '\0'; // Retorna um caractere nulo para indicar erro
    }

    // Mapeia os dígitos aos seus valores ASCII (0 a 9)
    char asciiChars[] = "0123456789";

    // Retorna o caractere correspondente
    return asciiChars[digit];
}
// Função para desenhar uma janela de cores na tela de texto
void draw_window(int x, int y, int w, int h, uint8_t color, uint8_t c) {
    uint16_t *video_memory = (uint16_t *)0xB8000;
    uint16_t attribute = (color << 8);
    uint16_t cc = c;

    // Limite as coordenadas para evitar desenhar fora da tela
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    for (int row = y; row < y + h; row++) {
        for (int col = x; col < x + w; col++) {
            uint16_t *location = video_memory + (row * 80 + col);
            *location = attribute | cc; // Espaço em branco com a cor especificada
        }
    }
}


 void kernel_main()
        {
        		   int c=0;
        		   char b[1024];
			   char *a="wait....: ";
			   video=0xb8000;
			   keymap="==1234567890-===qwertyuiop====asdfghjkl====\\zxcvbnm,.;/==== ";
			   memoryStart = (unsigned char *)0x200000;
			   timerss=(volatile unsigned int *)0xF0000;
			   NULL=0;		
			   cls();
			   for(c=0;c<6;c++){
			   	draw_window(1*c, 1*c, 20, 10, (c)*16,'-');
			   	
			   }
			   
			   
			   	
			   
			  
        }
 

