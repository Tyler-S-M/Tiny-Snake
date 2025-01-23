#include <iostream>

#define board_size 30

int main(){

    int snek[400] = {0, 0, board_size / 2, board_size / 2, board_size / 2, board_size / 2, board_size / 2, board_size / 2};
    int fruit[2] = {(board_size / 2) + 2, board_size / 2};
    int board[board_size][board_size];

    char buffer[1] = {0};
    int next = 8;

    unsigned short lfsr = 0xACE1u;
    unsigned bit;

    //setup
    for (int i = 391; i > 9; i--)
        snek[i] = -1;

    std::ios_base::sync_with_stdio(false);
    
    while(1){

        for (int i = 0; i < board_size * board_size; i++)
            board[i / board_size][i % board_size] = 32;
        
        //update snake body
        for (int i = next - 1; i > 1; i -= 2)
            for (int j = 0; j < 2; j++)
                snek[i - j] = (snek[i - 2 - j] + (i == 2 || i == 3) * ((snek[i - 2 - j] + snek[i - j] > -1) ? (snek[i - j]) : board_size)) % board_size;
            
        for (int i = next - 1; i > 1; i -= 2)
            board[snek[i - 1]][snek[i]] += 42;
            
        //check if head hit fruit (or body)
        if ((board[snek[2]][snek[3]] - 32 != 42) && buffer[0] != 0)
            {system("stty cooked"); exit(0);} //cheated here just because broken stdout is not fun, not needed though
            
        else if ((snek[2] % board_size) == fruit[0] && (snek[3] % board_size) == fruit[1])
            for (int i = 0; i < 2; i++)
                fruit[i] = (((snek[next++] = snek[next - 2])) + 5 - ((lfsr = (lfsr >> 1) | ((bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1) << 15)) % 10)) % (board_size - 1);
        
        //print the board
        system("stty cooked"); 

        std::cout << "\033[2J\033[1;1H" << std::endl;
        
        for (int i = 0; i < board_size * board_size; i++)
            std::cout << ((i % board_size == 0) ? "\n" : " ") << ((((int)(i / board_size)) == fruit[0] && ((int)(i % board_size)) == fruit[1]) ? 'O' : char(((board[i / board_size][i % board_size] > 32) ? 42 : 32) )) << std::flush;

        system("stty raw"); 

        //take inputs
        if (std::cin.readsome(buffer, 1) >= 1)
            for (int i = 0; i < 2; i++)
                snek[i] = (i == 0) * ((buffer[0] == 'w') * -1 + (buffer[0] == 's') * 1) + (i == 1) * ((buffer[0] == 'd') * 1 + (buffer[0] == 'a') * -1);

        //don't let game run too quick (adjust manually for tick speed)
        for (unsigned long long i = 0; i < 390000000; i++)
            __asm__ volatile ("nop");
        
    }

    return 0;
}
